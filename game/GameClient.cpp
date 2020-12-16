#include "GameClient.h"
#include "GameUtil.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <utility>
#include "AllGameTask.h"

using namespace std;

GameClient::GameClient(HWND hwnd, string hexHwnd, std::map<std::string, GameCompare> *cpMapPtr, OcrLite *ocrLite) {
    this->hwnd = hwnd;
    this->hexHwnd = std::move(hexHwnd);
    this->cpMapPtr = cpMapPtr;
    this->ocrLite = ocrLite;
    this->currentPosition = -1;
    this->currentTaskId = -1;
    this->initClientSize();
}

GameClient::~GameClient()
= default;

HWND GameClient::getHwnd() {
    return this->hwnd;
}

CompareResult GameClient::compare(std::string &&cpName) {
    return compare(move(cpName), false);
}

CompareResult GameClient::compare(std::string &&cpName, bool missPosition) {
    string iCpName = forward<string>(cpName);
    if (this->cpMapPtr->count(iCpName) == 0) {
        printf("无效CpName -> %s\n", iCpName.c_str());
        return CompareResult{0, 0, 0, 0, 0, -1, ""};
    }
    GameCompare cp = this->cpMapPtr->operator[](iCpName);
    return this->inCompare(cp, missPosition);
}

void GameClient::initClientSize() {
    RECT rect;
    GetWindowRect(this->hwnd, &rect);
    SetWindowPos(this->hwnd, nullptr, rect.left, rect.top, 1152, 679, SWP_SHOWWINDOW);
}

CompareResult GameClient::inCompare(GameCompare &cp, bool missPosition) {
    if (false && !missPosition && cp.position.find(0) == cp.position.end() &&
        !(cp.position.find(this->currentPosition) != cp.position.end())) {
        std::ostringstream ss;
        std::copy(cp.position.begin(), cp.position.end(), std::ostream_iterator<int>(ss, ","));
        printf("无效Position: %d -> %s\n", this->currentPosition, ss.str().c_str());
        return CompareResult{0, 0, 0, 0, 0, -1, ""};
    }
    string hash = getScreenshotPHash(hwnd, cp.x, cp.y, cp.w, cp.h);
    int pv = hammingDistance(hash, cp.hash);
    printf("N %s C %d H %s\n", cp.name.c_str(), pv, hash.c_str());
    return CompareResult{cp.x, cp.y, cp.w, cp.h, cp.r, pv, cp.name};
}

bool GameClient::inCompareValid(GameCompare &cp, bool missPosition) {
    int cpv = inCompare(cp, missPosition).pv;
    printf("N %s R %d C %d\n", cp.name.c_str(), cp.r, cpv);
    return cpv <= cp.r;
}

void GameClient::threadFunc() {
    GameTask *gt = new CommonTask;
    while (true) {
        for (int i = 0; i < 60; i++) {
            printf("Exec %d\n", i);
            gt->exec(this);
            this_thread::sleep_for(chrono::seconds(1));
        }
        break;
    }
    delete gt;
}


bool GameClient::backToHome() {
    if (onTheHome()) {
        printf("[%s]已在首页\n", this->hexHwnd.c_str());
        return true;
    }
    int curBack = 1;
    while (curBack <= 15) {
        printf("[%s]返回首页中第[%d]次\n", this->hexHwnd.c_str(), curBack);
        bool wait = false;
        for (auto &iter : *this->cpMapPtr) {
            if (iter.first.find("back_") != string::npos) {
                GameCompare cp = iter.second;
                if (inCompareValid(cp, true)) {
                    rangeMouseLbClick(this->hwnd, cp.x, cp.y, cp.x + cp.w, cp.y + cp.h);
                    this_thread::sleep_for(chrono::seconds(1));
                    checkModal(true, nullptr);
                    wait = true;
                    break;
                }
            }
        }
        if (wait) {
            this_thread::sleep_for(chrono::seconds(2));
            if (onTheHome()) {
                printf("[%s]已返回首页\n", this->hexHwnd.c_str());
                return true;
            }
        }
        curBack++;
    }
    printf("[%s]无法返回首页\n", this->hexHwnd.c_str());
    return false;
}

void GameClient::execTask() {
    thread t1(&GameClient::threadFunc, this);
    t1.detach();
}

bool GameClient::onTheHome() {
    return (inCompareValid(this->cpMapPtr->operator[]("home_not_expanded"), true)
            || inCompareValid(this->cpMapPtr->operator[]("home_expanded"), true)) &&
           inCompareValid(this->cpMapPtr->operator[]("buff_01"), true);
}

void GameClient::checkModal(bool accept, void(*checkModalCallback)(bool, GameClient *)) {
    CompareResult aCr = compare("accept_02");
    CompareResult rCr = compare("refuse_02");
    if (aCr.pv <= aCr.r && rCr.pv <= rCr.r) {
        if (checkModalCallback != nullptr) {
            checkModalCallback(accept, this);
        }
        if (accept) {
            rangeMouseLbClick(this->hwnd, aCr.x, aCr.y, aCr.x + aCr.w, aCr.y + aCr.h);
        } else {
            rangeMouseLbClick(this->hwnd, rCr.x, rCr.y, rCr.x + rCr.w, rCr.y + rCr.h);
        }
    }
}

OcrLite *GameClient::getOcrLite() {
    return this->ocrLite;
}

CompareLocation GameClient::getCompareLocation(string &&cpName) {
    string iCpName = forward<string>(cpName);
    if (this->cpMapPtr->count(iCpName) == 0) {
        return CompareLocation{0, 0, 0, 0, false};
    }
    GameCompare cp = this->cpMapPtr->operator[](iCpName);
    return CompareLocation{cp.x, cp.y, cp.w, cp.h, true};
}
