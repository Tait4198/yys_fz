//
// Created by 2020/12/17.
//

#ifndef YYS_FZ_YUHUNTASK_H
#define YYS_FZ_YUHUNTASK_H

#include "BaseGameTask.h"

class YuHunTask : public BaseGameTask {
public:
    YuHunTask(const std::string &configJson);

public:
    static GameTask *createInstance(const std::string &configJsonStr);

    void exec(GameClient *client, std::vector<int> &otherClientTaskIds) override;

    ~YuHunTask() override;

    int getTaskId() override;

    int getGroupTaskId() override;

private:
    bool enterLoop{};
    int battleCount{};

    void initConfigCallback(Json::Value *configJson);
};


#endif //YYS_FZ_YUHUNTASK_H
