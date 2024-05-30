#include "servicesender.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>
#include <random>
#include <string>
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
#include <QApplication>
#include <qstring.h>
#include <QString>

ServiceSender::ServiceSender() {
    context = zmq::context_t(1); // Create the context
    // Initialize the subscriber socket
    subscriber = zmq::socket_t(context, ZMQ_SUB);
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "thibe<quest!>", 13);
    // Initialize the ventilator socket
    ventilator = zmq::socket_t(context, ZMQ_PUSH);
    ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
    // Send initial message
    std::string fullMessage = "thibe<quest?>StockService";
    ventilator.send(zmq::buffer(fullMessage), zmq::send_flags::dontwait);

}

void ServiceSender::sendMessage(const std::string& message, zmq::socket_t& socket) {
    // Send message along with session ID if provided
    std::string fullMessage = "thibe<quest?>" + message;
    if (!sessionId.empty()) {
        fullMessage += "|" + sessionId;
    }
    zmq::message_t msg(fullMessage.size());
    memcpy(msg.data(), fullMessage.c_str(), fullMessage.size());
    socket.send(msg, zmq::send_flags::dontwait);
}

void ServiceSender::receiveMessage() {

    zmq::message_t msg;
    subscriber.recv(msg);
    QString receivedMessage = QString::fromStdString(std::string(static_cast<char*>(msg.data()), msg.size()));
    //std::cout << "ReceivedMessageOne: " << receivedMessage.toStdString() << std::endl;

    size_t pos = receivedMessage.toStdString().find("|");
    if (pos != std::string::npos && pos + 1 < receivedMessage.toStdString().size()) {
        sessionId = receivedMessage.toStdString().substr(pos + 1);
        CreateMainWindow(sessionId);
    }
}

void ServiceSender::onReadButtonClicked() {
    inputLineEdit = mainWindow->findChild<QLineEdit*>("inputLineEdit");
    if (inputLineEdit) {
        std::string message = inputLineEdit->text().toStdString();
        sendMessage(message, ventilator);
        inputLineEdit->clear();
    }
}

void ServiceSender::onExitButtonClicked() {

    std::cout << "Received Exit" << std::endl;
    inputLineEdit->clear();
    AantalGokken = 4;
    CountFunction = 0;
    std::string message = "thibe<quest!>/ServiceOneNOTActive/";
    sendMessage(message,ventilator);
    StopButtonPressed = true;
    std::cout << "StopButtonPressed: " << StopButtonPressed << std::endl;
}

void ServiceSender::CreateMainWindow(const std::string& sessionId) {
    mainWindow = new QWidget();
    mainWindow->setWindowTitle("Stock Exchange Service");

    descriptionLabel = new QLabel("Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n", mainWindow);
    inputLineEdit = new QLineEdit(mainWindow);
    inputLineEdit->setPlaceholderText("Enter your text here");
    inputLineEdit->setObjectName("inputLineEdit"); // Setting object name for finding later

    QPushButton *readButton = new QPushButton("Read Input", mainWindow);
    QPushButton *exitButton = new QPushButton("Exit", mainWindow);

    MessageOne = new QLabel(mainWindow);

    QVBoxLayout *layoutV = new QVBoxLayout();
    layoutV->addWidget(descriptionLabel);
    layoutV->addWidget(inputLineEdit);
    layoutV->addWidget(readButton);
    layoutV->addWidget(exitButton);
    layoutV->addWidget(MessageOne);

    QHBoxLayout *layoutH = new QHBoxLayout();
    layoutH->addWidget(&Goodcolorgoodplace);
    layoutH->addWidget(&GoodcolorgoodplaceTwo);
    layoutH->addWidget(&GoodcolorgoodplaceThree);
    layoutH->addWidget(&GoodcolorgoodplaceFour);

    layoutH->addWidget(&Goodcolorwrongplace);
    layoutH->addWidget(&GoodcolorwrongplaceTwo);
    layoutH->addWidget(&GoodcolorwrongplaceThree);
    layoutH->addWidget(&GoodcolorwrongplaceFour);

    layoutH->addWidget(&Wrongcolorwrongplace);
    layoutH->addWidget(&WrongcolorwrongplaceTwo);
    layoutH->addWidget(&WrongcolorwrongplaceThree);
    layoutH->addWidget(&WrongcolorwrongplaceFour);

    QVBoxLayout *mainLayout = new QVBoxLayout(); // You missed initializing mainLayout
    mainLayout->addLayout(layoutV);
    mainLayout->addLayout(layoutH);

    mainWindow->setLayout(mainLayout);
    mainWindow->resize(400, 300);
    mainWindow->show();

    // Connect the button click signal to the slot
    connect(readButton, &QPushButton::clicked, this, &ServiceSender::onReadButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &ServiceSender::onExitButtonClicked);
    receiveThread = std::thread(&ServiceSender::startReceiving, this);
}

