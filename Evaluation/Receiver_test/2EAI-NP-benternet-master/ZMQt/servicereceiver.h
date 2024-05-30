#ifndef SERVICERECEIVER_H
#define SERVICERECEIVER_H

#include <zmq.hpp>
#include <mutex>
#include <QObject>
#include <string>
#include "serviceonehandler.h"
#include "servicetwohandler.h"
#include "servicethreehandler.h"
#include <zmq.hpp>


class ServiceReceiver {
public:
    ServiceReceiver();
    void receiveMessage(const std::string& receivedMessage); // Updated function signature
    void sendMessage(const std::string& message);
    void startHeartbeat();

private:
    zmq::context_t context;
    zmq::socket_t subscriber;
    zmq::socket_t ventilator;
    std::string sessionId;

    std::string generateSessionId();
    std::string getCurrentTimestamp();

    ServiceOneHandler serviceOneHandler;
    ServiceTwoHandler serviceTwoHandler;
    ServiceThreeHandler serviceThreeHandler;

};

extern std::mutex sessionMutex; // Declare sessionMutex as extern

#endif // SERVICERECEIVER_H
