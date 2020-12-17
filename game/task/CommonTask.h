#pragma once

#include "GameTask.h"

class CommonTask : public GameTask {
public:
    void offerAReward(GameClient *client, bool accept);

public:
    static GameTask* createInstance();

    void exec(GameClient *client, std::string configJson) override;

    ~CommonTask() override;

    int getTaskId() override;

    int getGroupTaskId() override;
};