void ServiceSender::startReceiving() {
    // Voer keepreceiveMessage continu uit in een loop
    while (true) {
        keepreceiveMessage();
        // Voeg eventueel een kleine vertraging toe om CPU-resources te sparen
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Bijvoorbeeld 100 milliseconden
    }
}

void ServiceSender::keepreceiveMessage() {
    zmq::message_t msgone;
    subscriber.recv(msgone);
    std::string receivedMessageone = std::string(static_cast<char*>(msgone.data()), msgone.size());
//    std::cout << "Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n" << std::endl;
    size_t pos = receivedMessageone.find("|");
    if (pos != std::string::npos && pos + 1 < receivedMessageone.size()) {
        std::string content = receivedMessageone.substr(0, pos);
        std::string id = receivedMessageone.substr(pos + 1);

        if(id == sessionId)
        {
            size_t pos = receivedMessageone.find("(");
            if (pos == std::string::npos && pos + 1 < receivedMessageone.size()) {
                std::string messagedescription = "something";
                std::cout << "i am here nothing found: " << std::endl;
                startServiceThree(id,receivedMessageone);
            }
            std::cout << "i am here something found: " << std::endl;
            size_t postwo = content.find("(");
            if (postwo != std::string::npos && postwo + 1 < content.size()) {
                std::string ServiceInfobad = content.substr(0, postwo);
                std::string ServiceInfobadd = content.substr(postwo + 1);

                size_t posthree = ServiceInfobadd.find(")");
                if (posthree != std::string::npos && posthree + 1 < ServiceInfobadd.size()) {
                    std::string ServiceInfogood = ServiceInfobadd.substr(0, posthree);
                    std::string Derdeinvoer = ServiceInfobadd.substr(posthree + 1);

                    //std::cout << "ID: "<<id << std::endl;
                    //std::cout << "Derdeinvoer: "<<Derdeinvoer << std::endl;
                    //std::cout << "receivedSessionId: "<<ServiceInfogood << std::endl;

                    if(ServiceInfogood == "ServiceOneActive" )
                    {
                        std::cout << "function to start ServiceOne" << std::endl;
                        startServiceOne(id,Derdeinvoer);
                    }
                    if(ServiceInfogood == "ServiceTwoActive" )
                    {
                        std::cout << "function to start ServiceTwo" << std::endl;
                        startServiceTwo(id,Derdeinvoer);
                    }
                    if(ServiceInfogood == "ServiceThreeActive" )
                    {

                        std::cout << "function to start ServiceThree" << std::endl;
                        startServiceThree(id,Derdeinvoer);

                    }
                }
            }
        }
    }
}

void ServiceSender::startServiceOne(const std::string& id, const std::string& Derdeinvoer)
{
    if(StopButtonPressed == true)
    {
        std::cout << "Service One is Stopped" << std::endl;
    }
    std::cout << "Service One is Active" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "DerdeInvoer: " << Derdeinvoer << std::endl;

    MessageOne->clear();
    MessageOne->setText(QString::fromStdString(Derdeinvoer));

}

void ServiceSender::startServiceTwo(const std::string& id, const std::string& Derdeinvoer)
{
    std::cout << "Service Two is Active" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "DerdeInvoer: " << Derdeinvoer << std::endl;

    descriptionLabel->clear();
    std::cout << "Service Two is Stopped" << std::endl;
    std::string messagedescription = "Welcome To Service Two";
    descriptionLabel->setText(QString::fromStdString(messagedescription));

    MessageOne->clear();
    MessageOne->setText(QString::fromStdString(Derdeinvoer));

    if(StopButtonPressed == true)
    {
        descriptionLabel->clear();
        std::string messagedescription ="Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n";
        descriptionLabel->setText(QString::fromStdString(messagedescription));
         startReceiving();
    }

}

