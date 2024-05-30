#include "MainWindowManager.h"
#include "ServiceSender.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <iostream>
#include <QApplication>

QWidget* MainWindowManager::mainWindow = nullptr;
QLineEdit* MainWindowManager::inputLineEdit = nullptr;
QLabel* MainWindowManager::descriptionLabel = nullptr;
QLabel* MainWindowManager::MessageOne = nullptr;

zmq::context_t MainWindowManager::context(1);
zmq::socket_t MainWindowManager::ventilator(MainWindowManager::context, ZMQ_PUSH);
zmq::socket_t MainWindowManager::subscriber(MainWindowManager::context, ZMQ_SUB);
std::string MainWindowManager::sessionId;
std::thread MainWindowManager::receiveThread;

QLabel* MainWindowManager::MessageTwo = nullptr;
QLabel* MainWindowManager::MessageThree = nullptr;
QLabel* MainWindowManager::MessageFour = nullptr;

std::string MainWindowManager::ColorGuessPointer;

QLabel* MainWindowManager::descriptionLabelColorOne = nullptr;
QLabel* MainWindowManager::descriptionLabelColorTwo = nullptr;
QLabel* MainWindowManager::descriptionLabelColorThree = nullptr;
QLabel* MainWindowManager::descriptionLabelColorFour = nullptr;

QLabel* MainWindowManager::descriptionLabelPerfect = nullptr;
QLabel* MainWindowManager::descriptionLabelAlmost = nullptr;
QLabel* MainWindowManager::descriptionLabelBad = nullptr;

QLabel MainWindowManager::errorMessageLabel;
QLabel* MainWindowManager::Goodcolorgoodplace = nullptr;
QLabel* MainWindowManager::Goodcolorwrongplace = nullptr;
QLabel* MainWindowManager::Wrongcolorwrongplace = nullptr;
QLabel* MainWindowManager::GoodcolorgoodplaceTwo = nullptr;
QLabel* MainWindowManager::GoodcolorwrongplaceTwo = nullptr;
QLabel* MainWindowManager::WrongcolorwrongplaceTwo = nullptr;
QLabel* MainWindowManager::GoodcolorgoodplaceThree = nullptr;
QLabel* MainWindowManager::GoodcolorwrongplaceThree = nullptr;
QLabel* MainWindowManager::WrongcolorwrongplaceThree = nullptr;
QLabel* MainWindowManager::GoodcolorgoodplaceFour = nullptr;
QLabel* MainWindowManager::GoodcolorwrongplaceFour = nullptr;
QLabel* MainWindowManager::WrongcolorwrongplaceFour = nullptr;

bool MainWindowManager::StopButtonPressed = false;

void MainWindowManager::CreateMainWindow(const std::string& sessionId) {

    QApplication app(argc, argv);

    mainWindow = new QWidget();
    mainWindow->setWindowTitle("Stock Exchange Service");

    descriptionLabel = new QLabel("Choose your service: \n\n1. Shares Under\n2. ExchangeRate Differential\n3. Mastermind\n\nEnter the number of the selected service, and the Stock you want to see:\n- APPL\n- CADL\n- KIMpL\n\nExamples: \n\n- Shares Under 200\n- ExchangeRate Differential AAPL\n- Mastermind Go\n\n", mainWindow);
    inputLineEdit = new QLineEdit(mainWindow);
    inputLineEdit->setPlaceholderText("Enter your text here");
    inputLineEdit->setObjectName("inputLineEdit");
    MessageOne = new QLabel(mainWindow);
    MessageTwo = new QLabel(mainWindow);
    MessageThree = new QLabel(mainWindow);
    MessageFour = new QLabel(mainWindow);

    Goodcolorgoodplace = new QLabel(mainWindow);
    Goodcolorwrongplace = new QLabel(mainWindow);
    Wrongcolorwrongplace = new QLabel(mainWindow);
    GoodcolorgoodplaceTwo = new QLabel(mainWindow);
    GoodcolorwrongplaceTwo = new QLabel(mainWindow);
    WrongcolorwrongplaceTwo = new QLabel(mainWindow);
    GoodcolorgoodplaceThree = new QLabel(mainWindow);
    GoodcolorwrongplaceThree = new QLabel(mainWindow);
    WrongcolorwrongplaceThree = new QLabel(mainWindow);
    GoodcolorgoodplaceFour = new QLabel(mainWindow);
    GoodcolorwrongplaceFour = new QLabel(mainWindow);
    WrongcolorwrongplaceFour = new QLabel(mainWindow);

    QPushButton *readButton = new QPushButton("Read Input", mainWindow);
    QPushButton *exitButton = new QPushButton("Exit", mainWindow);

    MessageOne = new QLabel(mainWindow);

    QVBoxLayout *layoutV = new QVBoxLayout();
    layoutV->addWidget(descriptionLabel);
    layoutV->addWidget(inputLineEdit);
    layoutV->addWidget(readButton);
    layoutV->addWidget(exitButton);
    layoutV->addWidget(MessageOne);

    QHBoxLayout *layoutH = new QHBoxLayout();
    layoutH->addWidget(Goodcolorgoodplace);
    layoutH->addWidget(GoodcolorgoodplaceTwo);
    layoutH->addWidget(GoodcolorgoodplaceThree);
    layoutH->addWidget(GoodcolorgoodplaceFour);

    layoutH->addWidget(Goodcolorwrongplace);
    layoutH->addWidget(GoodcolorwrongplaceTwo);
    layoutH->addWidget(GoodcolorwrongplaceThree);
    layoutH->addWidget(GoodcolorwrongplaceFour);

    layoutH->addWidget(Wrongcolorwrongplace);
    layoutH->addWidget(WrongcolorwrongplaceTwo);
    layoutH->addWidget(WrongcolorwrongplaceThree);
    layoutH->addWidget(WrongcolorwrongplaceFour);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(layoutV);

    mainWindow->setLayout(mainLayout);
    mainWindow->resize(400, 300);
    mainWindow->show();

    QObject::connect(readButton, &QPushButton::clicked, &MainWindowManager::onReadButtonClicked);
    QObject::connect(exitButton, &QPushButton::clicked, &MainWindowManager::onExitButtonClicked);
}

void MainWindowManager::onReadButtonClicked() {
    if (inputLineEdit) {
        std::string message = inputLineEdit->text().toStdString();
        ServiceSender sender;
        sender.sendMessage(message, sender.ventilator);
        inputLineEdit->clear();
    }
}

void MainWindowManager::onExitButtonClicked() {
    std::cout << "Received Exit" << std::endl;
    inputLineEdit->clear();
    ServiceSender sender;
    sender.sendMessage("thibe<quest!>/ServiceOneNOTActive/", sender.ventilator);
    std::cout << "Exit button clicked, service stopped" << std::endl;
}
