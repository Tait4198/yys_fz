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
        printf("task init fail");
    }
    delete reader;
}

BaseGameTask::BaseGameTask(GameClient *client, CompareManager *compareManager) : GameTask(client, compareManager) {
}

bool BaseGameTask::openBuff(std::vector<int> &buff) {

}
