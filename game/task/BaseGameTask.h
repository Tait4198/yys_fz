#ifndef YYS_FZ_BASEGAMETASK_H
#define YYS_FZ_BASEGAMETASK_H

#include "GameTask.h"
#include <json/json.h>
#include <functional>

class BaseGameTask : public GameTask {
public:
    BaseGameTask(GameClient *client, CompareManager *compareManager);
protected:
    static void initConfig(const std::string& configJsonStr, const std::function<void(Json::Value*)>& callback);

    bool openBuff(std::vector<int>& buff);
};


#endif //YYS_FZ_BASEGAMETASK_H
