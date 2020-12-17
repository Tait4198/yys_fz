#include "GameTaskManager.h"

GameTaskManager::GameTaskManager() {
    this->registerTask();
}

GameTaskManager::~GameTaskManager() {}

void GameTaskManager::registerTask() {
    this->createTaskMap["Common"] = CommonTask::createInstance
}

GameTask *GameTaskManager::newTask(std::string &&taskName) {
    std::string inTaskName = std::forward<std::string>(taskName);
    if (this->createTaskMap.count(inTaskName)) {
        return this->createTaskMap[inTaskName]();
    }
    return nullptr;
}
