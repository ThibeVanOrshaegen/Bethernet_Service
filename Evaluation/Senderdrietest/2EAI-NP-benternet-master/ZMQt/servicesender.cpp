#include "ServiceSender.h"
#include "MainWindowManager.h"
#include <iostream>
#include <QApplication>

ServiceSender::ServiceSender()
    : context(1), subscriber(context, ZMQ_SUB), ventilator(context, ZMQ_PUSH) {
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "thibe<quest!>", 13);
    ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
    std::string fullMessage = "thibe<quest?>StockService";
    ventilator.send(zmq::buffer(fullMessage), zmq::send_flags::dontwait);
}

void ServiceSender::sendMessage(const std::string& message, zmq::socket_t& socket) {
    std::string fullMessage = "thibe<quest?>" + message;
    if (!sessionId.empty()) {
        fullMessage += "|" + sessionId;
    }
    zmq::message_t msg(fullMessage.size());
    memcpy(msg.data(), fullMessage.c_str(), fullMessage.size());
    socket.send(msg, zmq::send_flags::dontwait);
}

void ServiceSender::receiveMessage() {
    zmq::message_t msg;
    subscriber.recv(msg);
    std::string receivedMessage = std::string(static_cast<char*>(msg.data()), msg.size());
    processReceivedMessage(receivedMessage);
}

void ServiceSender::startReceiving() {
    while (true) {
        keepreceiveMessage();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ServiceSender::keepreceiveMessage() {
    zmq::message_t msgone;
    subscriber.recv(msgone);
    std::string receivedMessageone = std::string(static_cast<char*>(msgone.data()), msgone.size());
    processReceivedMessage(receivedMessageone);
}

void ServiceSender::processReceivedMessage(const std::string& receivedMessage) {
    size_t pos = receivedMessage.find("|");
    if (pos != std::string::npos && pos + 1 < receivedMessage.size()) {
        sessionId = receivedMessage.substr(pos + 1);
        MainWindowManager::CreateMainWindow(sessionId);
    }
}

