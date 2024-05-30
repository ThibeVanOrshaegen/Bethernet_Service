#include <QApplication>
#include "ServiceSender.h"
#include "MainWindowManager.h"

#include <thread>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // Create the QApplication instance first
    ServiceSender sender;
    sender.receiveMessage();
    MainWindowManager::CreateMainWindow("some_session_id");

    // Start receiving thread
    std::thread receiveThread(&ServiceSender::startReceiving, &sender); // Pass the object along with the member function
    return app.exec();
}
