//
// Created by 2020/12/17.
//

#ifndef YYS_FZ_ACCEPTBATTLETASK_H
#define YYS_FZ_ACCEPTBATTLETASK_H

#include "BaseGameTask.h"

class AcceptBattleTask : public BaseGameTask {
public:
    explicit AcceptBattleTask(const std::string &configJsonStr);

public:
    static GameTask *createInstance(const std::string &configJsonStr);

    void exec(GameClient *client, std::vector<int> &otherClientTaskIds) override;

    ~AcceptBattleTask() override;

    int getTaskId() override;

    int getGroupTaskId() override;

private:
    bool enterLoop;

    void initConfigCallback(Json::Value *configJson);
};


#endif //YYS_FZ_ACCEPTBATTLETASK_H
