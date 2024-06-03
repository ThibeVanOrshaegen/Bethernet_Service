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

#include "qapplication.h"
#include "servicereceiver.h"

constexpr int NUM_THREADS = 1; // Number of threads for handling clients

std::atomic<int> sessionCounter(1000); // Atomic variable for generating session ID

int generateSessionId() {
    return ++sessionCounter;
}

void clientHandler() {
    ServiceReceiver servicereceiver;
    servicereceiver.receiveMessage();

    servicereceiver.randomizeColors();
    std::string GamewinString = servicereceiver.getGameWinString();
    std::cout << GamewinString << std::endl;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // Create multiple ServiceReceiver instances, each in its own thread
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(clientHandler);
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
