#include "SpTupoBattleTask.h"
#include <regex>

SpTupoBattleTask::SpTupoBattleTask(const std::string &configJsonStr, GameClient *client,
                                   CompareManager *compareManager) : BaseGameTask(client, compareManager) {
    SpTupoBattleTask::initConfig(configJsonStr, [this](auto &&PH1) {
        initConfigCallback(std::forward<decltype(PH1)>(PH1));
    });
    this->initCheck = false;

    this->currentBattleCount = 0;
}

GameTask *
SpTupoBattleTask::createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager) {
    return new SpTupoBattleTask(configJsonStr, client, compareManager);
}

bool SpTupoBattleTask::exec(std::vector<GameClient *> &otherClients) {
    if (this->currentBattleCount >= this->battleCount) {
        printf("本轮战斗结束\n");
        return false;
    }

    HWND hwnd = client->getHwnd();
    if (!initCheck) {
        this->initCheck = true;
        // 检测是否在突破界面
        if (!this->compareManager->compareValid(hwnd, "tupo_in_room")) {
            printf("未处于突破界面\n");
            return false;
        }
    }
    // 等待获取突破券数量
    if (getRemainingTupoSize() == 0) {
        printf("没有可用的突破券\n");
        return false;
    }

    bool battleStart = false;
    for (auto &iter : this->compareManager->getCpMap()) {
        if (iter.first.find("tupo_check_") != std::string::npos) {
            GameCompare cp = iter.second;
            if (this->compareManager->compareValid(hwnd, 0, cp)) {
                rangeMouseLbClick(hwnd, cp.x, cp.y, cp.x + cp.w, cp.y + cp.h);
                std::this_thread::sleep_for(std::chrono::seconds(1));

                cv::Mat sMat = getScreenshotMat(hwnd);
                std::vector<TextBlock> tbs = this->compareManager->getOcrLite()->detect(sMat);
                for (auto &tb:tbs) {
                    if (tb.text == utf8Encode(L"进攻")) {
                        int sx, sy, ex, ey;
                        setClickXY(tb.boxPoint, &sx, &sy, &ex, &ey);
                        printf("进攻按钮位置 %d %d %d %d\n", sx, sy, ex, ey);
                        rangeMouseLbClick(hwnd, sx, sy, ex, ey);
                        battleStart = true;
                        break;
                    }
                }
                break;
            }
        }
    }

    if (battleStart) {
        printf("突破战斗开始\n");
        int waitBattleIndex = 0;
        while (!this->compareManager->compareValid(hwnd, "tupo_back")) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            // 超过10秒未检测进入战斗
            if (waitBattleIndex++ > 2 * 10) {
                return true;
            }
        }
        // 标记最左
        // todo 标记校验
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        printf("标记最左\n");
        rangeMouseLbClick(hwnd, 80, 380, 100, 390);

        std::mt19937 rng{this->randomDev()};
        std::uniform_int_distribution<std::mt19937::result_type> dis(1000, 3000);
        std::uniform_int_distribution<std::mt19937::result_type> disClick(1, 3);

        // 等待战斗结束
        while (true) {
            if (!this->compareManager->compareValid(hwnd, "tupo_back")) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                rangeMouseLbClick(hwnd, 1055, 450, 1130, 600, disClick(rng));
                std::this_thread::sleep_for(std::chrono::milliseconds(dis(rng)));
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        // 等待返回突破选择页面
        while (!this->compareManager->compareValid(hwnd, "tupo_in_room")) {
            rangeMouseLbClick(hwnd, 1055, 450, 1130, 600, disClick(rng));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        // 可能出现攻破奖励
        rangeMouseLbClick(hwnd, 1055, 450, 1130, 600, disClick(rng));

        // 更新战斗次数
        this->currentBattleCount++;
    } else {
        // 等待检测刷新按钮是否可用
        printf("等待刷新突破目标\n");
        while (!this->compareManager->compareValid(hwnd, "tupo_refresh")) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        printf("刷新突破目标\n");
        CompareLocation refreshCl = this->compareManager->getCompareLocation("tupo_refresh");
        rangeMouseLbClick(hwnd, refreshCl.x, refreshCl.y, refreshCl.x + refreshCl.w, refreshCl.y + refreshCl.h);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //todo 提示校验
        this->compareManager->checkModal(hwnd, true, nullptr);
    }
    return true;
}

int SpTupoBattleTask::getRemainingTupoSize() {
    cv::Mat tpSizeMat = getScreenshotMat(this->client->getHwnd(), 910, 0, 105, 55);
    std::vector<TextBlock> tbs = this->compareManager->getOcrLite()->detect(tpSizeMat);
    if (tbs.size() != 1 || !std::regex_match(tbs[0].text, std::regex("\\d{1,2}/\\d{1,2}"))) {
        printf("突破券数量获取失败\n");
        return -1;
    }
    int tpSize = std::strtol(tbs[0].text.substr(0, tbs[0].text.find('/')).c_str(),
                             nullptr, 10);
    printf("剩余突破券 %d\n", tpSize);
    return tpSize;
}

void SpTupoBattleTask::setClickXY(std::vector<cv::Point> &points, int *sx, int *sy, int *ex, int *ey) {
    *sx = 9999;
    *sy = 9999;
    *ex = 0;
    *ey = 0;
    for (auto &p :points) {
        if (p.x < *sx) {
            *sx = p.x;
        }
        if (p.x > *ex) {
            *ex = p.x;
        }
        if (p.y < *sy) {
            *sy = p.y;
        }
        if (p.y > *ey) {
            *ey = p.y;
        }
    }
}

SpTupoBattleTask::~SpTupoBattleTask() = default;

int SpTupoBattleTask::getGroupTaskId() {
    return 0;
}

void SpTupoBattleTask::initConfigCallback(Json::Value *configJson) {
    this->battleCount = configJson->operator[]("battleCount").asInt();
}

int SpTupoBattleTask::getTaskId() {
    return 0;
}
