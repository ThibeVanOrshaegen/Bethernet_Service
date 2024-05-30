// main.cpp
#include <QApplication>
#include "servicesender.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ServiceSender sender;
    sender.receiveMessage();
    return app.exec();
}
