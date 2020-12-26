#include "FzManager.h"
#include <json/json.h>
#include <chrono>
#include <thread>

using namespace std;

FzManager fzManager;

FzManager::FzManager() {
//    system("chcp 65001");
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
        GameTask::GameTaskBundle bundle = convertTaskBundle(tasksJsonStr);
        thread th(&FzManager::taskFunc, this, client, bundle);
        th.detach();
        return true;
    }
    return false;
}

GameTask::GameTaskBundle FzManager::convertTaskBundle(const std::string &tasksJsonStr) {
    Json::Value jsonValue;
    std::vector<GameTask::GameTaskParam> taskParams;
    if (this->jsonConvert->convert(tasksJsonStr, &jsonValue)) {
        Json::StreamWriterBuilder builder;
        builder.settings_["indentation"] = "";
        for (auto &item : jsonValue["tasks"]) {
            taskParams.push_back(GameTask::GameTaskParam{
                    Json::writeString(builder, item["config"]),
                    item["taskName"].asString(),
                    item["order"].asInt()
            });
        }
        std::sort(taskParams.begin(), taskParams.end());
        std::string groupName = jsonValue["groupName"].asString();
        int cycles = jsonValue["cycles"].asInt();
        return GameTask::GameTaskBundle{cycles, groupName, taskParams};
    }
    return GameTask::GameTaskBundle{0, "", taskParams};
}

void FzManager::taskFunc(GameClient *client, GameTask::GameTaskBundle &taskBundle) {
    printf("[%s] Task Start\n", client->getHexHwnd().c_str());
    GameTask *commonTask = this->gameTaskManager->newTask("Common", "{}", client, this->compareManager);
    // 注册组
    client->setCurrentGroup(taskBundle.groupName);
    groupManager->registered(client, taskBundle.groupName);
    this_thread::sleep_for(chrono::milliseconds(500));

    for (int i = 0; i < taskBundle.cycles; i++) {
        for (auto &gtp : taskBundle.taskParams) {
            GameTask *task = this->gameTaskManager->newTask(move(gtp.taskName), gtp.configJson, client,
                                                            this->compareManager);
            if (task != nullptr) {
                while (true) {
                    std::vector<GameClient *> otherClient = groupManager->getClientByHexHwnd(client->getHexHwnd());
                    commonTask->exec(otherClient);
                    if (!task->exec(otherClient)) {
                        break;
                    }
                    this_thread::sleep_for(chrono::seconds(1));
                }
                delete task;
            } else {
                printf("invalid task %s\n", gtp.taskName.c_str());
            }
        }
    }
    delete commonTask;
    // 解除组
    client->setCurrentGroup("");
    groupManager->unregister(client->getHexHwnd());
    client->setRun(false);
    printf("[%s] Task End\n", client->getHexHwnd().c_str());
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

CompareManager *FzManager::getCompareManager() {
    return this->compareManager;
}
