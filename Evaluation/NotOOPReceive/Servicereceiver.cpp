#include "servicereceiver.h"
#include <iostream>
#include <random>
#include <mutex>
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
#include <QApplication>
#include <qstring.h>

std::mutex sessionMutex;

ServiceReceiver::ServiceReceiver() {
    context = zmq::context_t(1);
    subscriber = zmq::socket_t(context, ZMQ_SUB);
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "thibe<quest?>", 13);
    ventilator = zmq::socket_t(context, ZMQ_PUSH);
    ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");

}

void ServiceReceiver::receiveMessage() {
    while (true) {
        zmq::message_t msg;
        if (subscriber.recv(msg, zmq::recv_flags::dontwait)) {
            std::string receivedMessage(static_cast<char*>(msg.data()), msg.size());
            sessionId = generateSessionId();
            std::cout << "receivedMessagetest: "<<receivedMessage << std::endl;

            if (receivedMessage == "thibe<quest?>StockService") {
                std::cout << "Accepted" << std::endl;
                //std::cout << "receivedMessage: " <<receivedMessage<< std::endl;
                std::string message = "thibe<quest!>Accepted";
//                std::cout << "Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n" << std::endl;

                sendMessage(message + "|" + sessionId + ">" +"Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- thibe<quest?>Shares Under 200|3439\n- thibe<quest?>ExchangeRate Differential AAPL|3439\n- thibe<quest?>Mastermind Go Go|3439\n\n" );
            } else {
                // Extract session ID and message content
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
                            std::string eersteinvoergoed= slechteinvoer.substr(0, posthree);
                            std::string tweedeinvoerslecht = slechteinvoer.substr(posthree + 1);

                            size_t postfour = tweedeinvoerslecht.find(" ");
                            if (postfour != std::string::npos && postfour + 1 < tweedeinvoerslecht.size()) {
                                std::string tweedeinvoergoed= tweedeinvoerslecht.substr(0, postfour);
                                std::string derdeinvoer = tweedeinvoerslecht.substr(postfour + 1);

                                std::cout << "eersteinvoer: "<<eersteinvoer << std::endl;
                                std::cout << "slechteinvoer: "<<slechteinvoer << "\n" <<std::endl;
                                std::cout << "eersteinvoerslecht: "<<eersteinvoergoed << std::endl;
                                std::cout << "tweedeinvoer: "<<tweedeinvoergoed << std::endl;
                                std::cout << "derdeinvoer: "<<derdeinvoer << std::endl;

                                std::cout << "Received message: " << content << " with session ID: " << receivedSessionId << std::endl;
                                if (eersteinvoergoed == "Shares" && tweedeinvoergoed == "Under") startServiceOne(receivedSessionId,derdeinvoer);
                                if (eersteinvoergoed == "ExchangeRate" && tweedeinvoergoed == "Differential" ) startServiceTwo(receivedSessionId,derdeinvoer);
                                if (eersteinvoergoed == "MasterMind" && tweedeinvoergoed == "Go") startServiceThree(receivedSessionId);
                            }
                        }
                    }
                }
            }
        }
    }
}

void ServiceReceiver::startServiceOne(const std::string& receivedSessionId, const std::string& derdeinvoer) {
    std::lock_guard<std::mutex> lock(sessionMutex);
    std::cout << "ServiceOneActive" << std::endl;
    std::cout << "ID: " << receivedSessionId << std::endl;
    std::cout << "Derdeinvoer: " << derdeinvoer << std::endl;

    double stockValue = std::stod(derdeinvoer);
    // URLs for the API requests
    std::vector<std::string> urls = {
        "https://api.polygon.io/v1/open-close/AAPL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS",
        "https://api.polygon.io/v1/open-close/KIMpL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS",
        "https://api.polygon.io/v1/open-close/CADL/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"
    };

    QNetworkAccessManager manager;
    QJsonObject combinedData;

    for (const auto& url : urls) {
        QNetworkRequest networkRequest(QUrl(QString::fromStdString(url)));
        QEventLoop loop;
        QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        QNetworkReply* reply = manager.get(networkRequest);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            double highValue = jsonObject["high"].toDouble();
            if (highValue < stockValue) {

                QString stockName = jsonObject["symbol"].toString();
                QJsonObject messageObject;

                messageObject["highValue"] = highValue;
                std::string testmessage = std::to_string(highValue);
                //std::cout << "HighValueNew: " << highValue << std::endl;

                messageObject["stockName"] = stockName;
                QString teststockname = stockName;
                //std::cout << "StocknameNew: " << stockName.toStdString() << std::endl;

                combinedData[stockName] = messageObject;

                QString messageToSend = stockName +" HighValueNew: " + QString::fromStdString(testmessage);
                if (!MessageToSendTest) {
                    MessageToSendTest = new QString;
                }
                *MessageToSendTest = messageToSend;
                std::cout << "MessagetoSend: " << messageToSend.toStdString() << std::endl;

                // Append the message to the combined string
                messagesCombined += messageToSend + "\n";
            }
        } else {
            std::cerr << "Failed to perform HTTP request: " << reply->errorString().toStdString() << std::endl;
        }
        reply->deleteLater();
    }
    if (MessageToSendTest) {
        std::cout << "" << messagesCombined.toStdString() << std::endl;
        std::string message = "thibe<quest!>(ServiceOneActive)" + messagesCombined.toStdString() +"|" + receivedSessionId;
        sendMessage(message);
        receiveMessage();
    }
}

