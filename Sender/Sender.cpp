#include <iostream>
#include <string>
#include <zmq.hpp>

int main() {
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
        std::string message = "thibe>quest?>tijdd";
        ventilator.send(zmq::buffer(message), zmq::send_flags::dontwait);

        // Receive response
        while (true) {
        zmq::message_t msg;
        subscriber.recv(msg);
        std::cout << "Received: " << std::string(static_cast<char*>(msg.data()), msg.size()) << std::endl;
}
    } catch (const zmq::error_t& ex) {
        std::cerr << "Caught an exception: " << ex.what() << std::endl;
    }

    return 0;
}