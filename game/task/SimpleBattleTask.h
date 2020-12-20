#ifndef YYS_FZ_SIMPLEBATTLETASK_H
#define YYS_FZ_SIMPLEBATTLETASK_H

#include "BaseGameTask.h"
#include <random>

class SimpleBattleTask : public BaseGameTask {
public:
    explicit SimpleBattleTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

public:
    static GameTask *
    createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

    bool exec(std::vector<GameClient *> &otherClients) override;

    ~SimpleBattleTask() override;

    int getTaskId() override;

    int getGroupTaskId() override;

private:
    int battleCount{};
    int curBattleCount;
    int battleExecCount;
    std::random_device randomDev;
    bool isLeader;

    void initConfigCallback(Json::Value *configJson);
};


#endif //YYS_FZ_SIMPLEBATTLETASK_H
