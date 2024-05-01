#include <iostream>
#include <string>
#include <zmq.hpp>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QEventLoop>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    try {
        zmq::context_t context(1);

        // Outgoing messages go out through here
        zmq::socket_t ventilator(context, ZMQ_PUSH);
        ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");

        // Incoming messages come in here
        zmq::socket_t subscriber(context, ZMQ_SUB);
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, "thibe>quest?>", 13); // Subscribe to all messages

        // Receive message
        zmq::message_t msg;
        subscriber.recv(msg);
        std::string request = std::string(static_cast<char*>(msg.data()), msg.size());
        std::cout << "Received: " << request << std::endl;

        // Make HTTP request to API
        QNetworkAccessManager manager;
        QNetworkRequest networkRequest(QUrl("https://api.polygon.io/v1/open-close/CADL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"));

        QEventLoop loop;
        QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        QNetworkReply* reply = manager.get(networkRequest);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            // Read API response
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            // Extract the "high" value from the JSON object
            double highValue = jsonObject["high"].toDouble();
            std::cout << "High Value: " << highValue << std::endl;

            if(highValue > 3.0)
            {
                std::cout << "High Value is under 3"<<std::endl;

                // Extract the stock name from the JSON object
                QString stockName = jsonObject["symbol"].toString();
                std::cout << "Stock Name: " << stockName.toStdString() << std::endl;

                // Construct JSON object
                QJsonObject messageObject;
                messageObject["stockName"] = stockName;
                messageObject["highValue"] = highValue;
                //messageObject["responseData"] = QString::fromUtf8(responseData);

                // Convert JSON object to string
                QJsonDocument doc(messageObject);
                QByteArray jsonBytes = doc.toJson(QJsonDocument::Compact);
                std::string jsonString = "thibe>quest!>" +jsonBytes.toStdString();

                // Send message back to the subscriber
                ventilator.send(zmq::buffer(jsonString), zmq::send_flags::dontwait);
            }
            else {
                std::cout << "Making the second API request..." << std::endl;

                //Make the second API request
                QNetworkRequest networkRequesttwee(QUrl("https://api.polygon.io/v1/open-close/AAPL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"));
                QNetworkReply* replytwee = manager.get(networkRequesttwee);

                loop.exec();
                if (reply->error() == QNetworkReply::NoError) {
                    // Read API response
                    QByteArray responseDatatwee = replytwee->readAll();
                    QJsonDocument jsonResponsetwee = QJsonDocument::fromJson(responseDatatwee);
                    QJsonObject jsonObjecttwee = jsonResponsetwee.object();

                    // Extract the "high" value from the JSON object
                    double highValuetwee = jsonObjecttwee["high"].toDouble();
                    std::cout << "High Value: " << highValuetwee << std::endl;

                    // Extract the stock name from the JSON object
                    QString stockNametwee = jsonObjecttwee["symbol"].toString();
                    std::cout << "Stock Name: " << stockNametwee.toStdString() << std::endl;

                    QJsonObject messageObjecttwee;
                    messageObjecttwee["stockName"] = stockNametwee;
                    messageObjecttwee["highValue"] = highValuetwee;

                    QJsonDocument doc(messageObjecttwee);
                    QByteArray jsonBytes = doc.toJson(QJsonDocument::Compact);
                    std::string jsonString = "thibe>quest!>" +jsonBytes.toStdString();

                    ventilator.send(zmq::buffer(jsonString), zmq::send_flags::dontwait);
                }

            }
        } else {
            std::cerr << "Failed to perform HTTP request: " << reply->errorString().toStdString() << std::endl;
        }

        reply->deleteLater();

    } catch (const zmq::error_t& ex) {
        std::cerr << "Caught an exception: " << ex.what() << std::endl;
    }

    return a.exec();
}