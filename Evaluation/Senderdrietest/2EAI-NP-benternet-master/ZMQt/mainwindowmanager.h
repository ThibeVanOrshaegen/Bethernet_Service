#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <zmq.hpp>
#include <QObject>
#include <string>
#include <thread>

class MainWindowManager : public QObject {
    Q_OBJECT
public:
    static void CreateMainWindow(const std::string& sessionId);

    static QWidget *mainWindow;
    static QLineEdit *inputLineEdit;
    static QLabel *descriptionLabel;
    static QLabel *MessageOne;

    static zmq::context_t context;
    static zmq::socket_t ventilator;
    static zmq::socket_t subscriber;
    static std::string sessionId; // Store session ID
    static std::thread receiveThread;

    static QLabel *MessageTwo;
    static QLabel *MessageThree;
    static QLabel *MessageFour;

    static std::string ColorGuessPointer;

    static QLabel *descriptionLabelColorOne;
    static QLabel *descriptionLabelColorTwo;
    static QLabel *descriptionLabelColorThree;
    static QLabel *descriptionLabelColorFour;

    static QLabel *descriptionLabelPerfect;
    static QLabel *descriptionLabelAlmost;
    static QLabel *descriptionLabelBad;

    static QLabel errorMessageLabel;
    static QLabel *Goodcolorgoodplace;
    static QLabel *Goodcolorwrongplace;
    static QLabel *Wrongcolorwrongplace;
    static QLabel *GoodcolorgoodplaceTwo;
    static QLabel *GoodcolorwrongplaceTwo;
    static QLabel *WrongcolorwrongplaceTwo;
    static QLabel *GoodcolorgoodplaceThree;
    static QLabel *GoodcolorwrongplaceThree;
    static QLabel *WrongcolorwrongplaceThree;
    static QLabel *GoodcolorgoodplaceFour;
    static QLabel *GoodcolorwrongplaceFour;
    static QLabel *WrongcolorwrongplaceFour;

    static bool StopButtonPressed;

private slots:
    static void onReadButtonClicked();
    static void onExitButtonClicked();
};

#endif // MAINWINDOWMANAGER_H
