#include "YuHunTask.h"

YuHunTask::YuHunTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager)
        : BaseGameTask(client, compareManager) {
    YuHunTask::initConfig(configJsonStr, [this](auto &&PH1) {
        initConfigCallback(std::forward<decltype(PH1)>(PH1));
    });
    this->init = false;
    this->enterLoop = false;
}

GameTask *
YuHunTask::createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager) {
    return new YuHunTask(configJsonStr, client, compareManager);
}

bool YuHunTask::exec(std::vector<GameClient *> &otherClients) {
    if (this->init) {
        return false;
    } else {
        // 未完成初始化准备
        if (!this->compareManager->backToHome(this->client->getHwnd(), this->client->getHexHwnd())) {
            // 无法退回首页
            return false;
        } else {
            // 已返回主页
            this->client->setCurrentPosition(100);
        }
        // 开启buff
        if (!this->buff.empty()) {
            this->openBuff(this->buff);
        }
        if (otherClients.empty()) {
            // 单人
            if (!this->initSingleBattle()) {
                return false;
            }
        } else {
            // 组队
            if (!this->initGroupBattle()) {
                return false;
            }
        }
        this->init = true;
    }
    return true;
}

void YuHunTask::initConfigCallback(Json::Value *configJson) {
    this->battleCount = configJson->operator[]("battleCount").asInt();
    if (configJson->isMember("buff")) {
        Json::Value buffValue = configJson->operator[]("buff");
        for (const auto &i : buffValue) {
            this->buff.push_back(i.asInt());
        }
    }

}

YuHunTask::~YuHunTask() = default;

int YuHunTask::getTaskId() {
    return 0;
}

int YuHunTask::getGroupTaskId() {
    return 0;
}

bool YuHunTask::initSingleBattle() {
    if (!this->compareManager->compareValid(this->client->getHwnd(),
                                            this->client->getCurrentPosition(),
                                            "home_expanded")) {
        // 菜单未展开
    }
    return true;
}

bool YuHunTask::initGroupBattle() {
    return true;
}


