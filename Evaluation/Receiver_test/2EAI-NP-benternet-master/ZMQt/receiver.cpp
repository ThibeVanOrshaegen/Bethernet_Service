#include "Receiver.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <iostream>
Receiver::Receiver(QObject *parent) : QObject(parent), context(1), ventilator(context, ZMQ_PUSH)
{
    // Connect to ZeroMQ ventilator
    ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");

    // Connect to ZeroMQ subscriber
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "Stock>quest?>", 13);

    zmq::message_t msg;
    subscriber.recv(&msg);

    QString receivedMessage = QString::fromStdString(std::string(static_cast<char *>(msg.data()), msg.size()));
    processRequest(receivedMessage);
}

Receiver::~Receiver()
{
}

void Receiver::processRequest(const QString &request)
{
    QNetworkAccessManager networkManager;
    QNetworkRequest networkRequest;
    if (request == "Stock>quest?>CADL")
    {
        networkRequest.setUrl(QUrl("https://api.polygon.io/v1/open-close/CADL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"));
    }
    else if (request == "Stock>quest?>AAPL")
    {
        networkRequest.setUrl(QUrl("https://api.polygon.io/v1/open-close/AAPL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"));
    }
    // Add more conditions for other requests if needed

    QNetworkReply *reply = networkManager.get(networkRequest);

    // Connect to the finished signal of the network reply
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray responseData = reply->readAll();
            emit requestProcessed(responseData);
        }
        else
        {
            std::cerr << "Network request failed: " << reply->errorString().toStdString() << std::endl;
        }
        reply->deleteLater();
    });
}
