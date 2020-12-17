#pragma once

#include "Windows.h"
#include "windef.h"
#include "GameCommon.h"
#include "GameClient.h"
#include "CompareManager.h"
#include "GroupManager.h"
#include "../common/JsonConvert.h"
#include "task/GameTaskManager.h"

class FzManager {
public:

    FzManager();

    ~FzManager();

    int updateClients();

    std::vector<std::string> getHexHwndList();

    HWND getHwndByHexHwnd(std::string &&hexHwnd);

    bool execTasks(const std::string &hexHwnd, const std::string &tasksJsonStr);

    CompareResult compare(HWND &hwnd, std::string &&cpName);

    CompareResult compare(std::string &&hexHwnd, std::string &&cpName);

private:
    std::map<std::string, GameClient *> clientMap;
    GameTaskManager *gameTaskManager;
    GroupManager *groupManager;
    JsonConvert *jsonConvert;
    CompareManager *compareManager;

    int cleanClients(bool cleanAll);

    std::vector<GameTask::GameTaskParam> convertTaskParam(const std::string &tasksJsonStr);

    void taskFunc(GameClient *client, std::vector<GameTask::GameTaskParam> &gameTaskParams);
};