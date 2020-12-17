//
// Created by 2020/12/17.
//

#ifndef YYS_FZ_BASEGAMETASK_H
#define YYS_FZ_BASEGAMETASK_H

#include "GameTask.h"
#include <json/json.h>
#include <functional>

class BaseGameTask : public GameTask {
protected:
    static void initConfig(const std::string& configJsonStr, const std::function<void(Json::Value*)>& callback);
};


#endif //YYS_FZ_BASEGAMETASK_H
