#ifndef YYS_FZ_SPYUHUNBATTLETASK_H
#define YYS_FZ_SPYUHUNBATTLETASK_H

#include "BaseGameTask.h"
#include <random>

class SpYuhunBattleTask : public BaseGameTask {
public:
    explicit SpYuhunBattleTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

public:
    static GameTask *
    createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

    bool exec(std::vector<GameClient *> &otherClients) override;

    ~SpYuhunBattleTask() override;

    int getTaskId() override;

    int getGroupTaskId() override;

private:
    std::random_device randomDev;

    bool initCheck;
    // 是否队长
    bool isLeader;

    int battleCount{};

    int currentBattleCount;
    int battleExecCount;
    int battleFailExecCount;

    void initConfigCallback(Json::Value *configJson);
};


#endif //YYS_FZ_SPYUHUNBATTLETASK_H
