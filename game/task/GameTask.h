#pragma once

#include "../GameClient.h"

class GameTask {
public:
    static GameTask* createInstance();

    virtual void exec(GameClient *client, std::string configJson) = 0;

    virtual int getGroupTaskId() = 0;

    virtual int getTaskId() = 0;

    virtual ~GameTask() = default;
};