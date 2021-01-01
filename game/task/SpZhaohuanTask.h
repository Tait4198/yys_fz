#ifndef YYS_FZ_SPZHAOHUANTASK_H
#define YYS_FZ_SPZHAOHUANTASK_H

#include "BaseGameTask.h"


class SpZhaohuanTask  : public BaseGameTask {
public:
    explicit SpZhaohuanTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);
public:
    static GameTask *
    createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

    bool exec(std::vector<GameClient *> &otherClients) override;

    int getGroupTaskId() override;

    int getTaskId() override;

    ~SpZhaohuanTask() override = default;

private:
    int zhCount{};
    int currentZhCount;

    void initConfigCallback(Json::Value *configJson);
};


#endif //YYS_FZ_SPZHAOHUANTASK_H
