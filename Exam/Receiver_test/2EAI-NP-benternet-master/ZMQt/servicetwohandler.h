#ifndef SERVICETWOHANDLER_H
#define SERVICETWOHANDLER_H

#include <string>
#include <zmq.hpp>
#include <QNetworkAccessManager>
#include <QObject>
#include <QEventLoop>

class ServiceTwoHandler {
public:

    ServiceTwoHandler(zmq::context_t& context);
    void startServiceTwo(const std::string& receivedSessionId, const std::string& derdeinvoer);

private:
    zmq::socket_t ventilator;
    QNetworkAccessManager manager;
    zmq::context_t context;
    zmq::socket_t subscriber;
    std::string sessionId; // Store session ID

    void sendMessage(const std::string& message);
};

#endif // SERVICETWOHANDLER_H
