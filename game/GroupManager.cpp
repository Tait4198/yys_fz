#include "GroupManager.h"

std::vector<GameClient *> GroupManager::getClientByHexHwnd(const std::string &hwndHex) {
    std::vector<GameClient *> clients;
    if (this->clientMap.count(hwndHex)) {
        std::string groupName = this->clientMap[hwndHex].groupName;
        for (auto &iter : this->clientMap) {
            if (iter.first != hwndHex && iter.second.groupName == groupName) {
                clients.push_back(iter.second.client);
            }
        }
    } else {
        printf("Invalid hexHwnd\n");
    }
    return clients;
}

void GroupManager::registered(GameClient *client, std::string groupName) {
    this->clientMap[client->getHexHwnd()] = GroupClient{client, std::move(groupName)};
}

void GroupManager::unregister(const std::string &hwndHex) {
    if (this->clientMap.count(hwndHex)) {
        this->clientMap.erase(hwndHex);
    } else {
        printf("Invalid hexHwnd\n");
    }
}

