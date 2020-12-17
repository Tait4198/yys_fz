#include "YuHunTask.h"

YuHunTask::YuHunTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager)
        : BaseGameTask(client, compareManager) {
    YuHunTask::initConfig(configJsonStr, [this](auto &&PH1) {
        initConfigCallback(std::forward<decltype(PH1)>(PH1));
    });
    this->enterLoop = false;
}

GameTask *
YuHunTask::createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager) {
    return new YuHunTask(configJsonStr, client, compareManager);
}

bool YuHunTask::exec(std::vector<int> &otherClientTaskIds) {
    if (this->enterLoop) {

    } else {

    }
    return true;
}

void YuHunTask::initConfigCallback(Json::Value *configJson) {
    this->battleCount = configJson->operator[]("battleCount").asInt();
}

YuHunTask::~YuHunTask() = default;

int YuHunTask::getTaskId() {
    return 0;
}

int YuHunTask::getGroupTaskId() {
    return 0;
}


