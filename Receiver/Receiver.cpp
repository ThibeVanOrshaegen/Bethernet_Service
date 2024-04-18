#include <iostream>
#include <string>
#include <zmq.hpp>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n)    Sleep(n)
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    try
    {
        zmq::context_t context(1);

        // Outgoing message go out through here
        zmq::socket_t ventilator(context, ZMQ_PUSH);
        ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");

        // Incoming messages come in here
        zmq::socket_t subscriber(context, ZMQ_SUB);
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, "Stock>quest?>", 13);

        zmq::message_t msg;
        subscriber.recv(&msg);

        QString StockCADL = "Stock>quest?>CADL";
        QString StockAAPL = "Stock>quest?>AAPL";
        QString StockCADLGROUPED = "Stock>quest?>CADLPrevious";
        QString StockAAPLGROUPED = "Stock>quest?>PreviousAAPL";


        QString receivedMessage = QString::fromStdString(std::string(static_cast<char*>(msg.data()), msg.size()));
        if (receivedMessage == "Stock>quest?>CADL")
        {
            // Send the appropriate message to the ventilator
            ventilator.send("Stock>quest?>CADL", 19);

            // Make the HTTP request
            QNetworkAccessManager networkManager;
            QNetworkRequest request(QUrl("https://api.polygon.io/v1/open-close/CADL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"));
            QNetworkReply *reply = networkManager.get(request);

            // Connect to the finished signal of the network reply
            QObject::connect(reply, &QNetworkReply::finished, [=, &ventilator]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray responseData = reply->readAll();
                    ventilator.send(responseData.constData(), responseData.size());
                } else {
                    std::cerr << "Network request failed: " << reply->errorString().toStdString() << std::endl;
                }
                reply->deleteLater();
            });
            return a.exec();
        }
        else if (receivedMessage == StockAAPL)
        {
            // Send the appropriate message to the ventilator
            ventilator.send("Stock>quest?>AAPL", 17);
            // Make the HTTP request
            QNetworkAccessManager networkManager;
            QNetworkRequest request(QUrl("https://api.polygon.io/v1/open-close/AAPL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"));
            QNetworkReply *reply = networkManager.get(request);

            // Connect to the finished signal of the network reply
            QObject::connect(reply, &QNetworkReply::finished, [=, &ventilator]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray responseData = reply->readAll();
                    ventilator.send(responseData.constData(), responseData.size());
                } else {
                    std::cerr << "Network request failed: " << reply->errorString().toStdString() << std::endl;
                }
                reply->deleteLater();
            });
            return a.exec();
        }
        else if (receivedMessage == "Stock>quest?>CADLPrevious")
        {
            // Send the appropriate message to the ventilator
            ventilator.send("Stock>quest?>CADLPrevious", 25);
            // Make the HTTP request
            QNetworkAccessManager networkManager;
            QNetworkRequest request(QUrl("https://api.polygon.io/v2/aggs/ticker/CADL/prev?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"));
            QNetworkReply *reply = networkManager.get(request);

            // Connect to the finished signal of the network reply
            QObject::connect(reply, &QNetworkReply::finished, [=, &ventilator]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray responseData = reply->readAll();
                    ventilator.send(responseData.constData(), responseData.size());
                } else {
                    std::cerr << "Network request failed: " << reply->errorString().toStdString() << std::endl;
                }
                reply->deleteLater();
            });
            return a.exec();
        }
        else if (receivedMessage == StockAAPLGROUPED)
        {
            // Send the appropriate message to the ventilator
            ventilator.send("Stock>quest?>PreviousAAPL", 25);
            // Make the HTTP request
            QNetworkAccessManager networkManager;
            QNetworkRequest request(QUrl("https://api.polygon.io/v2/aggs/ticker/AAPL/prev?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"));
            QNetworkReply *reply = networkManager.get(request);

            // Connect to the finished signal of the network reply
            QObject::connect(reply, &QNetworkReply::finished, [=, &ventilator]() {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray responseData = reply->readAll();
                    ventilator.send(responseData.constData(), responseData.size());
                } else {
                    std::cerr << "Network request failed: " << reply->errorString().toStdString() << std::endl;
                }
                reply->deleteLater();
            });
            return a.exec();
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}
