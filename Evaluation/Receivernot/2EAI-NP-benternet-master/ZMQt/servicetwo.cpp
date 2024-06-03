#include "ServiceTwo.h"
#include "ServiceSender.h"
#include <iostream>
#include <QLabel>

void ServiceTwo::startService(const std::string& id, const std::string& Derdeinvoer)
{
    std::cout << "Service Two is Active" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "DerdeInvoer: " << Derdeinvoer << std::endl;

    MainWindowManager::descriptionLabel->clear();
    std::cout << "Service Two is Stopped" << std::endl;
    std::string messagedescription = "Welcome To Service Two";
    MainWindowManager::descriptionLabel->setText(QString::fromStdString(messagedescription));

    MainWindowManager::MessageOne->clear();
    MainWindowManager::MessageOne->setText(QString::fromStdString(Derdeinvoer));

    if(MainWindowManager::StopButtonPressed == true)
    {
        MainWindowManager::descriptionLabel->clear();
        std::string messagedescription ="Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n";
        MainWindowManager::descriptionLabel->setText(QString::fromStdString(messagedescription));
        serviceSender.startReceiving();
    }

}
