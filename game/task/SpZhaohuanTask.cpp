#include "SpZhaohuanTask.h"

bool SpZhaohuanTask::exec(std::vector<GameClient *> &otherClients) {
    if (this->currentZhCount >= this->zhCount) {
        printf("召唤结束\n");
        return false;
    }
    HWND hwnd = this->client->getHwnd();
    if (this->compareManager->compareValid(hwnd, "posui_zhaohuan")) {
        CompareLocation cl = compareManager->getCompareLocation("posui_zhaohuan");
        rangeMouseLbClick(hwnd, cl.x, cl.y, cl.x + cl.w, cl.y + cl.h);
        this->currentZhCount++;
        printf("当前自动召唤(%d/%d)\n", this->currentZhCount, this->zhCount);
    }
    return true;
}

int SpZhaohuanTask::getGroupTaskId() {
    return 0;
}

int SpZhaohuanTask::getTaskId() {
    return 0;
}

SpZhaohuanTask::SpZhaohuanTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager)
        : BaseGameTask(client, compareManager) {
    SpZhaohuanTask::initConfig(configJsonStr, [this](auto &&PH1) {
        initConfigCallback(std::forward<decltype(PH1)>(PH1));
    });
    this->currentZhCount = 0;
}

GameTask *
SpZhaohuanTask::createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager) {
    return new SpZhaohuanTask(configJsonStr, client, compareManager);
}

void SpZhaohuanTask::initConfigCallback(Json::Value *configJson) {
    this->zhCount = configJson->operator[]("zhaohuanCount").asInt();
}
