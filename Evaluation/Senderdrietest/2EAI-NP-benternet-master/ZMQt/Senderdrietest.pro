TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

DEFINES += ZMQ_STATIC
# DEFINES += NZMQT_LIB

LIBS += -L$$PWD/../lib -lzmq -lnzmqt -lws2_32 -lIphlpapi
INCLUDEPATH += $$PWD/../include

SOURCES += main.cpp \
    mainwindowmanager.cpp \
    serviceone.cpp \
    servicesender.cpp \
    servicethree.cpp \
    servicetwo.cpp

QT += network
QT += widgets

HEADERS += \
    mainwindowmanager.h \
    serviceone.h \
    servicesender.h \
    servicethree.h \
    servicetwo.h
