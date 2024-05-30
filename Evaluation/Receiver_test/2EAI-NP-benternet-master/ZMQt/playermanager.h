#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <string>

// Forward declaration of ServiceReceiver
class ServiceReceiver;

class PlayerManager {
public:
    static bool isNewService(const std::string& message, ServiceReceiver& receiver);
    static ServiceReceiver receiverSensornew; // Declaration only
};

#endif // PLAYER_MANAGER_H