void ServiceReceiver::startServiceTwo(const std::string& receivedSessionId, const std::string &derdeinvoer) {
    std::cout << "ServiceTwoActive" << std::endl;
    std::cout << "ID: " << receivedSessionId << std::endl;
    std::cout << "Derdeinvoer: " << derdeinvoer << std::endl;

    std::vector<std::string> urll = {
        "https://api.polygon.io/v1/open-close/" + derdeinvoer + "/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"
    };

    QNetworkAccessManager manager;
    for (const auto& urlreal : urll) {
    QNetworkRequest networkRequest(QUrl(QString::fromStdString(urlreal)));
    //QNetworkRequest networkRequest(QUrl("https://api.polygon.io/v1/open-close/"+ stockSymbol +"/2023-01-09?adjusted=true&apiKey=9R_AeMLkHBOPzgqDs2HiizXhCI4r67CS"));

    QEventLoop loop;
    QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QNetworkReply* reply = manager.get(networkRequest);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
    // Read API response
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonResponse.object();

    // Extract the "o" (OpeningPrice) value from the JSON object
    double openingPrice = jsonObject["open"].toDouble();
    std::cout << "OpeningPrice: " << openingPrice << std::endl;

    // Extract the "c" (ClosingPrice) value from the JSON object
    double closingPrice = jsonObject["close"].toDouble();
    std::cout << "ClosingPrice: " << closingPrice << std::endl;

    double Min = openingPrice  - closingPrice;
    double Delen = (Min /closingPrice)*100;
    std::cout << "Verschil: " << Delen <<"%"<< std::endl;

    // Send the OpeningPrice and ClosingPrice back to the sender
    std::string messageToSend = "OpeningPrice: " + std::to_string(openingPrice) + "\nClosingPrice: " + std::to_string(closingPrice) + "\nDifference: " + std::to_string(Delen) + " %";
    std::cout << "messageToSend: " << messageToSend <<"%"<< std::endl;

    std::string message = "thibe<quest!>(ServiceTwoActive)" + messageToSend +"|" + receivedSessionId;
    sendMessage(message);
   // ventilator.send(zmq::buffer("thibe>quest!>"+messageToSend), zmq::send_flags::dontwait);
    } else {
    std::cerr << "Failed to perform HTTP request: " << reply->errorString().toStdString() << std::endl;
    }
    reply->deleteLater();
    }
}

