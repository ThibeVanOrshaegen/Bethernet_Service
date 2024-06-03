#ifndef SERVICEONEHANDLER_H
#define SERVICEONEHANDLER_H

#include <zmq.hpp>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <mutex>

class ServiceOneHandler {
public:
    ServiceOneHandler(zmq::context_t& context);
    void startServiceOne(const std::string& receivedSessionId, const std::string& derdeinvoer,const std::string& datetwee);
    void sendMessage(const std::string& message);
    void processResponse(const QJsonObject& jsonObject, QString& combinedMessage, double stockValue);

private:
    zmq::socket_t ventilator;
    QNetworkAccessManager manager;
    std::mutex sessionMutex;
    zmq::context_t context;
    zmq::socket_t subscriber;
    std::string sessionId; // Store session ID
};

#endif // SERVICEONEHANDLER_H
