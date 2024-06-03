#include "ServiceOne.h"
#include <iostream>
#include <QLabel>

void ServiceOne::startService(const std::string& id, const std::string& Derdeinvoer)
{
    if(mainWindowmanager.StopButtonPressed == true)
    {
        std::cout << "Service One is Stopped" << std::endl;
    }
    std::cout << "Service One is Active" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "DerdeInvoer: " << Derdeinvoer << std::endl;

    mainWindowmanager.MessageOne->clear();
    mainWindowmanager.MessageOne->setText(QString::fromStdString(Derdeinvoer));

}
