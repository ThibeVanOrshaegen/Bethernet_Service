#ifndef SERVICETWO_H
#define SERVICETWO_H

#include "ServiceSender.h"
#include "qlabel.h"
#include <zmq.hpp>
#include <QObject>
#include <string>
#include <thread>
#include <QLineEdit>
#include <QString>
#include "MainWindowManager.h"

#include <string>

class ServiceTwo {
public:
    void startService(const std::string& id, const std::string& input);
    MainWindowManager mainWindowmanager;

    private:
    ServiceSender serviceSender;

};

#endif // SERVICETWO_H
