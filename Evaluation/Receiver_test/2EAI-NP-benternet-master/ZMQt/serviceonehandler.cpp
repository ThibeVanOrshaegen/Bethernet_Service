#include "serviceonehandler.h"
#include <iostream>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include <QTimer>
#include <exception>

ServiceOneHandler::ServiceOneHandler(zmq::context_t& context)
    : ventilator(context, ZMQ_PUSH), context(1), subscriber(context, ZMQ_SUB) {
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "thibe<quest?>", 13);
    ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
}

void ServiceOneHandler::startServiceOne(const std::string& receivedSessionId, const std::string& derdeinvoer) {
    try {
        std::lock_guard<std::mutex> lock(sessionMutex);
        std::cout << "ServiceOneActive" << std::endl;
        std::cout << "ID: " << receivedSessionId << std::endl;
        std::cout << "Derdeinvoer: " << derdeinvoer << std::endl;

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
            "https://api.polygon.io/v1/open-close/AAPL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS",
            "https://api.polygon.io/v1/open-close/KIMpL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS",
            "https://api.polygon.io/v1/open-close/CADL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"
        };

        QJsonObject combinedData;

        for (const auto& url : urls) {
            try {
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

                        if (jsonObject.contains("high") && jsonObject["high"].isDouble()) {
                            double highValue = jsonObject["high"].toDouble();
                            if (highValue < stockValue) {
                                if (jsonObject.contains("symbol") && jsonObject["symbol"].isString()) {
                                    QString stockName = jsonObject["symbol"].toString();
                                    QJsonObject messageObject;
                                    messageObject["highValue"] = highValue;
                                    combinedData[stockName] = messageObject;
                                    QString messageToSend = stockName + " HighValueNew: " + QString::number(highValue);
                                    sendMessage(messageToSend.toStdString());
                                } else {
                                    std::cerr << "Error: Symbol data is missing or invalid" << std::endl;
                                    sendMessage("Error: Symbol data is missing or invalid");
                                }
                            }
                        } else {
                            std::cerr << "Error: High value data is missing or invalid" << std::endl;
                            sendMessage("Error: High value data is missing or invalid");
                        }
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
