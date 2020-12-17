#include "CommonTask.h"

#include <utility>
#include "../GameUtil.h"

void CommonTask::exec(GameClient *client, std::vector<int>& otherClientTaskIds) {
    this->offerAReward(client, false);
}

int CommonTask::getTaskId() {
    return 0;
}

void CommonTask::offerAReward(GameClient *client, bool accept) {
    CompareResult cr = client->compare("check_01");
    if (cr.pv <= cr.r) {
        if (accept) {
            CompareResult aCr = client->compare("accept_01");
            if (aCr.pv <= aCr.r) {
                rangeMouseLbClick(client->getHwnd(), aCr.x, aCr.y, aCr.x + aCr.w, aCr.y + aCr.h);
            }
        } else {
            CompareResult rCr = client->compare("refuse_01");
            if (rCr.pv <= rCr.r) {
                rangeMouseLbClick(client->getHwnd(), rCr.x, rCr.y, rCr.x + rCr.w, rCr.y + rCr.h);
            }
        }
    }
}

int CommonTask::getGroupTaskId() {
    return -1;
}

GameTask *CommonTask::createInstance(std::string configJsonStr) {
    return new CommonTask(std::move(configJsonStr));
}

CommonTask::CommonTask(const std::string& configJsonStr) {

}

CommonTask::~CommonTask() = default;
