#include "servicethreehandler.h"
#include <zmq.hpp>
#include <iostream>
#include <exception>
#include "ServiceReceiver.h"
#include "ServiceFunctions.h"


ServiceThreeHandler::ServiceThreeHandler(zmq::context_t& context)
    : ventilator(context, ZMQ_PUSH), context(1), subscriber(context, ZMQ_SUB) {
    try {
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        std::string subscription = "thibe<quest?" + sessionId;
        subscriber.setsockopt(ZMQ_SUBSCRIBE, subscription.c_str(), subscription.size());
        //subscriber.setsockopt(ZMQ_SUBSCRIBE, "thibe<quest?>", 13);
        ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
    } catch (const zmq::error_t& e) {
        std::cerr << "ZMQ Error in ServiceThreeHandler constructor: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Standard exception in ServiceThreeHandler constructor: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error in ServiceThreeHandler constructor" << std::endl;
    }
}

void ServiceThreeHandler::startServiceThree(const std::string& receivedSessionId) {
    std::cout << "ServiceThreeActive" << std::endl;
    std::cout << "ID: " << receivedSessionId << std::endl;
    int attempts = 0;
    bool guessedCorrectly = false;

    while (attempts < 5 && !guessedCorrectly) {
        try {
            std::string message = "thibe<quest!>(ServiceThreeActive)" + receivedSessionId + "|" + receivedSessionId + "\nWelcome To MasterMind\n\nTo win the Game you have to guess the correct colors on the correct place\nYou have 5 attempts to guess then the game is over!\n\nExample:\ncolorone colortwo colorthree colorfour\n\nGood Luck!!\n\n";
            sendMessage(message);
            bool guessOneServiceReset = false; // Flag to ensure it only happens once
            while (true) {
                try {
                    zmq::message_t GuessOne;
                    subscriber.recv(GuessOne, zmq::recv_flags::none);
                    std::string GuessOneService = std::string(static_cast<char*>(GuessOne.data()), GuessOne.size());
                    std::cout << "Test een" << std::endl;

                    if (!guessOneServiceReset) {
                        GuessOneService = "nul";
                        guessOneServiceReset = true; // Set the flag to true after first reset
                    }

                    //                // Process the received message
                    //                size_t posone = GuessOneService.find("|");
                    //                if (posone != std::string::npos && posone + 1 < GuessOneService.size()) {
                    //                    std::string content = GuessOneService.substr(0, posone);
                    //                    std::string id = GuessOneService.substr(posone + 1);
                    //                    std::string message = "thibe<quest!>/New service]" + receivedSessionId + "|" + receivedSessionId;
                    //                    std::cout << "Message receiver stopped because new service not a color" << std::endl;

                    //                    sendMessage(message);
                    //                }

                    size_t pos = GuessOneService.find("/");
                    if (pos != std::string::npos && pos + 1 < GuessOneService.size()) {
                        std::string content = GuessOneService.substr(0, pos);
                        std::string id = GuessOneService.substr(pos + 1);
                        std::string message = "thibe<quest!>/NOT_ACTIVE_OKE]" + receivedSessionId + "|" + receivedSessionId;
                        std::cout << "Message receiver stopped send" << std::endl;
                        sendMessage(message);
                        receiveMessage();
                    }

                    std::cout << "GuessOneService: " << GuessOneService << std::endl;
                    if(GuessOneService == "thibe<quest?>StockService" )
                    {
                        std::string receivedMessage = "thibe<quest?>StockService"; // Define your message here
                        yourFunction(receivedMessage);
                    }

                    // Process the game logic
                    size_t spacePosone = GuessOneService.find('>');
                    if (spacePosone != std::string::npos) {
                        std::string Sringone = GuessOneService.substr(0, spacePosone);
                        std::string Stringtwo = GuessOneService.substr(spacePosone + 1);

                        size_t spacePostwo = Stringtwo.find(' ');
                        if (spacePostwo != std::string::npos) {
                            std::string ColorOne = Stringtwo.substr(0, spacePostwo);
                            std::string StringFour = Stringtwo.substr(spacePostwo + 1);

                            size_t spacePosthree = StringFour.find(' ');
                            if (spacePosthree != std::string::npos) {
                                std::string ColorTwo = StringFour.substr(0, spacePosthree);
                                std::string StringFive = StringFour.substr(spacePosthree + 1);

                                size_t spacePostfour = StringFive.find(' ');
                                if (spacePostfour != std::string::npos) {
                                    std::string ColorThree = StringFive.substr(0, spacePostfour);
                                    std::string StringSix = StringFive.substr(spacePostfour + 1);

                                    size_t spacePostfive = StringSix.find('|');
                                    if (spacePostfive != std::string::npos) {
                                        std::string ColorFour = StringSix.substr(0, spacePostfive);
                                        std::string StringSeven = StringSix.substr(spacePostfive + 1);

                                        std::string ConcatenatedString = ColorOne + " " + ColorTwo + " " + ColorThree + " " + ColorFour;
                                        std::cout << "ConcatenatedString: " << ConcatenatedString << std::endl;
                                        std::cout << "ColorOne: " << ColorOne << std::endl;
                                        std::cout << "ColorTwo: " << ColorTwo << std::endl;
                                        std::cout << "ColorThree: " << ColorThree << std::endl;
                                        std::cout << "ColorFour: " << ColorFour << std::endl;

                                        //-------------------------------------------------------------------------------------------------------------------
                                        // Firstcolor
                                        //-------------------------------------------------------------------------------------------------------------------
                                        if (ColorOne == GameColorOne.toStdString()) {
                                            std::cout << "perfect" << std::endl;
                                            QString messageToSendcolorone = "perfect " + QString::fromStdString(ColorOne);
                                            *MessageOne = messageToSendcolorone;
                                        } else if (ColorOne == GameColorTwo.toStdString() || ColorOne == GameColorThree.toStdString() || ColorOne == GameColorFour.toStdString()) {
                                            std::cout << "almost" << std::endl;
                                            QString messageToSendcolorone = "almost " + QString::fromStdString(ColorOne);
                                            *MessageOne = messageToSendcolorone;
                                        } else {
                                            std::cout << "bad" << std::endl;
                                            QString messageToSendcolorone = "bad " + QString::fromStdString(ColorOne);
                                            *MessageOne = messageToSendcolorone;
                                        }

                                        //-------------------------------------------------------------------------------------------------------------------
                                        // Secondcolor
                                        //-------------------------------------------------------------------------------------------------------------------
                                        if (ColorTwo == GameColorTwo.toStdString()) {
                                            std::cout << "perfect" << std::endl;
                                            QString messageToSendcolorTwo = "perfect " + QString::fromStdString(ColorTwo);
                                            *MessageTwo = messageToSendcolorTwo;
                                        } else if (ColorTwo == GameColorOne.toStdString() || ColorTwo == GameColorThree.toStdString() || ColorTwo == GameColorFour.toStdString()) {
                                            std::cout << "almost" << std::endl;
                                            QString messageToSendcolorTwo = "almost " + QString::fromStdString(ColorTwo);
                                            *MessageTwo = messageToSendcolorTwo;
                                        } else {
                                            std::cout << "bad" << std::endl;
                                            QString messageToSendcolorTwo = "bad " + QString::fromStdString(ColorTwo);
                                            *MessageTwo = messageToSendcolorTwo;
                                        }

                                        //-------------------------------------------------------------------------------------------------------------------
                                        // Thirdcolor
                                        //-------------------------------------------------------------------------------------------------------------------
                                        if (ColorThree == GameColorThree.toStdString()) {
                                            std::cout << "perfect" << std::endl;
                                            QString messageToSendcolorThree = "perfect " + QString::fromStdString(ColorThree);
                                            *MessageThree = messageToSendcolorThree;
                                        } else if (ColorThree == GameColorOne.toStdString() || ColorThree == GameColorTwo.toStdString() || ColorThree == GameColorFour.toStdString()) {
                                            std::cout << "almost" << std::endl;
                                            QString messageToSendcolorThree = "almost " + QString::fromStdString(ColorThree);
                                            *MessageThree = messageToSendcolorThree;
                                        } else {
                                            std::cout << "bad" << std::endl;
                                            QString messageToSendcolorThree = "bad " + QString::fromStdString(ColorThree);
                                            *MessageThree = messageToSendcolorThree;
                                        }

                                        //-------------------------------------------------------------------------------------------------------------------
                                        // Fourthcolor
                                        //-------------------------------------------------------------------------------------------------------------------
                                        if (ColorFour == GameColorFour.toStdString()) {
                                            std::cout << "perfect" << std::endl;
                                            QString messageToSendcolorFour = "perfect " + QString::fromStdString(ColorFour);
                                            *MessageFour = messageToSendcolorFour;
                                        } else if (ColorFour == GameColorOne.toStdString() || ColorFour == GameColorTwo.toStdString() || ColorFour == GameColorThree.toStdString()) {
                                            std::cout << "almost" << std::endl;
                                            QString messageToSendcolorFour = "almost " + QString::fromStdString(ColorFour);
                                            *MessageFour = messageToSendcolorFour;
                                        } else {
                                            std::cout << "bad" << std::endl;
                                            QString messageToSendcolorFour = "bad " + QString::fromStdString(ColorFour);
                                            *MessageFour = messageToSendcolorFour;
                                        }

                                        QString combinedMessage = "thibe<quest!>" + *MessageOne + " " + *MessageTwo + " " + *MessageThree + " " + *MessageFour + "|" + QString::fromStdString(receivedSessionId);
                                        std::cout << "combinedMessageFirst: " << combinedMessage.toStdString() << std::endl;
                                        std::cout << MessageOne->toStdString() << " " << MessageTwo->toStdString() << " " << MessageThree->toStdString() << " " << MessageFour->toStdString() << std::endl;
                                        sendMessage(combinedMessage.toStdString());
                                    }
                                }
                            }
                        }
                    }
                } catch (const zmq::error_t& e) {
                    std::cerr << "ZMQ Error during message processing: " << e.what() << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Standard exception during message processing: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "Unknown error during message processing" << std::endl;
                }
            }
            if (guessedCorrectly) {
                std::cout << "Congratulations! You won!" << std::endl;
            } else {
                std::cout << "Sorry, you lost. Better luck next time!" << std::endl;
            }
        } catch (const zmq::error_t& e) {
            std::cerr << "ZMQ Error in startServiceThree: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Standard exception in startServiceThree: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error in startServiceThree" << std::endl;
        }
    }
}
    void ServiceThreeHandler::sendMessage(const std::string& message) {
        try {
            zmq::message_t msg(message.size());
            memcpy(msg.data(), message.c_str(), message.size());

            std::cout << "Message send: " << message << std::endl;
            ventilator.send(zmq::buffer(message), zmq::send_flags::dontwait);
            std::cout << "Correct: " << message << std::endl;
        } catch (const zmq::error_t& e) {
            std::cerr << "ZMQ Error in sendMessage: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Standard exception in sendMessage: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error in sendMessage" << std::endl;
        }
    }

    void ServiceThreeHandler::receiveMessage() {
        try {
            while (true) {
                try {
                    zmq::message_t msg;
                    if (subscriber.recv(msg, zmq::recv_flags::dontwait)) {
                        std::string receivedMessage(static_cast<char*>(msg.data()), msg.size());
                        std::cout << "receivedMessagetest: " << receivedMessage << std::endl;

                        if (receivedMessage == "thibe<quest?>StockService") {
                            std::cout << "Accepted" << std::endl;
                            std::string message = "thibe<quest!>Accepted";
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
                                            if (eersteinvoergoed == "MasterMind" && tweedeinvoergoed == "Go") {
                                                startServiceThree(receivedSessionId);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                } catch (const zmq::error_t& e) {
                    std::cerr << "ZMQ Error during message reception: " << e.what() << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Standard exception during message reception: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "Unknown error during message reception" << std::endl;
                }
            }
        } catch (const zmq::error_t& e) {
            std::cerr << "ZMQ Error in receiveMessage: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Standard exception in receiveMessage: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error in receiveMessage" << std::endl;
        }
    }

    //thibe<quest?>StockService
    // Received (26) : thibe<quest!>Accepted|3769
    //thibe<quest?>MasterMind Go Go|7652
    //Message send !
    //Received (34) : thibe<quest?>MasterMind Go Go|5034
    //Received (42) : thibe<quest!>(ServiceThreeActive)3769|3769
    //Received (35) : thibe<quest!>/New service]3769|3769
    //thibe<quest?>blue yellow pink red|7652
    //Message send !
    //Received (38) : thibe<quest?>blue yellow pink red|8910
    //Received (64) : thibe<quest!>perfect blue almost yellow bad pink almost red|3769
    //thibe<quest?>red yellow blue red|3769
    //thibe<quest?>red yellow blue red|3769
    //Message send !
    //Received (37) : thibe<quest?>red yellow blue red|3769
    //Received (65) : thibe<quest!>almost red almost yellow almost blue almost red|3769
    //thibe<quest?>StockService
    //Message send !
    //Received (25) : thibe<quest?>StockService
    //Received (35) : thibe<quest!>/New service]3769|3769
