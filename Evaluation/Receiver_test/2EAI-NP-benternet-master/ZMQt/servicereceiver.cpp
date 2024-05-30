 #include "servicereceiver.h"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <zmq.hpp>
#include "ServiceFunctions.h"

std::mutex sessionMutex;

ServiceReceiver::ServiceReceiver()
    : context(1), subscriber(context, ZMQ_SUB), ventilator(context, ZMQ_PUSH),
    serviceOneHandler(context), serviceTwoHandler(context), serviceThreeHandler(context) {
    try {
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, "thibe<quest?>", 13);
        ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
    } catch (const zmq::error_t& e) {
        std::cerr << "ZMQ error during initialization: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception during initialization: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error during initialization" << std::endl;
    }
}

void ServiceReceiver::receiveMessage(const std::string& receivedMessage){
    while (true) {
        zmq::message_t msg;
        try {

            std::lock_guard<std::mutex> lock(sessionMutex);
            if (subscriber.recv(msg, zmq::recv_flags::dontwait)) {
                std::string receivedMessage(static_cast<char*>(msg.data()), msg.size());
                sessionId = generateSessionId();
                std::cout << "receivedMessagetest: " << receivedMessage << std::endl;

                if (receivedMessage == "thibe<quest?>StockService") {
                    std::cout << "Accepted" << std::endl;
                    std::string message = "thibe<quest!>Accepted\nChoose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n";
                    sendMessage(message + "|" + sessionId);
                } else {
                    size_t pos = receivedMessage.find("|");
                    if (pos != std::string::npos && pos + 1 < receivedMessage.size()) {
                        std::string content = receivedMessage.substr(0, pos);
                        std::string receivedSessionId = receivedMessage.substr(pos + 1);

                        size_t postwo = content.find(">");
                        if (postwo != std::string::npos && postwo + 1 < content.size()) {
                            std::string eersteinvoer = content.substr(0, postwo);
                            std::string slechteinvoer = content.substr(postwo + 1);

                            size_t posthree = slechteinvoer.find(" ");
                            if (posthree != std::string::npos && posthree + 1 < slechteinvoer.size()) {
                                std::string eersteinvoergoed = slechteinvoer.substr(0, posthree);
                                std::string tweedeinvoerslecht = slechteinvoer.substr(posthree + 1);

                                size_t postfour = tweedeinvoerslecht.find(" ");
                                if (postfour != std::string::npos && postfour + 1 < tweedeinvoerslecht.size()) {
                                    std::string tweedeinvoergoed = tweedeinvoerslecht.substr(0, postfour);
                                    std::string derdeinvoer = tweedeinvoerslecht.substr(postfour + 1);

                                    std::cout << "Received message: " << content << " with session ID: " << receivedSessionId << std::endl;
                                    if (eersteinvoergoed == "Shares" && tweedeinvoergoed == "Under") serviceOneHandler.startServiceOne(receivedSessionId, derdeinvoer);
                                    if (eersteinvoergoed == "ExchangeRate" && tweedeinvoergoed == "Differential") serviceTwoHandler.startServiceTwo(receivedSessionId, derdeinvoer);
                                    if (eersteinvoergoed == "MasterMind" && tweedeinvoergoed == "Go" && derdeinvoer == "Go") serviceThreeHandler.startServiceThree(receivedSessionId);
                                }
                            }
                        }
                    }
                }
            }
        } catch (const zmq::error_t& e) {
            std::cerr << "ZMQ error while receiving message: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Exception while receiving message: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error while receiving message" << std::endl;
        }
    }
}


void ServiceReceiver::sendMessage(const std::string& message) {
    try {
        zmq::message_t msg(message.size());
        memcpy(msg.data(), message.c_str(), message.size());
        std::cout << "Message sent: " << message << std::endl;
        ventilator.send(zmq::buffer(message), zmq::send_flags::dontwait);
    } catch (const zmq::error_t& e) {
        std::cerr << "ZMQ error while sending message: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception while sending message: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error while sending message" << std::endl;
    }
}

std::string ServiceReceiver::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* now_tm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void ServiceReceiver::startHeartbeat() {
    while (true) {
        try {
            std::string heartbeatMessage = "heartbeat|" + getCurrentTimestamp();
            sendMessage(heartbeatMessage);
            std::this_thread::sleep_for(std::chrono::seconds(10));
        } catch (const std::exception& e) {
            std::cerr << "Exception during heartbeat: " << e.what() << std::endl;
        }
    }
}

std::string ServiceReceiver::generateSessionId() {
    try {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(1000, 9999);
        return std::to_string(dis(gen));
    } catch (const std::exception& e) {
        std::cerr << "Exception while generating session ID: " << e.what() << std::endl;
        return "0000"; // Default session ID in case of error
    } catch (...) {
        std::cerr << "Unknown error while generating session ID" << std::endl;
        return "0000"; // Default session ID in case of error
    }
}

//thibe<quest?>StockService|8012
//thibe<quest?>ExchangeRate Differential AAPL|4475
//thibe<quest?>MasterMind Go Go|1340
//thibe<quest?>blue yellow pink red|1340
