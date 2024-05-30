ifndef SERVICESENDER_H
#define SERVICESENDER_H

#include "qlabel.h"
#include "qlineedit.h"
#include <zmq.hpp>
#include <QObject>
#include <string>
#include <thread>
#include <QLineEdit>
#include <QString>


class QWidget;
class QPushButton;

class ServiceSender : public QObject {
    Q_OBJECT

public:
    ServiceSender();
    virtual ~ServiceSender();
    void receiveMessage(); // Function to receive messages from the server
    void keepreceiveMessage();
    void startServiceOne(const std::string& receivedSessionId, const std::string& derdeinvoer);
    void startServiceTwo(const std::string& receivedSessionId, const std::string& derdeinvoer);
    void startServiceThree(const std::string& id, const std::string& Derdeinvoer);

    int AantalGokken = 4;
    int CountFunction = 1;


public slots:
    void onReadButtonClicked();
    void onExitButtonClicked();

private:
    zmq::context_t context;
    zmq::socket_t ventilator;
    zmq::socket_t subscriber;
    QWidget* mainWindow;
    std::string sessionId; // Store session ID
    std::thread receiveThread;

    QLabel *MessageOne;
    QLabel *MessageTwo;
    QLabel *MessageThree;
    QLabel *MessageFour;

    std::string ColorGuessPointer;

    QLabel *descriptionLabel;
    QLabel *descriptionLabelColorOne;
    QLabel *descriptionLabelColorTwo;
     QLabel *descriptionLabelColorThree;
     QLabel *descriptionLabelColorFour;

     QLabel *descriptionLabelPerfect;
     QLabel *descriptionLabelAlmost;
     QLabel *descriptionLabelBad;

    QLineEdit *inputLineEdit;

    QLabel errorMessageLabel;
    QLabel Goodcolorgoodplace;
    QLabel Goodcolorwrongplace;
    QLabel Wrongcolorwrongplace;
    QLabel GoodcolorgoodplaceTwo;
    QLabel GoodcolorwrongplaceTwo;
    QLabel WrongcolorwrongplaceTwo;
    QLabel GoodcolorgoodplaceThree;
    QLabel GoodcolorwrongplaceThree;
    QLabel WrongcolorwrongplaceThree;
    QLabel GoodcolorgoodplaceFour;
    QLabel GoodcolorwrongplaceFour;
    QLabel WrongcolorwrongplaceFour;

    bool StopButtonPressed = false;


    void sendMessage(const std::string& message, zmq::socket_t& socket); // Modify to use stored session ID
    void CreateMainWindow(const std::string& sessionId); // Modify to accept session ID
    void startReceiving();


    std::string generateSessionId(); // Method to generate a random session ID

};

#endif // SERVICESENDER_H