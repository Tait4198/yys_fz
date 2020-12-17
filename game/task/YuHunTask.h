#ifndef YYS_FZ_YUHUNTASK_H
#define YYS_FZ_YUHUNTASK_H

#include "BaseGameTask.h"

class YuHunTask : public BaseGameTask {
public:
    explicit YuHunTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

public:
    static GameTask *createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

    bool exec(std::vector<int> &otherClientTaskIds) override;

    ~YuHunTask() override;

    int getTaskId() override;

    int getGroupTaskId() override;

private:
    bool enterLoop;
    int battleCount{};

    void initConfigCallback(Json::Value *configJson);
};


#endif //YYS_FZ_YUHUNTASK_H
