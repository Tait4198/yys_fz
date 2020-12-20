#pragma once

#include "GameTask.h"

class CommonTask : public GameTask {
public:
    void offerAReward(bool accept);

    explicit CommonTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

public:
    static GameTask *
    createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

    bool exec(std::vector<GameClient *> &otherClients) override;

    ~CommonTask() override;

    int getTaskId() override;

    int getGroupTaskId() override;
};

