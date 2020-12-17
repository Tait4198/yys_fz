#include "AcceptBattleTask.h"

AcceptBattleTask::AcceptBattleTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager)
        : BaseGameTask(client, compareManager) {
    AcceptBattleTask::initConfig(configJsonStr, [this](auto &&PH1) {
        initConfigCallback(std::forward<decltype(PH1)>(PH1));
    });
    this->enterLoop = false;
    this->otherClientReady = false;
}

GameTask *AcceptBattleTask::createInstance(const std::string &configJsonStr,
                                           GameClient *client, CompareManager *compareManager) {
    return new AcceptBattleTask(configJsonStr, client, compareManager);
}

bool AcceptBattleTask::exec(std::vector<int> &otherClientTaskIds) {
    return true;
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