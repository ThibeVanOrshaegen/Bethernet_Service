#include "servicetwohandler.h"
#include "servicereceiver.h"
#include <iostream>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <mutex>
#include <zmq.hpp>

ServiceTwoHandler::ServiceTwoHandler(zmq::context_t& context)
    : ventilator(context, ZMQ_PUSH), context(1), subscriber(context, ZMQ_SUB) {
    try {
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, "thibe<quest?>", 13);
        ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
    } catch (const zmq::error_t& e) {
        std::cerr << "ZMQ Error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred during ZMQ setup." << std::endl;
    }
}

void ServiceTwoHandler::startServiceTwo(const std::string& receivedSessionId, const std::string& derdeinvoer) {
    try {
        std::cout << "ServiceTwoActive" << std::endl;
        std::cout << "ID: " << receivedSessionId << std::endl;
        std::cout << "Derdeinvoer: " << derdeinvoer << std::endl;

        std::vector<std::string> urll = {
            "https://api.polygon.io/v1/open-close/" + derdeinvoer + "/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"
        };

        QNetworkAccessManager manager;
        for (const auto& urlreal : urll) {
            QNetworkRequest networkRequest(QUrl(QString::fromStdString(urlreal)));

            QEventLoop loop;
            QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
            QNetworkReply* reply = manager.get(networkRequest);
            loop.exec();

            if (reply->error() == QNetworkReply::NoError) {
                // Read API response
                QByteArray responseData = reply->readAll();
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                QJsonObject jsonObject = jsonResponse.object();

                // Extract the "o" (OpeningPrice) value from the JSON object
                double openingPrice = jsonObject["open"].toDouble();
                std::cout << "OpeningPrice: " << openingPrice << std::endl;

                // Extract the "c" (ClosingPrice) value from the JSON object
                double closingPrice = jsonObject["close"].toDouble();
                std::cout << "ClosingPrice: " << closingPrice << std::endl;

                double Min = openingPrice - closingPrice;
                double Delen = (Min / closingPrice) * 100;
                std::cout << "Verschil: " << Delen << "%" << std::endl;

                // Send the OpeningPrice and ClosingPrice back to the sender
                std::string messageToSend = "\nOpeningPrice: " + std::to_string(openingPrice) + "\nClosingPrice: " + std::to_string(closingPrice) + "\nDifference: " + std::to_string(Delen) + " %";
                std::cout << "messageToSend: " << messageToSend << "%" << std::endl;

                std::string message = "thibe<quest!>(ServiceTwoActive)" + messageToSend + "|" + receivedSessionId;
                sendMessage(message);
            } else {
                std::cerr << "Failed to perform HTTP request: " << reply->errorString().toStdString() << std::endl;
            }
            reply->deleteLater();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in startServiceTwo: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred in startServiceTwo." << std::endl;
    }
}

void ServiceTwoHandler::sendMessage(const std::string& message) {
    try {
        zmq::message_t msg(message.size());
        memcpy(msg.data(), message.c_str(), message.size());
        ventilator.send(zmq::buffer(message), zmq::send_flags::dontwait);
    } catch (const zmq::error_t& e) {
        std::cerr << "ZMQ Error in sendMessage: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception in sendMessage: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred in sendMessage." << std::endl;
    }
}
