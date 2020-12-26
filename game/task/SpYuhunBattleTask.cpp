#include "SpYuhunBattleTask.h"

SpYuhunBattleTask::SpYuhunBattleTask(const std::string &configJsonStr, GameClient *client,
                                     CompareManager *compareManager) : BaseGameTask(client, compareManager) {
    SpYuhunBattleTask::initConfig(configJsonStr, [this](auto &&PH1) {
        initConfigCallback(std::forward<decltype(PH1)>(PH1));
    });
    this->initCheck = false;
    this->isLeader = false;

    this->currentBattleCount = 0;
    this->battleExecCount = 0;
    this->battleFailExecCount = 0;
}

GameTask *
SpYuhunBattleTask::createInstance(const std::string &configJsonStr, GameClient *client,
                                  CompareManager *compareManager) {
    return new SpYuhunBattleTask(configJsonStr, client, compareManager);
}

bool SpYuhunBattleTask::exec(std::vector<GameClient *> &otherClients) {
    if (this->currentBattleCount >= this->battleCount) {
        printf("本轮战斗结束\n");
        return false;
    }
    HWND hwnd = client->getHwnd();
    if (!initCheck) {
        this->initCheck = true;
        // 检测是否在协战队伍
        if (!this->compareManager->compareValid(hwnd, "yuhun_in_room")) {
            printf("未处于协战队伍\n");
            return false;
        }
        // 检测是否队长
        this->isLeader = compareManager->compareValid(hwnd, "yuhun_leader_ready");
        if (this->isLeader) {
            printf("队长确认\n");
        }
        this->client->getClientStatus()->inRoom = true;
    }

    std::stringstream ss;
    if (this->isLeader) {
        ss << "队长[" << hwnd << "]";
    } else {
        ss << "队员[" << hwnd << "]";
    }

    // 战斗开始检测
    if (this->client->getClientStatus()->inRoom) {
        if (this->isLeader) {
            bool allJoin = true;
            for (auto gc : otherClients) {
                if (!gc->getClientStatus()->inRoom) {
                    allJoin = false;
                }
            }
            if (allJoin && compareManager->compareValid(hwnd, "yuhun_leader_ready")) {
                CompareLocation readyCl = compareManager->getCompareLocation("yuhun_leader_ready");
//                rangeMouseLbClick(hwnd, readyCl.x, readyCl.y, readyCl.x + readyCl.w, readyCl.y + readyCl.h);
                for (auto gc : otherClients) {
                    gc->getClientStatus()->inBattle = true;
                    gc->getClientStatus()->inRoom = true;
                }
                this->client->getClientStatus()->inBattle = true;
                this->client->getClientStatus()->inRoom = false;
                this->battleExecCount = 1;
                printf("%s第(%d/%d)把御魂开始\n", ss.str().c_str(), this->currentBattleCount + 1, this->battleCount);
            } else {
                printf("等待队友加入\n");
            }
        } else {
            // 仅队长控制才会出现的状态
            if (this->client->getClientStatus()->inRoom && this->client->getClientStatus()->inBattle) {
                this->client->getClientStatus()->inBattle = true;
                this->client->getClientStatus()->inRoom = false;
                this->battleExecCount = 1;
                printf("%s第(%d/%d)把御魂开始\n", ss.str().c_str(), this->currentBattleCount + 1, this->battleCount);
            } else {
                printf("等待队长更新\n");
            }
        }
    } else {
        std::mt19937 rng{this->randomDev()};
        std::uniform_int_distribution<std::mt19937::result_type> dis(1000, 3000);
        std::uniform_int_distribution<std::mt19937::result_type> disClick(1, 3);

        if (compareManager->compareValid(hwnd, "yuhun_leader_end_01") ||
            compareManager->compareValid(hwnd, "yuhun_player_end_01") ||
            compareManager->compareValid(hwnd, "yuhun_player_end_02")) {
            printf("%s战斗结束\n", ss.str().c_str());
            std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng)));
            rangeMouseLbClick(hwnd, 650, 395, 1120, 500, disClick(rng));
            std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng)));
        }
        if (compareManager->compareValid(hwnd, "yuhun_end_01")) {
            printf("%s结算结束\n", ss.str().c_str());
            while (!this->compareManager->compareValid(hwnd, "yuhun_in_room")) {
                rangeMouseLbClick(hwnd, 950, 200, 1120, 600, disClick(rng));
                std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng)));
            }
            printf("%s加入房间\n", ss.str().c_str());
            this->battleExecCount = 0;
            this->battleFailExecCount = 0;
            this->currentBattleCount++;

            this->client->getClientStatus()->inRoom = true;
            this->client->getClientStatus()->inBattle = false;
        }
    }
    if (this->battleExecCount > 75) {
        rangeMouseLbClick(hwnd, 950, 370, 1120, 600);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        if (this->battleFailExecCount++ > 3) {
            printf("状态出错\n");
            return false;
        }
    } else {
        this->battleExecCount++;
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