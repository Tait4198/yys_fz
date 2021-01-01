#include "GameTaskManager.h"
#include "CommonTask.h"
#include "SpYuhunBattleTask.h"
#include "SpTupoBattleTask.h"
#include "SpZhaohuanTask.h"

GameTaskManager::GameTaskManager() {
    this->registerTask();
}

GameTaskManager::~GameTaskManager() = default;

void GameTaskManager::registerTask() {
    this->createTaskMap["Common"] = CommonTask::createInstance;
    this->createTaskMap["SpYuhunBattle"] = SpYuhunBattleTask::createInstance;
    this->createTaskMap["SpTupoBattle"] = SpTupoBattleTask::createInstance;
    this->createTaskMap["SpZhaohuan"] = SpZhaohuanTask::createInstance;
}

GameTask *GameTaskManager::newTask(std::string &&taskName, const std::string &configJsonStr,
                                   GameClient *client, CompareManager *compareManager) {
    std::string iTaskName = std::forward<std::string>(taskName);
    if (this->createTaskMap.count(iTaskName)) {
        return this->createTaskMap[iTaskName](configJsonStr, client, compareManager);
    }
    return nullptr;
}
