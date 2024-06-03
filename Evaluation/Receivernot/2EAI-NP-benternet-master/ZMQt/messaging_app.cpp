#include "messaging_app.h"
#include <QApplication>
#include <QMessageBox>
#include <zmq.hpp>

void runMessagingApp() {
    try {
        zmq::context_t context(1);

        // Outgoing messages go out through here
        zmq::socket_t ventilator(context, ZMQ_PUSH);
        ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");

        // Incoming messages come in here
        zmq::socket_t subscriber(context, ZMQ_SUB);
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, "thibe>quest!>", 13); // Subscribe to all messages

        // Send a message
        std::string message = "thibe>quest?>StockService";
        ventilator.send(zmq::buffer(message), zmq::send_flags::dontwait);

        QApplication app(argc, argv); // Create QApplication instance

        // Setup a QMessageBox
        QMessageBox msgBox;
        msgBox.setWindowTitle("Message from Server");

        int duration = 10; // in seconds
        time_t startTime = time(nullptr);
        while (difftime(time(nullptr), startTime) < duration) {
            zmq::message_t msg;
            if (subscriber.recv(msg, zmq::recv_flags::dontwait)) {
                std::string receivedMessage = std::string(static_cast<char*>(msg.data()), msg.size());
                msgBox.setText(QString::fromStdString(receivedMessage));
                msgBox.exec(); // Display the QMessageBox
            }
            app.processEvents(); // Process any pending GUI events
        }

    } catch (const zmq::error_t& ex) {
        std::cerr << "Caught an exception: " << ex.what() << std::endl;
    }
}
