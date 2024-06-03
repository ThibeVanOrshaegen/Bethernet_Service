#include "serviceonehandler.h"
#include <iostream>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include <QTimer>
#include <exception>
#include <unordered_map>
#include <chrono>

ServiceOneHandler::ServiceOneHandler(zmq::context_t& context)
    : ventilator(context, ZMQ_PUSH), context(1), subscriber(context, ZMQ_SUB) {
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    std::string subscription = "thibe<quest?" + sessionId;
    subscriber.setsockopt(ZMQ_SUBSCRIBE, subscription.c_str(), subscription.size());
    //subscriber.setsockopt(ZMQ_SUBSCRIBE, "thibe<quest?>", 13);
    ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
}

struct CachedData {
    QJsonObject data;
    std::chrono::time_point<std::chrono::steady_clock> timestamp;
};

std::unordered_map<std::string, CachedData> cache;
std::mutex cacheMutex;
const std::chrono::minutes cacheDuration(10); //minutes

void ServiceOneHandler::processResponse(const QJsonObject& jsonObject, QString& combinedMessage, double stockValue) {
    if (jsonObject.contains("high") && jsonObject["high"].isDouble()) {
        double highValue = jsonObject["high"].toDouble();
        if (highValue < stockValue) {
            if (jsonObject.contains("symbol") && jsonObject["symbol"].isString()) {
                QString stockName = jsonObject["symbol"].toString();
                combinedMessage += stockName + " HighValueNew: " + QString::number(highValue) + " \n";
            } else {
                std::cerr << "Error: Symbol data is missing or invalid" << std::endl;
                sendMessage("Error: Symbol data is missing or invalid");
            }
        }
    } else {
        std::cerr << "Error: High value data is missing or invalid" << std::endl;
        sendMessage("Error: High value data is missing or invalid");
    }
}

void ServiceOneHandler::startServiceOne(const std::string& receivedSessionId, const std::string& derdeinvoer, const std::string& datetwee) {
    try {
        std::lock_guard<std::mutex> lock(sessionMutex);
        std::cout << "ServiceOneActive" << std::endl;
        std::cout << "ID: " << receivedSessionId << std::endl;
        std::cout << "Derdeinvoer: " << derdeinvoer << std::endl;
        std::cout << "datetwee: " << datetwee << std::endl;

        int daysToSubtract = QString::fromStdString(datetwee).toInt();
        std::cout << "daysToSubtract: " << daysToSubtract << std::endl;

        QDate date(2024, 5, 2);

        QString currentDateStr = date.toString("yyyy-MM-dd");
        std::string currentDate = currentDateStr.toStdString();

        QDate newDate = date.addDays(-daysToSubtract);
        std::string newDateStr = newDate.toString("yyyy-MM-dd").toStdString();

        std::cout << "currentDate: " << currentDate << std::endl;
        std::cout << "newDate: " << newDateStr << std::endl;
        std::cout << "ID: " << receivedSessionId << std::endl;

        double stockValue;
        try {
            stockValue = std::stod(derdeinvoer);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument for stock value: " << derdeinvoer << std::endl;
            sendMessage("Error: Invalid argument for stock value");
            return;
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range error for stock value: " << derdeinvoer << std::endl;
            sendMessage("Error: Out of range error for stock value");
            return;
        }

        std::vector<std::string> urls = {
            "https://api.polygon.io/v1/open-close/AAPL/"+newDateStr+"?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS",
            "https://api.polygon.io/v1/open-close/KIMpL/"+newDateStr+"?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS",
            "https://api.polygon.io/v1/open-close/CADL/"+newDateStr+"?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"
        };

        QJsonObject combinedData;
        QString combinedMessage = "thibe<quest!>(ServiceOneActive)";

        for (const auto& url : urls) {
            try {
                std::string cacheKey = url;
                {
                    std::lock_guard<std::mutex> cacheLock(cacheMutex);
                    auto it = cache.find(cacheKey);
                    if (it != cache.end()) {
                        auto now = std::chrono::steady_clock::now();
                        if (now - it->second.timestamp < cacheDuration) {
                            std::cout << "Cache hit for URL: " << cacheKey << std::endl;
                            QJsonObject cachedResponse = it->second.data;
                            processResponse(cachedResponse, combinedMessage, stockValue);
                            continue; //Skip the API call and move to the next URL
                        }
                    }
                }

                QNetworkRequest networkRequest(QUrl(QString::fromStdString(url)));
                QEventLoop loop;
                QNetworkReply* reply = manager.get(networkRequest);
                QTimer timer;
                timer.setSingleShot(true);

                QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
                QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

                timer.start(5000); // 5 seconds timeout
                loop.exec();

                if (timer.isActive()) {
                    timer.stop();

                    if (reply->error() == QNetworkReply::NoError) {
                        QByteArray responseData = reply->readAll();
                        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                        QJsonObject jsonObject = jsonResponse.object();

                        {
                            std::lock_guard<std::mutex> cacheLock(cacheMutex);
                            cache[cacheKey] = {jsonObject, std::chrono::steady_clock::now()};
                        }

                        processResponse(jsonObject, combinedMessage, stockValue);
                    } else {
                        std::cerr << "Failed to perform HTTP request: " << reply->errorString().toStdString() << std::endl;
                        sendMessage("Error: Failed to perform HTTP request: " + reply->errorString().toStdString());
                    }
                } else {
                    std::cerr << "Network request timed out" << std::endl;
                    sendMessage("Error: Network request timed out");
                    reply->abort();
                }

                reply->deleteLater();
            } catch (const std::exception& e) {
                std::cerr << "Exception occurred during network operation: " << e.what() << std::endl;
                sendMessage(std::string("Error: Exception during network operation: ") + e.what());
            } catch (...) {
                std::cerr << "Unknown error occurred during network operation" << std::endl;
                sendMessage("Error: Unknown error during network operation");
            }
        }

        sendMessage(combinedMessage.toStdString() + "|" + receivedSessionId + "|" + newDateStr);
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred in startServiceOne: " << e.what() << std::endl;
        sendMessage(std::string("Error: Exception in startServiceOne: ") + e.what());
    } catch (...) {
        std::cerr << "Unknown error occurred in startServiceOne" << std::endl;
        sendMessage("Error: Unknown error in startServiceOne");
    }
}

void ServiceOneHandler::sendMessage(const std::string& message) {
    try {
        zmq::message_t msg(message.size());
        memcpy(msg.data(), message.c_str(), message.size());
        ventilator.send(zmq::buffer(message), zmq::send_flags::dontwait);
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred while sending message: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred while sending message" << std::endl;
    }
}





