#include "SpYuhunBattleTask.h"

SpYuhunBattleTask::SpYuhunBattleTask(const std::string &configJsonStr, GameClient *client,
                                     CompareManager *compareManager) : BaseGameTask(client, compareManager) {
    SpYuhunBattleTask::initConfig(configJsonStr, [this](auto &&PH1) {
        initConfigCallback(std::forward<decltype(PH1)>(PH1));
    });
    this->curBattleCount = 0;
    this->battleExecCount = 0;
    this->isLeader = false;
}

GameTask *
SpYuhunBattleTask::createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager) {
    return new SpYuhunBattleTask(configJsonStr, client, compareManager);
}

bool SpYuhunBattleTask::exec(std::vector<GameClient *> &otherClients) {
    if (this->curBattleCount >= this->battleCount) {
        printf("本轮战斗结束\n");
        return false;
    }
    int position = client->getCurrentPosition();
    HWND hwnd = client->getHwnd();
    // 准备监听
    if (compareManager->compareValid(hwnd, position, "yuhun_leader_ready")) {
        if (!compareManager->compareValid(hwnd, position, "player_01_not_join")) {
            CompareLocation readyCl = compareManager->getCompareLocation("yuhun_leader_ready");
            rangeMouseLbClick(hwnd, readyCl.x, readyCl.y, readyCl.x + readyCl.w, readyCl.y + readyCl.h);
            this->battleExecCount = 1;
            this->isLeader = true;
            this->client->setClientStatus(1);
            printf("队长第(%d/%d)把御魂开始\n", this->curBattleCount + 1, this->battleCount);
        }
    }
    if (!this->isLeader && this->battleExecCount == 0) {
        if (!otherClients.empty() && otherClients[0]->getClientStatus()) {
            this->battleExecCount = 1;
            this->client->setCurrentPosition(0);
            printf("队员第(%d/%d)把御魂开始\n", this->curBattleCount + 1, this->battleCount);
        }
    }

    // 单把战斗超过75
    if (this->battleExecCount > 75) {
        rangeMouseLbClick(hwnd, 950, 370, 1120, 600);
    } else {
        this->battleExecCount++;
    }

    std::mt19937 rng{this->randomDev()};
    std::uniform_int_distribution<std::mt19937::result_type> dis(1000, 3000);
    std::uniform_int_distribution<std::mt19937::result_type> disClick(2, 3);
    if (compareManager->compareValid(hwnd, position, "yuhun_player_end_01") ||
        compareManager->compareValid(hwnd, position, "yuhun_leader_end_01") ||
        compareManager->compareValid(hwnd, position, "yuhun_player_end_02")) {
        printf("战斗结束\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng)));
        rangeMouseLbClick(hwnd, 650, 395, 1120, 500, disClick(rng));
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng)));
    }

    if (compareManager->compareValid(hwnd, position, "yuhun_end_01")) {
        while (compareManager->compareValid(hwnd, position, "yuhun_end_01")) {
            rangeMouseLbClick(hwnd, 950, 200, 1120, 600, disClick(rng));
            std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng)));
        }
        printf("结算结束\n");
        this->battleExecCount = 0;
        this->curBattleCount++;
        if (this->isLeader) {
            this->client->setClientStatus(0);
        } else {
            this->client->setClientStatus(1);
        }
    }
    return true;
}

SpYuhunBattleTask::~SpYuhunBattleTask() = default;

int SpYuhunBattleTask::getTaskId() {
    return 0;
}

int SpYuhunBattleTask::getGroupTaskId() {
    return 0;
}

void SpYuhunBattleTask::initConfigCallback(Json::Value *configJson) {
    this->battleCount = configJson->operator[]("battleCount").asInt();
}
