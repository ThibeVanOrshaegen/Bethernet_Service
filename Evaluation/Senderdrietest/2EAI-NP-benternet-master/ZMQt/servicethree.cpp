#include "ServiceThree.h"
#include <iostream>
#include <QLabel>

void ServiceThree::startService(const std::string& id, const std::string& Derdeinvoer)
{

    std::string ControleStop = "NOT_ACTIVE_OKE";

    std::cout << "Service Three is Active" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "DerdeInvoer: " << Derdeinvoer << std::endl;
    std::cout << "I am escaped " << std::endl;

    MainWindowManager::descriptionLabel->clear();

    size_t pos = Derdeinvoer.find("|");
    if (pos != std::string::npos && pos + 1 < Derdeinvoer.size()) {

        serviceSender.AantalGokken = serviceSender.AantalGokken -1;
        std::string  Gokkenover;
        Gokkenover = std::to_string(serviceSender.AantalGokken); // Convert integer to string

        std::string messagedescription = "Welcome To MasterMind\n\nTo win the Game you have to guess the correct colors on the correct place\nYou have " + Gokkenover + " attempts to guess then the game is over!\n\nExample:\ncolorone colortwo colorthree colorfour\n\nGood Luck!!\n\n";
        MainWindowManager::descriptionLabel->setText(QString::fromStdString(messagedescription));
        std::cout << "i am here now found in startServiceThree pos "<<Derdeinvoer << std::endl;
       MainWindowManager::ColorGuessPointer = Derdeinvoer;
        std::cout << "ColorguessOne: " << mainWindowmanager.ColorGuessPointer << std::endl;
        std::cout << "AantalGokken" << serviceSender.AantalGokken<< std::endl;
        std::cout << "Gokkenover" << Gokkenover<< std::endl;

        size_t posthree = mainWindowmanager.ColorGuessPointer.find("/");
        if (posthree != std::string::npos && posthree + 1 < mainWindowmanager.ColorGuessPointer.size()) {
            std::string badstringOne = mainWindowmanager.ColorGuessPointer.substr(0, posthree);
            std::string StatusStoppedOrNotbad = mainWindowmanager.ColorGuessPointer.substr(posthree + 1);

            size_t postfour = StatusStoppedOrNotbad.find("]");
            if (postfour != std::string::npos && postfour + 1 < StatusStoppedOrNotbad.size()) {
                std::string StatusStoppedOrNotGood = StatusStoppedOrNotbad.substr(0, postfour);
                std::string badstringtwo = StatusStoppedOrNotbad.substr(postfour + 1);

                std::cout << "StatusStoppedOrNotGood: " << StatusStoppedOrNotGood << std::endl;
                if(StatusStoppedOrNotGood == ControleStop)
                {
                    std::cout << "ServiceThree Confirmed Stop " << std::endl;


                    MainWindowManager::descriptionLabel->clear();
                    MainWindowManager::Goodcolorgoodplace->clear();
                    MainWindowManager::Goodcolorwrongplace->clear();
                    MainWindowManager::Wrongcolorwrongplace->clear();
                    MainWindowManager::GoodcolorgoodplaceTwo->clear();
                    MainWindowManager::GoodcolorwrongplaceTwo->clear();
                    MainWindowManager::WrongcolorwrongplaceTwo->clear();
                    MainWindowManager::GoodcolorgoodplaceThree->clear();
                    MainWindowManager::GoodcolorwrongplaceThree->clear();
                    MainWindowManager::WrongcolorwrongplaceThree->clear();
                    MainWindowManager::GoodcolorgoodplaceFour->clear();
                    MainWindowManager::GoodcolorwrongplaceFour->clear();
                    MainWindowManager::WrongcolorwrongplaceFour->clear();


                    std::string messagedescription ="Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n";
                    mainWindowmanager.descriptionLabel->setText(QString::fromStdString(messagedescription));

                    serviceSender.startReceiving();
                }
            }}
    }

    else if (pos == std::string::npos && pos + 1 < Derdeinvoer.size()) {

        std::cout << "serviceSender.CountFunction" << serviceSender.CountFunction<< std::endl;
        std::cout << "serviceSender.CountFunctionOne" << serviceSender.CountFunction<< std::endl;
        std::cout << "i am here not found in startServiceThree pos " << std::endl;
        std::string messagedescription = "Welcome To MasterMind\n\nTo win the Game you have to guess the correct colors on the correct place\nYou have 5 attempts to guess then the game is over!\n\nExample:\ncolorone colortwo colorthree colorfour\n\nGood Luck!!\n\n";
        mainWindowmanager.descriptionLabel->setText(QString::fromStdString(messagedescription));

        zmq::message_t msg;
        serviceSender.subscriber.recv(msg);
        std::string Colorguess = std::string(static_cast<char*>(msg.data()), msg.size());
        mainWindowmanager.ColorGuessPointer = Colorguess;
        std::cout << "ColorguessTwo: " << mainWindowmanager.ColorGuessPointer << std::endl;

        size_t posthree = mainWindowmanager.ColorGuessPointer.find("/");
        if (posthree != std::string::npos && posthree + 1 < mainWindowmanager.ColorGuessPointer.size()) {
            std::string badstringOne = mainWindowmanager.ColorGuessPointer.substr(0, posthree);
            std::string StatusStoppedOrNotbad = mainWindowmanager.ColorGuessPointer.substr(posthree + 1);

            size_t postfour = StatusStoppedOrNotbad.find("]");
            if (postfour != std::string::npos && postfour + 1 < StatusStoppedOrNotbad.size()) {
                std::string StatusStoppedOrNotGood = StatusStoppedOrNotbad.substr(0, postfour);
                std::string badstringtwo = StatusStoppedOrNotbad.substr(postfour + 1);

                std::cout << "StatusStoppedOrNotGood: " << StatusStoppedOrNotGood << std::endl;
                if(StatusStoppedOrNotGood == ControleStop)
                {
                    std::cout << "ServiceThree Confirmed Stop " << std::endl;
                    std::string messagedescription ="Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n";
                    mainWindowmanager.descriptionLabel->setText(QString::fromStdString(messagedescription));
                    serviceSender.startReceiving();
                }
            }}
    }
    MainWindowManager::errorMessageLabel.setStyleSheet("color: red;");
    MainWindowManager::Goodcolorgoodplace->setStyleSheet("color: green;");
    MainWindowManager::Goodcolorwrongplace->setStyleSheet("color: purple;");
    MainWindowManager::Wrongcolorwrongplace->setStyleSheet("color: red;");
    MainWindowManager::GoodcolorgoodplaceTwo->setStyleSheet("color: green;");
    MainWindowManager::GoodcolorwrongplaceTwo->setStyleSheet("color: purple;");
    MainWindowManager::WrongcolorwrongplaceTwo->setStyleSheet("color: red;");
    MainWindowManager::GoodcolorgoodplaceThree->setStyleSheet("color: green;");
    MainWindowManager::GoodcolorwrongplaceThree->setStyleSheet("color: purple;");
    MainWindowManager::WrongcolorwrongplaceThree->setStyleSheet("color: red;");
    MainWindowManager::GoodcolorgoodplaceFour->setStyleSheet("color: green;");
    MainWindowManager::GoodcolorwrongplaceFour->setStyleSheet("color: purple;");
    MainWindowManager::WrongcolorwrongplaceFour->setStyleSheet("color: red;");

    MainWindowManager::Goodcolorgoodplace->clear();
    MainWindowManager::Goodcolorwrongplace->clear();
    MainWindowManager::Wrongcolorwrongplace->clear();
    MainWindowManager::GoodcolorgoodplaceTwo->clear();
    MainWindowManager::GoodcolorwrongplaceTwo->clear();
    MainWindowManager::WrongcolorwrongplaceTwo->clear();
    MainWindowManager::GoodcolorgoodplaceThree->clear();
    MainWindowManager::GoodcolorwrongplaceThree->clear();
    MainWindowManager::WrongcolorwrongplaceThree->clear();
    MainWindowManager::GoodcolorgoodplaceFour->clear();
    MainWindowManager::GoodcolorwrongplaceFour->clear();
    MainWindowManager::WrongcolorwrongplaceFour->clear();

    std::cout << "I am here before parser " << std::endl;

    size_t spacePosone = MainWindowManager::ColorGuessPointer.find('>');
    if (spacePosone != std::string::npos) {
        std::string StringOne = MainWindowManager::ColorGuessPointer.substr(0, spacePosone);
        std::string StringTwo = MainWindowManager::ColorGuessPointer.substr(spacePosone + 1);

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
                                        serviceSender.CountFunction = serviceSender.CountFunction +1;
                                        std::cout << "serviceSender.CountFunction: " << serviceSender.CountFunction << std::endl;//oke

                                        if(serviceSender.CountFunction == 2) {
                                            std::cout << "serviceSender.CountFunctionTwo" << serviceSender.CountFunction<< std::endl;
                                            std::cout << "it is good but no"<< std::endl;
                                            MainWindowManager::descriptionLabel->clear();
                                            std::string messagedescription = "Welcome To MasterMind\n\nTo win the Game you have to guess the correct colors on the correct place\nYou have 4 attempts to guess then the game is over!\n\nExample:\ncolorone colortwo colorthree colorfour\n\nGood Luck!!\n\n";
                                            MainWindowManager::descriptionLabel->setText(QString::fromStdString(messagedescription));
                                        }
                                        if(serviceSender.CountFunction == 5) {
                                            std::cout << "you lose"<< std::endl;
                                            MainWindowManager::descriptionLabel->clear();
                                            std::string messagedescription = "You have lost i am opening another window";
                                            MainWindowManager::descriptionLabel->setText(QString::fromStdString(messagedescription));
                                        }

                                        if (StatusColorOne == "perfect") {
                                            MainWindowManager::Goodcolorgoodplace->setText(QString::fromStdString(ColorOne));
                                            MainWindowManager::MessageOne = MainWindowManager::Goodcolorgoodplace;
                                        }
                                        if (StatusColorOne == "almost") {
                                            MainWindowManager:: Goodcolorwrongplace->setText(QString::fromStdString(ColorOne));
                                            MainWindowManager::Goodcolorgoodplace = MainWindowManager::Goodcolorwrongplace;
                                        }

                                        if (StatusColorOne == "bad") {
                                            MainWindowManager::Wrongcolorwrongplace->setText(QString::fromStdString(ColorOne));
                                            MainWindowManager::Goodcolorgoodplace = MainWindowManager::Wrongcolorwrongplace;
                                        }

                                        if (StatusColorTwo == "perfect") {
                                            MainWindowManager::GoodcolorgoodplaceTwo->setText(QString::fromStdString(ColorTwo));
                                            MainWindowManager::MessageTwo = MainWindowManager::GoodcolorgoodplaceTwo;
                                        }

                                        if (StatusColorTwo == "almost") {
                                            MainWindowManager::GoodcolorwrongplaceTwo->setText(QString::fromStdString(ColorTwo));
                                            MainWindowManager::MessageTwo = MainWindowManager::GoodcolorwrongplaceTwo;
                                        }

                                        if (StatusColorTwo == "bad") {
                                            MainWindowManager::WrongcolorwrongplaceTwo->setText(QString::fromStdString(ColorTwo));
                                            MainWindowManager::MessageTwo = MainWindowManager::WrongcolorwrongplaceTwo;
                                        }

                                        if (StatusColorThree == "perfect") {
                                            MainWindowManager::GoodcolorgoodplaceThree->setText(QString::fromStdString(ColorThree));
                                            MainWindowManager::MessageThree = MainWindowManager::GoodcolorgoodplaceThree;
                                        }

                                        if (StatusColorThree == "almost") {
                                            MainWindowManager::GoodcolorwrongplaceThree->setText(QString::fromStdString(ColorThree));
                                            MainWindowManager::MessageThree = MainWindowManager::GoodcolorwrongplaceThree;
                                        }

                                        if (StatusColorThree == "bad") {
                                            MainWindowManager::WrongcolorwrongplaceThree->setText(QString::fromStdString(ColorThree));
                                            MainWindowManager::MessageThree = MainWindowManager::WrongcolorwrongplaceThree;
                                        }

                                        if (StatusColorFour == "perfect") {
                                            MainWindowManager::GoodcolorgoodplaceFour->setText(QString::fromStdString(ColorFour));
                                            MainWindowManager::MessageFour = MainWindowManager::GoodcolorgoodplaceFour;
                                        }

                                        if (StatusColorFour == "almost") {
                                            MainWindowManager::GoodcolorwrongplaceFour->setText(QString::fromStdString(ColorFour));
                                            MainWindowManager::MessageFour = MainWindowManager::GoodcolorwrongplaceFour;
                                        }

                                        if (StatusColorFour == "bad") {
                                            MainWindowManager::WrongcolorwrongplaceFour->setText(QString::fromStdString(ColorFour));
                                            MainWindowManager::MessageFour = MainWindowManager::WrongcolorwrongplaceFour;
                                        }

                                        std::cout << MainWindowManager::Goodcolorgoodplace->text().toStdString() + MainWindowManager::MessageTwo->text().toStdString() + MainWindowManager::MessageThree->text().toStdString() + MainWindowManager::MessageFour->text().toStdString()<< std::endl;
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
