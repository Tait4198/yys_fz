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

GameTask *GameTaskManager::newTask(std::string &&taskName, const std::string &configJsonStr,
                                   GameClient *client, CompareManager *compareManager) {
    std::string iTaskName = std::forward<std::string>(taskName);
    if (this->createTaskMap.count(iTaskName)) {
        return this->createTaskMap[iTaskName](configJsonStr, client, compareManager);
    }
    return nullptr;
}
