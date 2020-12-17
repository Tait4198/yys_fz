#include "GameTaskManager.h"
#include "CommonTask.h"
#include "YuHunTask.h"
#include "AcceptBattleTask.h"

GameTaskManager::GameTaskManager() {
    this->registerTask();
}

GameTaskManager::~GameTaskManager() = default;

void GameTaskManager::registerTask() {
    this->createTaskMap["Common"] = CommonTask::createInstance;
    this->createTaskMap["YuHun"] = YuHunTask::createInstance;
    this->createTaskMap["AcceptBattle"] = AcceptBattleTask::createInstance;
}

GameTask *GameTaskManager::newTask(std::string &&taskName, const std::string &configJsonStr) {
    std::string inTaskName = std::forward<std::string>(taskName);
    if (this->createTaskMap.count(inTaskName)) {
        return this->createTaskMap[inTaskName](configJsonStr);
    }
    return nullptr;
}
