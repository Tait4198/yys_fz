//
// Created by 2020/12/17.
//

#include "AcceptBattleTask.h"

AcceptBattleTask::AcceptBattleTask(const std::string &configJsonStr) {
    AcceptBattleTask::initConfig(configJsonStr, [this](auto &&PH1) {
        initConfigCallback(std::forward<decltype(PH1)>(PH1));
    });
    this->enterLoop = false;
}

GameTask *AcceptBattleTask::createInstance(const std::string &configJsonStr) {
    return new AcceptBattleTask(configJsonStr);
}

void AcceptBattleTask::exec(GameClient *client, std::vector<int> &otherClientTaskIds) {

}

AcceptBattleTask::~AcceptBattleTask() = default;

int AcceptBattleTask::getTaskId() {
    return 0;
}

int AcceptBattleTask::getGroupTaskId() {
    return 0;
}

void AcceptBattleTask::initConfigCallback(Json::Value *configJson) {

}
