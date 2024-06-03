#include "zeromqreceiver.h"
#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

ZeroMQReceiver::ZeroMQReceiver(QObject *parent) : QObject(parent), context(1), ventilator(context, ZMQ_PUSH), subscriber(context, ZMQ_SUB)
{
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "Stock>quest?>", 13);
    connect(&subscriber, &zmq::socket_t::readyRead, this, &ZeroMQReceiver::processMessage);
}

ZeroMQReceiver::~ZeroMQReceiver()
{
    // No need to explicitly clean up sockets or context, they will be cleaned up automatically.
}

void ZeroMQReceiver::processMessage()
{
    zmq::message_t msg;
    subscriber.recv(&msg);

    QString receivedMessage = QString::fromStdString(std::string(static_cast<char *>(msg.data()), msg.size()));
    // Process received message and perform appropriate actions
    if (receivedMessage == "Stock>quest?>CADL")
    {
        // Your code for CADL message handling
    }
    else if (receivedMessage == "Stock>quest?>AAPL")
    {
        // Your code for AAPL message handling
    }
    // Handle other messages similarly
}
