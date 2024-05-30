#ifndef MESSAGING_APP_H
#define MESSAGING_APP_H

#include <iostream>
#include <string>
#include <QApplication>
#include <QMessageBox>
#include <zmq.hpp>

#ifdef _WIN32
#include <windows.h>
#define sleep(n) Sleep(n * 1000) // Convert seconds to milliseconds
#else
#include <unistd.h> // Include unistd.h for sleep function
#endif

void runMessagingApp();

#endif // MESSAGING_APP_H
