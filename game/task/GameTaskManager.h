#ifndef YYS_FZ_GAMETASKMANAGER_H
#define YYS_FZ_GAMETASKMANAGER_H

#include "GameTask.h"
#include <map>
#include <string>

typedef std::function<GameTask *(const std::string &)> TaskCreateFuncType;

class GameTaskManager {
public:
    GameTaskManager();

    ~GameTaskManager();

    GameTask *newTask(std::string &&, const std::string &configJsonStr);

private:
    std::map<std::string, TaskCreateFuncType> createTaskMap;

    void registerTask();
};


#endif //YYS_FZ_GAMETASKMANAGER_H
