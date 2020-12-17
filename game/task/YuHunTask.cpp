//
// Created by 2020/12/17.
//

#include "YuHunTask.h"

GameTask *YuHunTask::createInstance(const std::string &configJsonStr) {
    return new YuHunTask(configJsonStr);
}

void YuHunTask::exec(GameClient *client, std::vector<int> &otherClientTaskIds) {
    if (this->enterLoop) {

    } else {

    }
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

YuHunTask::YuHunTask(const std::string &configJsonStr) {
    YuHunTask::initConfig(configJsonStr, [this](auto &&PH1) {
        initConfigCallback(std::forward<decltype(PH1)>(PH1));
    });
    this->enterLoop = false;
}


