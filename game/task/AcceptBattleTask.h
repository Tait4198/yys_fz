#ifndef YYS_FZ_ACCEPTBATTLETASK_H
#define YYS_FZ_ACCEPTBATTLETASK_H

#include "BaseGameTask.h"

class AcceptBattleTask : public BaseGameTask {
public:
    AcceptBattleTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

public:
    static GameTask *
    createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

    bool exec(std::vector<int> &otherClientTaskIds) override;

    ~AcceptBattleTask() override;

    int getTaskId() override;

    int getGroupTaskId() override;

private:
    bool enterLoop;
    bool otherClientReady;

    void initConfigCallback(Json::Value *configJson);
};


#endif //YYS_FZ_ACCEPTBATTLETASK_H
