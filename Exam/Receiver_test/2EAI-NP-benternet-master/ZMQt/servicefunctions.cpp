#include "ServiceFunctions.h"
#include "ServiceReceiver.h" // Include ServiceReceiver.h here

// Your code for ServiceFunctions.cpp goes here

#include <iostream>

void yourFunction(const std::string& receivedMessage) {
    // Create an instance of ServiceReceiver
    ServiceReceiver receiver;
    // Call receiveMessage() through the instance
    std::cout << "Test twee: " <<receivedMessage<< std::endl;
    receiver.receiveMessage(receivedMessage);
}
