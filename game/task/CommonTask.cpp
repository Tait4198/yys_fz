#include "CommonTask.h"

CommonTask::CommonTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager)
        : GameTask(client, compareManager) {

}

bool CommonTask::exec(std::vector<int> &otherClientTaskIds) {
    this->offerAReward(false);
    return true;
}

int CommonTask::getTaskId() {
    return 0;
}

void CommonTask::offerAReward(bool accept) {
    HWND hwnd = this->client->getHwnd();
    CompareResult cr = this->compareManager->compare(hwnd, -1, "check_01");
    if (cr.pv <= cr.r) {
        if (accept) {
            CompareResult aCr = this->compareManager->compare(hwnd, -1, "accept_01");
            if (aCr.pv <= aCr.r) {
                rangeMouseLbClick(client->getHwnd(), aCr.x, aCr.y, aCr.x + aCr.w, aCr.y + aCr.h);
            }
        } else {
            CompareResult rCr = this->compareManager->compare(hwnd, -1, "refuse_01");
            if (rCr.pv <= rCr.r) {
                rangeMouseLbClick(client->getHwnd(), rCr.x, rCr.y, rCr.x + rCr.w, rCr.y + rCr.h);
            }
        }
    }
}

int CommonTask::getGroupTaskId() {
    return -1;
}

GameTask *
CommonTask::createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager) {
    return new CommonTask(configJsonStr, client, compareManager);
}

CommonTask::~CommonTask() = default;
