#pragma once

#include "GameTask.h"

class CommonTask : public GameTask {
public:
    void offerAReward(GameClient *client, bool accept);

    CommonTask(const std::string& configJsonStr);

public:
    static GameTask *createInstance(std::string configJsonStr);

    void exec(GameClient *client, std::vector<int>& otherClientTaskIds) override;

    ~CommonTask() override;

    int getTaskId() override;

    int getGroupTaskId() override;
};

