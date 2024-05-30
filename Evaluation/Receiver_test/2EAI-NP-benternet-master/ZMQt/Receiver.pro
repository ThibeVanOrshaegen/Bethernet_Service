TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

DEFINES += ZMQ_STATIC
# DEFINES += NZMQT_LIB

LIBS += -L$$PWD/../lib -lzmq -lnzmqt -lws2_32 -lIphlpapi
INCLUDEPATH += $$PWD/../include

SOURCES += main.cpp \
    servicefunctions.cpp \
    serviceonehandler.cpp \
    servicereceiver.cpp \
    servicethreehandler.cpp \
    servicetwohandler.cpp

QT += network
QT += widgets

HEADERS += \
    servicefunctions.h \
    serviceonehandler.h \
    servicereceiver.h \
    servicethreehandler.h \
    servicetwohandler.h
