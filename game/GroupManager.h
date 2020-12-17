#ifndef YYS_FZ_GROUPMANAGER_H
#define YYS_FZ_GROUPMANAGER_H

#include "GameClient.h"
#include <map>
#include <vector>

class GroupManager {
public:
    struct GroupClient {
        GameClient *client;
        std::string groupName;
    };

    std::vector<int> getTaskIdByHexHwnd(const std::string &groupName);

    void registered(GameClient *client, std::string groupName);

    void unregister(const std::string &hwndHex);

private:
    std::map<std::string, GroupClient> clientMap;
};


#endif //YYS_FZ_GROUPMANAGER_H
