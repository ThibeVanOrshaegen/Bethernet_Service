#ifndef SERVICEONE_H
#define SERVICEONE_H

#include "qlabel.h"
#include <zmq.hpp>
#include <QObject>
#include <string>
#include <thread>
#include <QLineEdit>
#include <QString>
#include "MainWindowManager.h"
#include "ServiceSender.h"

class ServiceOne {
public:
    void startService(const std::string& id, const std::string& input);
    MainWindowManager mainWindowmanager;


private:
    zmq::context_t context;
    zmq::socket_t ventilator;
    zmq::socket_t subscriber;



};

#endif // SERVICEONE_H
