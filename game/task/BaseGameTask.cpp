#include "BaseGameTask.h"

void BaseGameTask::initConfig(const std::string &configJsonStr, const std::function<void(Json::Value *)> &callback) {
    Json::CharReaderBuilder b;
    Json::CharReader *reader(b.newCharReader());
    Json::Value configJson;
    JSONCPP_STRING errs;
    bool success = reader->parse(configJsonStr.c_str(), configJsonStr.c_str() + strlen(configJsonStr.c_str()),
                                 &configJson, &errs);
    if (success && errs.empty()) {
        callback(&configJson);
    } else {
        printf("task init fail\n");
    }
    delete reader;
}

BaseGameTask::BaseGameTask(GameClient *client, CompareManager *compareManager) : GameTask(client, compareManager) {
}

bool BaseGameTask::openBuff(std::vector<int> &buff) {
    CompareLocation buffCl = this->compareManager->getCompareLocation("buff_01");
    rangeMouseLbClick(client->getHwnd(), buffCl.x, buffCl.y, buffCl.x + buffCl.w, buffCl.y + buffCl.h);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // 检测buff界面是否打开
    if (this->compareManager->compareValid(client->getHwnd(), client->getCurrentPosition(), "buff_02")) {
        return false;
    }
    return true;
}