void ServiceReceiver::startServiceThree(const std::string& receivedSessionId) {

    std::cout << "ServiceThreeActive" << std::endl;
    std::cout << "ID: " << receivedSessionId << std::endl;

    std::string message = "thibe<quest!>(ServiceThreeActive)" + receivedSessionId +"|" + receivedSessionId +">"+ "\n\nWelcome To MasterMind\n\nTo win the Game you have to guess the correct colors on the correct place\nYou have 5 attempts to guess then the game is over!\n\nExample:\ncolorone colortwo colorthree colorfour\n\nGood Luck!!\nThis is an example: \n\nGuessOneService: thibe<quest?>blue yellow pink red|2567\n\n";
    sendMessage(message);
    while (true)
    {
    zmq::message_t GuessOne;
    subscriber.recv(GuessOne);
    std::string GuessOneService= std::string(static_cast<char*>(GuessOne.data()), GuessOne.size());

    size_t pos = GuessOneService.find("/");
    if (pos != std::string::npos && pos + 1 < GuessOneService.size()) {
    std::string content = GuessOneService.substr(0, pos);
    std::string id = GuessOneService.substr(pos + 1);
    std::string message = "thibe<quest!>/NOT_ACTIVE_OKE]" + receivedSessionId +"|" + receivedSessionId;
    std::cout << "Message receiver stopped send" << std::endl;
    sendMessage(message);
    receiveMessage();
    }

    std::cout << "GuessOneService: " << GuessOneService << std::endl;
    // GuessOneService: thibe<quest?>blue yellow pink red|2567

    size_t spacePosone = GuessOneService.find('>');
    if (spacePosone != std::string::npos) {
    std::string Sringone = GuessOneService.substr(0, spacePosone);
    std::string Stringtwo = GuessOneService.substr(spacePosone + 1);
//    std::cout << "Sringone: " << Sringone << std::endl;
//    std::cout << "Stringtwo: " << Stringtwo << std::endl;

    size_t spacePostwo= Stringtwo.find(' ');
    if (spacePostwo != std::string::npos) {
    std::string ColorOne = Stringtwo.substr(0, spacePostwo); //colorOne
    std::string StringFour = Stringtwo.substr(spacePostwo + 1);
//    std::cout << "StringFour: " << StringFour << std::endl;

    size_t spacePosthree = StringFour.find(' ');
    if (spacePosthree != std::string::npos) {
                std::string ColorTwo = StringFour.substr(0, spacePosthree); //colortwo
                std::string StringFive = StringFour.substr(spacePosthree + 1);
//                std::cout << "StringFive: " << StringFive << std::endl;

                size_t spacePostfour = StringFive.find(' ');
                if (spacePostfour != std::string::npos) {
                    std::string ColorThree = StringFive.substr(0, spacePostfour); //colorthree
                    std::string StringSix = StringFive.substr(spacePostfour +1);
//                    std::cout << "StringSix: " << StringSix << std::endl;

                    size_t spacePostfive = StringSix.find('|');
                    if (spacePostfive != std::string::npos) {
                        std::string ColorFour = StringSix.substr(0, spacePostfive); //colorfour
                        std::string StringSeven = StringSix.substr(spacePostfive +1);
//                        std::cout << "StringSeven: " << StringSeven << std::endl;

                        std::string ConcatenatedString = ColorOne + " " + ColorTwo + " " + ColorThree+ " " +ColorFour ;
                        std::cout << "ConcatenatedString: " << ConcatenatedString << std::endl;
                        std::cout << "ColorOne: " << ColorOne << std::endl;
                        std::cout << "ColorTwo: " << ColorTwo << std::endl;
                        std::cout << "ColorThree: " << ColorThree << std::endl;
                        std::cout << "ColorFour: " << ColorFour << std::endl;

//                        std::cout << "GameColorOne: " <<GameColorOne.toStdString() << std::endl;
//                        std::cout << "GameColorOne: " <<GameColorTwo.toStdString() << std::endl;
//                        std::cout << "GameColorOne: " <<GameColorThree.toStdString() << std::endl;
//                        std::cout << "GameColorOne: " <<GameColorFour.toStdString() << std::endl;


                        //bad if all wrong almost if correct color wrong place perfect correct place and color
                        //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                        // Firstcolor
                        //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                        if(ColorOne == GameColorOne.toStdString())
                        {
                            std::cout << "perfect"<< std::endl;
                            QString messageToSendcolorone = "perfect " + QString::fromStdString(ColorOne);
                            *MessageOne = messageToSendcolorone;
                        }
                        else if(ColorOne == GameColorTwo.toStdString() ||ColorOne == GameColorThree.toStdString() || ColorOne == GameColorFour.toStdString() )
                        {
                            std::cout << "almost"<< std::endl;
                            QString messageToSendcolorone = "almost " + QString::fromStdString(ColorOne);
                            *MessageOne = messageToSendcolorone;
                        }
                        else  if(ColorOne != GameColorOne.toStdString() && ColorOne != GameColorTwo.toStdString() &&ColorOne != GameColorThree.toStdString() && ColorOne != GameColorFour.toStdString() )
                        {
                            std::cout << "bad"<< std::endl;
                            QString messageToSendcolorone = "bad " + QString::fromStdString(ColorOne);
                            *MessageOne = messageToSendcolorone;
                        }
                        //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                        // Secondcolor
                        //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                        if(ColorTwo == GameColorTwo.toStdString())
                        {
                            std::cout << "perfect"<< std::endl;
                            QString messageToSendcolorTwo = "perfect " + QString::fromStdString(ColorTwo);
                            *MessageTwo = messageToSendcolorTwo;
                        }
                        else if(ColorTwo == GameColorOne.toStdString() ||ColorTwo == GameColorThree.toStdString() || ColorTwo == GameColorFour.toStdString() )
                        {
                            std::cout << "almost"<< std::endl;
                            QString messageToSendcolorTwo = "almost " + QString::fromStdString(ColorTwo);
                            *MessageTwo = messageToSendcolorTwo;
                        }
                        else  if(ColorTwo != GameColorOne.toStdString() && ColorTwo != GameColorTwo.toStdString() && ColorTwo != GameColorThree.toStdString() &&  ColorTwo != GameColorFour.toStdString() )
                        {
                            std::cout << "bad"<< std::endl;
                            QString messageToSendcolorTwo = "bad " + QString::fromStdString(ColorTwo);
                            *MessageTwo = messageToSendcolorTwo;
                        }
                        //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                        // Thirdcolor
                        //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                        if(ColorThree == GameColorThree.toStdString())
                        {
                            std::cout << "perfect"<< std::endl;
                            QString messageToSendcolorThree = "perfect " + QString::fromStdString(ColorThree);
                            *MessageThree = messageToSendcolorThree;
                        }
                        else if(ColorThree == GameColorOne.toStdString() ||ColorThree == GameColorTwo.toStdString() || ColorThree == GameColorFour.toStdString() )
                        {
                            std::cout << "almost"<< std::endl;
                            QString messageToSendcolorThree = "almost " + QString::fromStdString(ColorThree);
                            *MessageThree = messageToSendcolorThree;
                        }
                        else  if(ColorThree != GameColorOne.toStdString() && ColorThree != GameColorTwo.toStdString() && ColorThree != GameColorThree.toStdString() &&  ColorThree != GameColorFour.toStdString() )
                        {
                            std::cout << "bad"<< std::endl;
                            QString messageToSendcolorThree = "bad " + QString::fromStdString(ColorThree);
                            *MessageThree = messageToSendcolorThree;
                        }
                        //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                        // Thirdcolor
                        //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                        if(ColorFour == GameColorFour.toStdString())
                        {
                            std::cout << "perfect"<< std::endl;
                            QString messageToSendcolorFour = "perfect " + QString::fromStdString(ColorFour);
                            *MessageFour = messageToSendcolorFour;
                        }
                        else if(ColorFour == GameColorOne.toStdString() ||ColorFour == GameColorTwo.toStdString() || ColorFour == GameColorThree.toStdString() )
                        {
                            std::cout << "almost"<< std::endl;
                            QString messageToSendcolorFour = "almost " + QString::fromStdString(ColorFour);
                            *MessageFour = messageToSendcolorFour;
                        }
                        else  if(ColorFour != GameColorOne.toStdString() && ColorFour != GameColorTwo.toStdString() && ColorFour != GameColorThree.toStdString() &&  ColorFour != GameColorFour.toStdString() )
                        {
                            std::cout << "bad"<< std::endl;
                            QString messageToSendcolorFour = "bad " + QString::fromStdString(ColorFour);
                            *MessageFour = messageToSendcolorFour;
                        }
//                        if(ColorFour == GameColorOne.toStdString() && Secondcolor == GameColorTwo.toStdString() && Thirdcolor == GameColorThree.toStdString() &&Fourthcolor == GameColorFour.toStdString())
//                        {
//                            std::cout << "You win"<< std::endl;
//                            QString messageToSendcolorone = "win";
//                            *MessageOne = messageToSendcolorone;
//                        }
                        QString combinedMessage = "thibe<quest!>" + *MessageOne +" " + *MessageTwo +" " + *MessageThree +" " + *MessageFour +"|" + QString::fromStdString(receivedSessionId);
                        std::cout << "combinedMessageFirst: " << combinedMessage.toStdString() << std::endl;
                        std::cout << MessageOne->toStdString() << " " << MessageTwo->toStdString() << " " << MessageThree->toStdString() << " " << MessageFour->toStdString() << std::endl;
                        sendMessage(combinedMessage.toStdString());

                                   }
                            }
                     }
              }
       }
    }
}

void ServiceReceiver::sendMessage(const std::string& message) {
    zmq::message_t msg(message.size());
    memcpy(msg.data(), message.c_str(), message.size());
    ventilator.send(zmq::buffer(msg.to_string()), zmq::send_flags::dontwait);
}

std::string ServiceReceiver::generateSessionId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1000, 9999);
    return std::to_string(dis(gen));
}