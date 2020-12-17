#include "FzManager.h"
#include <json/json.h>
#include <chrono>

using namespace std;

FzManager fzManager;

FzManager::FzManager() {
    system("chcp 65001");
    this->jsonConvert = new JsonConvert;
    this->compareManager = new CompareManager(this->jsonConvert);
    this->gameTaskManager = new GameTaskManager;
    this->groupManager = new GroupManager;

    // 客户端必须最后初始化
    this->updateClients();
}

FzManager::~FzManager() {
    this->cleanClients(true);
    delete this->jsonConvert;
    delete this->compareManager;
    delete this->gameTaskManager;
    delete this->groupManager;
}


int FzManager::updateClients() {
    vector<HWND> hwndList = findYysHwnds();
    int newHwnd = -this->cleanClients(false);
    for (auto &hwnd : hwndList) {
        stringstream ss;
        ss << "0x" << hwnd;
        if (!this->clientMap.count(ss.str())) {
            this->clientMap[ss.str()] = new GameClient(hwnd, ss.str());
            this->compareManager->backToHome(hwnd, ss.str());
            newHwnd++;
        }
    }
    return newHwnd;
}

HWND FzManager::getHwndByHexHwnd(string &&hexHwnd) {
    if (this->clientMap.count(hexHwnd)) {
        return this->clientMap[hexHwnd]->getHwnd();
    }
    return nullptr;
}

CompareResult FzManager::compare(HWND &hwnd, std::string &&cpName) {
    return this->compareManager->compare(hwnd, -1, move(cpName));
}

CompareResult FzManager::compare(std::string &&hexHwnd, std::string &&cpName) {
    std::string iHexHwnd = forward<std::string>(hexHwnd);
    if (this->clientMap.count(iHexHwnd)) {
        GameClient *client = this->clientMap[iHexHwnd];
        return this->compareManager->compare(client->getHwnd(), client->getCurrentPosition(), move(cpName));
    }
    return CompareResult{0, 0, 0, 0, 0, -1, ""};
}

vector<string> FzManager::getHexHwndList() {
    vector<string> hwndList;
    for (auto &iter : this->clientMap) {
        hwndList.push_back(iter.first);
    }
    return hwndList;
}

bool FzManager::execTasks(const std::string &hexHwnd, const std::string &tasksJsonStr) {
    if (this->clientMap.count(hexHwnd)) {
        GameClient *client = this->clientMap[hexHwnd];
        if (client->isRun()) {
            return false;
        }
        client->setRun(true);
        std::vector<GameTask::GameTaskParam> params = convertTaskParam(tasksJsonStr);
        thread th(&FzManager::taskFunc, this, client, params);
        th.detach();
        return true;
    }
    return false;
}

std::vector<GameTask::GameTaskParam> FzManager::convertTaskParam(const std::string &tasksJsonStr) {
    Json::Value jsonValue;
    std::vector<GameTask::GameTaskParam> taskParams;
    if (this->jsonConvert->convert(tasksJsonStr, &jsonValue)) {
        Json::StreamWriterBuilder builder;
        builder.settings_["indentation"] = "";
        for (auto &item : jsonValue) {
            taskParams.push_back(GameTask::GameTaskParam{
                    Json::writeString(builder, item["config"]),
                    item["taskName"].asString(),
                    item["order"].asInt()
            });
        }
        std::sort(taskParams.begin(), taskParams.end());
        for (auto &taskParam : taskParams) {
            printf("Task -> %d %s\n", taskParam.order, taskParam.taskName.c_str());
        }
    }
    return taskParams;
}

void FzManager::taskFunc(GameClient *client, std::vector<GameTask::GameTaskParam> &gameTaskParams) {
    GameTask *commonTask = this->gameTaskManager->newTask("Common", "{}", client, this->compareManager);
    for (auto &gtp : gameTaskParams) {
        GameTask *task = this->gameTaskManager->newTask(move(gtp.taskName), gtp.configJson, client,
                                                        this->compareManager);
        if (task != nullptr) {
            std::vector<int> taskIds;
            for (int i = 0; i < 30; i++) {
                commonTask->exec(taskIds);
                task->exec(taskIds);
                this_thread::sleep_for(chrono::seconds(1));
            }
            delete task;
        } else {
            printf("invalid task %s\n", gtp.taskName.c_str());
        }
    }
    delete commonTask;
    client->setRun(false);
}

int FzManager::cleanClients(bool cleanAll) {
    int cleanCount = 0;
    for (auto &iter : this->clientMap) {
        if (!IsWindow(iter.second->getHwnd()) || cleanAll) {
            delete iter.second;
            this->clientMap.erase(iter.first);
            cleanCount++;
        }
    }
    return cleanCount;
}
