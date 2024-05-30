#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <zmq.hpp>
#include <QNetworkAccessManager>
#include <QCoreApplication>

class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = nullptr);
    ~Receiver();

private:
    zmq::context_t context;
    zmq::socket_t ventilator;

signals:
    void requestProcessed(const QByteArray &responseData);

private slots:
    void processRequest(const QString &request);

};

#endif // RECEIVER_H
