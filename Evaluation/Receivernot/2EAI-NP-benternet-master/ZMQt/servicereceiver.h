#ifndef SERVICERECEIVER_H
#define SERVICERECEIVER_H

#include <zmq.hpp>
#include <mutex>
#include <QObject>
#include <string>

class ServiceReceiver {
public:
    ServiceReceiver();
    void startServiceOne(const std::string& receivedSessionId, const std::string& derdeinvoer);
    void startServiceTwo(const std::string& receivedSessionId, const std::string& derdeinvoer);
    void startServiceThree(const std::string& receivedSessionId);
    void receiveMessage(); // Declaration of receiveMessage function
    void sendMessage(const std::string& message);
    void randomizeColors();
    std::string getGameWinString();


private:
    zmq::context_t context;
    zmq::socket_t subscriber;
    zmq::socket_t ventilator;
    std::string sessionId; // Store session ID

    QString *MessageToSendTest = nullptr;
    QString *MessageToSendTestTwee = nullptr;
    QString messagesCombined;

    QString *MessageOne = new QString;
    QString *MessageTwo = new QString;
    QString *MessageThree = new QString;
    QString *MessageFour = new QString;

    QString GameColorOne;
    QString GameColorTwo;
    QString GameColorThree;
    QString GameColorFour;

    std::string generateSessionId();
};

extern std::mutex sessionMutex; // Declare sessionMutex as extern

#endif // SERVICERECEIVER_H
