#include <iostream>
#include <string>
#include <zmq.hpp>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QEventLoop>
#include <QLabel>
#include <qstring.h>
#include <thread> // Add threading support
#include <vector>
#include <mutex> // Add mutex for synchronization
#include <atomic> // Add atomic variable for session ID

#include "ServiceFunctions.h"
#include "qapplication.h"
#include "servicereceiver.h"

constexpr int NUM_THREADS = 1; // Number of threads for handling clients

std::atomic<int> sessionCounter(1000); // Atomic variable for generating session ID

int generateSessionId() {
    return ++sessionCounter;
}

void clientHandler() {
    try {
        ServiceReceiver servicereceiver;
        std::string receivedMessage = "message_to_pass"; // Define your message here
        yourFunction(receivedMessage); // Pass the received message to yourFunction
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in clientHandler: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught in clientHandler." << std::endl;
    }
}


int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);

        // Create multiple ServiceReceiver instances, each in its own thread
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREADS; ++i) {
            try {
                threads.emplace_back(clientHandler);
            } catch (const std::exception& e) {
                std::cerr << "Exception caught while creating thread: " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "Unknown exception caught while creating thread." << std::endl;
            }
        }

        // Start heartbeat thread
        std::thread heartbeatThread([](){
            while (true) {
                try {
                    // Send heartbeat message
                    // You can adjust the sleep duration as needed
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                    // Get current time
                    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

                    std::cout << "Heartbeat sent at: " << std::ctime(&now) << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Exception caught in heartbeat thread: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "Unknown exception caught in heartbeat thread." << std::endl;
                }
            }
        });

        // Join all threads
        for (auto& thread : threads) {
            try {
                thread.join();
            } catch (const std::exception& e) {
                std::cerr << "Exception caught while joining thread: " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "Unknown exception caught while joining thread." << std::endl;
            }
        }

        // Join heartbeat thread
        heartbeatThread.join();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in main: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception caught in main." << std::endl;
        return 1;
    }
}
