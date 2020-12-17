#include "GroupManager.h"

#include <utility>

std::vector<int> GroupManager::getTaskIdByGroup(const std::string &hwndHex) {
    std::vector<int> taskIds;
    if (this->clientMap.count(hwndHex)) {
        std::string groupName = this->clientMap[hwndHex].groupName;
        for (auto &iter : this->clientMap) {
            if (!(iter.first == hwndHex) && iter.second.groupName == groupName) {
                taskIds.push_back(iter.second.client->getCurrentTaskId());
            }
        }
    } else {
        printf("Invalid hexHwnd\n");
    }
    return taskIds;
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

