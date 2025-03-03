#pragma once

#include "../CompareManager.h"
#include "../GameClient.h"

class GameTask {
public:
    GameTask(GameClient *client, CompareManager *compareManager);

    struct GameTaskParam {
        std::string configJson;
        std::string taskName;
        int order;

        bool operator<(const GameTaskParam &param) const {
            return order < param.order;
        }
    };

    struct GameTaskBundle {
        int cycles;
        std::string groupName;
        std::vector<GameTaskParam> taskParams;
    };

    virtual bool exec(std::vector<GameClient *> &otherClients) = 0;

    virtual int getGroupTaskId() = 0;

    virtual int getTaskId() = 0;

    virtual ~GameTask() = default;

protected:
    GameClient *client;
    CompareManager *compareManager;
};