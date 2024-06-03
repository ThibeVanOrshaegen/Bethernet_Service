#ifndef SERVICETHREE_H
#define SERVICETHREE_H

#include <string>
#include "ServiceSender.h"
#include "qlabel.h"
#include "qlineedit.h"
#include <zmq.hpp>
#include <QObject>
#include <string>
#include <thread>
#include <QLineEdit>
#include <QString>
#include "MainWindowManager.h"

class ServiceThree {
public:
    void startService(const std::string& id, const std::string& input);
    MainWindowManager mainWindowmanager;
    ServiceSender serviceSender;

};

#endif // SERVICETHREE_H
