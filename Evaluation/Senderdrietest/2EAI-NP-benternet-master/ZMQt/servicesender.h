#ifndef SERVICESENDER_H
#define SERVICESENDER_H

#include <zmq.hpp>
#include <string>
#include <thread>

class ServiceSender {
public:
    ServiceSender();
    void sendMessage(const std::string& message, zmq::socket_t& socket);
    void receiveMessage();
    void startReceiving();
    void keepreceiveMessage();
    int AantalGokken = 4;
    int CountFunction = 1;
    zmq::socket_t subscriber;
    zmq::socket_t ventilator;

private:
    zmq::context_t context;
    std::string sessionId;
    std::thread receiveThread;



    void processReceivedMessage(const std::string& receivedMessage);
};

#endif // SERVICESENDER_H