void ServiceSender::startServiceThree(const std::string& id, const std::string& Derdeinvoer)
{

    std::string ControleStop = "NOT_ACTIVE_OKE";

    std::cout << "Service Three is Active" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "DerdeInvoer: " << Derdeinvoer << std::endl;
    std::cout << "I am escaped " << std::endl;

 descriptionLabel->clear();

    size_t pos = Derdeinvoer.find("|");
    if (pos != std::string::npos && pos + 1 < Derdeinvoer.size()) {

        AantalGokken = AantalGokken -1;
        std::string  Gokkenover;
       Gokkenover = std::to_string(AantalGokken); // Convert integer to string

        std::string messagedescription = "Welcome To MasterMind\n\nTo win the Game you have to guess the correct colors on the correct place\nYou have " + Gokkenover + " attempts to guess then the game is over!\n\nExample:\ncolorone colortwo colorthree colorfour\n\nGood Luck!!\n\n";
        descriptionLabel->setText(QString::fromStdString(messagedescription));
        std::cout << "i am here now found in startServiceThree pos "<<Derdeinvoer << std::endl;
        ColorGuessPointer = Derdeinvoer;
        std::cout << "ColorguessOne: " << ColorGuessPointer << std::endl;
        std::cout << "AantalGokken" << AantalGokken<< std::endl;
        std::cout << "Gokkenover" << Gokkenover<< std::endl;

        size_t posthree = ColorGuessPointer.find("/");
        if (posthree != std::string::npos && posthree + 1 < ColorGuessPointer.size()) {
            std::string badstringOne = ColorGuessPointer.substr(0, posthree);
            std::string StatusStoppedOrNotbad = ColorGuessPointer.substr(posthree + 1);

            size_t postfour = StatusStoppedOrNotbad.find("]");
            if (postfour != std::string::npos && postfour + 1 < StatusStoppedOrNotbad.size()) {
                std::string StatusStoppedOrNotGood = StatusStoppedOrNotbad.substr(0, postfour);
                std::string badstringtwo = StatusStoppedOrNotbad.substr(postfour + 1);

                std::cout << "StatusStoppedOrNotGood: " << StatusStoppedOrNotGood << std::endl;
                if(StatusStoppedOrNotGood == ControleStop)
                {
                    std::cout << "ServiceThree Confirmed Stop " << std::endl;

                    descriptionLabel->clear();
                    Goodcolorgoodplace.clear();
                    Goodcolorwrongplace.clear();
                    Wrongcolorwrongplace.clear();
                    GoodcolorgoodplaceTwo.clear();
                    GoodcolorwrongplaceTwo.clear();
                    WrongcolorwrongplaceTwo.clear();
                    GoodcolorgoodplaceThree.clear();
                    GoodcolorwrongplaceThree.clear();
                    WrongcolorwrongplaceThree.clear();
                    GoodcolorgoodplaceFour.clear();
                    GoodcolorwrongplaceFour.clear();
                    WrongcolorwrongplaceFour.clear();

                    std::string messagedescription ="Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n";
                    descriptionLabel->setText(QString::fromStdString(messagedescription));

                    startReceiving();
                }
            }}
    }

    else if (pos == std::string::npos && pos + 1 < Derdeinvoer.size()) {

        std::cout << "CountFunction" << CountFunction<< std::endl;
        std::cout << "CountFunctionOne" << CountFunction<< std::endl;
        std::cout << "i am here not found in startServiceThree pos " << std::endl;
        std::string messagedescription = "Welcome To MasterMind\n\nTo win the Game you have to guess the correct colors on the correct place\nYou have 5 attempts to guess then the game is over!\n\nExample:\ncolorone colortwo colorthree colorfour\n\nGood Luck!!\n\n";
        descriptionLabel->setText(QString::fromStdString(messagedescription));

        zmq::message_t msg;
        subscriber.recv(msg);
        std::string Colorguess = std::string(static_cast<char*>(msg.data()), msg.size());
        ColorGuessPointer = Colorguess;
        std::cout << "ColorguessTwo: " << ColorGuessPointer << std::endl;

        size_t posthree = ColorGuessPointer.find("/");
        if (posthree != std::string::npos && posthree + 1 < ColorGuessPointer.size()) {
            std::string badstringOne = ColorGuessPointer.substr(0, posthree);
            std::string StatusStoppedOrNotbad = ColorGuessPointer.substr(posthree + 1);

            size_t postfour = StatusStoppedOrNotbad.find("]");
            if (postfour != std::string::npos && postfour + 1 < StatusStoppedOrNotbad.size()) {
                std::string StatusStoppedOrNotGood = StatusStoppedOrNotbad.substr(0, postfour);
                std::string badstringtwo = StatusStoppedOrNotbad.substr(postfour + 1);

             std::cout << "StatusStoppedOrNotGood: " << StatusStoppedOrNotGood << std::endl;
                if(StatusStoppedOrNotGood == ControleStop)
             {
                     std::cout << "ServiceThree Confirmed Stop " << std::endl;
                    std::string messagedescription ="Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n";
                    descriptionLabel->setText(QString::fromStdString(messagedescription));
                    startReceiving();
             }
            }}
    }

    //thibe<quest!>almost red bad green perfect yellow bad pink|4074

    errorMessageLabel.setStyleSheet("color: red;");
    Goodcolorgoodplace.setStyleSheet("color: green;");
    Goodcolorwrongplace.setStyleSheet("color: purple;");
    Wrongcolorwrongplace.setStyleSheet("color: red;");
    GoodcolorgoodplaceTwo.setStyleSheet("color: green;");
    GoodcolorwrongplaceTwo.setStyleSheet("color: purple;");
    WrongcolorwrongplaceTwo.setStyleSheet("color: red;");
    GoodcolorgoodplaceThree.setStyleSheet("color: green;");
    GoodcolorwrongplaceThree.setStyleSheet("color: purple;");
    WrongcolorwrongplaceThree.setStyleSheet("color: red;");
    GoodcolorgoodplaceFour.setStyleSheet("color: green;");
    GoodcolorwrongplaceFour.setStyleSheet("color: purple;");
    WrongcolorwrongplaceFour.setStyleSheet("color: red;");

    Goodcolorgoodplace.clear();
    Goodcolorwrongplace.clear();
    Wrongcolorwrongplace.clear();
    GoodcolorgoodplaceTwo.clear();
    GoodcolorwrongplaceTwo.clear();
    WrongcolorwrongplaceTwo.clear();
    GoodcolorgoodplaceThree.clear();
    GoodcolorwrongplaceThree.clear();
    WrongcolorwrongplaceThree.clear();
    GoodcolorgoodplaceFour.clear();
    GoodcolorwrongplaceFour.clear();
    WrongcolorwrongplaceFour.clear();

    std::cout << "I am here before parser " << std::endl;

    size_t spacePosone = ColorGuessPointer.find('>');
    if (spacePosone != std::string::npos) {
        std::string StringOne = ColorGuessPointer.substr(0, spacePosone);
        std::string StringTwo = ColorGuessPointer.substr(spacePosone + 1);

        size_t spacePostwo= StringTwo.find(' ');
        if (spacePostwo != std::string::npos) {
            std::string StatusColorOne = StringTwo.substr(0, spacePostwo);
            std::string StringFour = StringTwo.substr(spacePostwo + 1);

            size_t spacePosthree= StringFour.find(' ');
            if (spacePostwo != std::string::npos) {
                std::string ColorOne = StringFour.substr(0, spacePosthree);//oke
                std::string  StringSix = StringFour.substr(spacePosthree + 1);

                size_t spacePosFour= StringSix.find(' ');
                if (spacePosFour != std::string::npos) {
                    std::string StatusColorTwo = StringSix.substr(0, spacePosFour);//oke
                    std::string  StringEight= StringSix.substr(spacePosFour + 1);

                    size_t spacePoseFive= StringEight.find(' ');
                    if (spacePoseFive != std::string::npos) {
                        std::string ColorTwo = StringEight.substr(0, spacePoseFive);//oke
                        std::string  StringTen= StringEight.substr(spacePoseFive + 1);

                        size_t spacePoseSix= StringTen.find(' ');
                        if (spacePoseSix != std::string::npos) {
                            std::string StatusColorThree = StringTen.substr(0, spacePoseSix);//oke
                            std::string  StringTwelve= StringTen.substr(spacePoseSix + 1);

                            size_t spacePoseSeven= StringTwelve.find(' ');
                            if (spacePoseSeven != std::string::npos) {
                                std::string ColorThree = StringTwelve.substr(0, spacePoseSeven);//oke
                                std::string  StringFourteen= StringTwelve.substr(spacePoseSeven + 1);

                                size_t spacePoseEight= StringFourteen.find(' ');
                                if (spacePoseEight != std::string::npos) {
                                    std::string StatusColorFour = StringFourteen.substr(0, spacePoseEight);//oke
                                    std::string  StringSixteen= StringFourteen.substr(spacePoseEight + 1);

                                    size_t spacePoseNine= StringSixteen.find('|');
                                    if (spacePoseNine != std::string::npos) {
                                        std::string ColorFour = StringSixteen.substr(0, spacePoseNine);//oke
                                        std::string  StringEighteen= StringSixteen.substr(spacePoseNine + 1);//oke

                                        std::cout << "StatusColorOne: " << StatusColorOne << std::endl;//oke
                                        std::cout << "ColorOne: " << ColorOne << std::endl;//oke
                                        std::cout << "StatusColorTwo: " << StatusColorTwo<< std::endl;
                                        std::cout << "ColorTwo: " << ColorTwo << std::endl;//oke
                                        std::cout << "StatusColorThree: " << StatusColorThree << std::endl;
                                        std::cout << "ColorThree: " << ColorThree << std::endl;//oke
                                        std::cout << "StatusColorFour: " << StatusColorFour<< std::endl;
                                        std::cout << "ColorFour: " << ColorFour << std::endl;//oke
                                        //  std::cout << "StringEighteen: " << StringEighteen<< std::endl; is id niet nodig

                                        std::cout << "I am here after parser " << std::endl;
                                        CountFunction = CountFunction +1;
                                        std::cout << "CountFunction: " << CountFunction << std::endl;//oke

                                        if(CountFunction == 2) {
                                            std::cout << "CountFunctionTwo" << CountFunction<< std::endl;
                                            std::cout << "it is good but no"<< std::endl;
                                             descriptionLabel->clear();
                                            std::string messagedescription = "Welcome To MasterMind\n\nTo win the Game you have to guess the correct colors on the correct place\nYou have 4 attempts to guess then the game is over!\n\nExample:\ncolorone colortwo colorthree colorfour\n\nGood Luck!!\n\n";
                                            descriptionLabel->setText(QString::fromStdString(messagedescription));
                                        }
                                        if(CountFunction == 5) {
                                            std::cout << "you lose"<< std::endl;
                                            descriptionLabel->clear();
                                            std::string messagedescription = "You have lost i am opening another window";
                                            descriptionLabel->setText(QString::fromStdString(messagedescription));
                                        }

                                        if (StatusColorOne == "perfect") {
                                            Goodcolorgoodplace.setText(QString::fromStdString(ColorOne));
                                            MessageOne = &Goodcolorgoodplace;
                                        }
                                        if (StatusColorOne == "almost") {
                                            Goodcolorwrongplace.setText(QString::fromStdString(ColorOne));
                                            MessageOne = &Goodcolorwrongplace;
                                        }

                                        if (StatusColorOne == "bad") {
                                            Wrongcolorwrongplace.setText(QString::fromStdString(ColorOne));
                                            MessageOne = &Wrongcolorwrongplace;
                                        }

                                        if (StatusColorTwo == "perfect") {
                                            GoodcolorgoodplaceTwo.setText(QString::fromStdString(ColorTwo));
                                            MessageTwo = &GoodcolorgoodplaceTwo;
                                        }

                                        if (StatusColorTwo == "almost") {
                                            GoodcolorwrongplaceTwo.setText(QString::fromStdString(ColorTwo));
                                            MessageTwo = &GoodcolorwrongplaceTwo;
                                        }

                                        if (StatusColorTwo == "bad") {
                                            WrongcolorwrongplaceTwo.setText(QString::fromStdString(ColorTwo));
                                            MessageTwo = &WrongcolorwrongplaceTwo;
                                        }

                                        if (StatusColorThree == "perfect") {
                                            GoodcolorgoodplaceThree.setText(QString::fromStdString(ColorThree));
                                            MessageThree = &GoodcolorgoodplaceThree;
                                        }

                                        if (StatusColorThree == "almost") {
                                            GoodcolorwrongplaceThree.setText(QString::fromStdString(ColorThree));
                                            MessageThree = &GoodcolorwrongplaceThree;
                                        }

                                        if (StatusColorThree == "bad") {
                                            WrongcolorwrongplaceThree.setText(QString::fromStdString(ColorThree));
                                            MessageThree = &WrongcolorwrongplaceThree;
                                        }

                                        if (StatusColorFour == "perfect") {
                                            GoodcolorgoodplaceFour.setText(QString::fromStdString(ColorFour));
                                            MessageFour = &GoodcolorgoodplaceFour;
                                        }

                                        if (StatusColorFour == "almost") {
                                            GoodcolorwrongplaceFour.setText(QString::fromStdString(ColorFour));
                                            MessageFour = &GoodcolorwrongplaceFour;
                                        }

                                        if (StatusColorFour == "bad") {
                                            WrongcolorwrongplaceFour.setText(QString::fromStdString(ColorFour));
                                            MessageFour = &WrongcolorwrongplaceFour;
                                        }

                                        std::cout << MessageOne->text().toStdString() + MessageTwo->text().toStdString() + MessageThree->text().toStdString() + MessageFour->text().toStdString()<< std::endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}

ServiceSender::~ServiceSender() {

}

std::string ServiceSender::generateSessionId() {
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1000, 9999); // Assuming session ID is a 4-digit number

    // Generate random session ID
    return std::to_string(dis(gen));
}