#ifndef ZEROMQRECEIVER_H
#define ZEROMQRECEIVER_H

#include <QObject>
#include <zmq.hpp>

class ZeroMQReceiver : public QObject
{
    Q_OBJECT

public:
    explicit ZeroMQReceiver(QObject *parent = nullptr);
    ~ZeroMQReceiver();

private:
    zmq::context_t context;
    zmq::socket_t ventilator;
    zmq::socket_t subscriber;

private slots:
    void processMessage();
};

#endif // ZEROMQRECEIVER_H
