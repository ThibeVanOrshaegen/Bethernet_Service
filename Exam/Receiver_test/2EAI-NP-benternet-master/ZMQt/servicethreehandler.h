#ifndef SERVICETHREEHANDLER_H
#define SERVICETHREEHANDLER_H

#include <string>
#include <zmq.hpp>
#include <mutex>
#include <QObject>
#include <string>
#include "PlayerManager.h"

class ServiceThreeHandler {
public:

    ServiceThreeHandler(zmq::context_t& context);
    void startServiceThree(const std::string& receivedSessionId);
    void receiveMessage();
   // Servicereceiver ReceiverService;

private:
    zmq::socket_t ventilator;
    void sendMessage(const std::string& message);

    zmq::context_t context;
    zmq::socket_t subscriber;
    std::string sessionId; // Store session ID

    QString *MessageToSendTest = nullptr;
    QString *MessageToSendTestTwee = nullptr;
    QString messagesCombined;

    QString *MessageOne = new QString;
    QString *MessageTwo = new QString;
    QString *MessageThree = new QString;
    QString *MessageFour = new QString;

    QString GameColorOne = "blue";
    QString GameColorTwo = "red";
    QString GameColorThree = "yellow";
    QString GameColorFour = "black";

    std::string generateSessionId();

    void processReceivedMessage(const std::string& receivedMessage, const std::string& receivedSessionId);

};

#endif // SERVICETHREEHANDLER_H
