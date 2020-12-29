#ifndef YYS_FZ_SPTUPOBATTLETASK_H
#define YYS_FZ_SPTUPOBATTLETASK_H

#include "BaseGameTask.h"
#include <random>

class SpTupoBattleTask : public BaseGameTask {
public:
    explicit SpTupoBattleTask(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);
public:
    static GameTask *
    createInstance(const std::string &configJsonStr, GameClient *client, CompareManager *compareManager);

    bool exec(std::vector<GameClient *> &otherClients) override;

    ~SpTupoBattleTask() override;

    int getTaskId() override;

    int getGroupTaskId() override;

private:
    std::random_device randomDev;

    bool initCheck;
    int currentBattleCount;
    // 突破券数量
    int currentTupoCount;

    int battleCount{};

    void initConfigCallback(Json::Value *configJson);

    int getRemainingTupoSize();

    static void setClickXY(std::vector<cv::Point> &points, int *sx, int *sy, int *ex, int *ey);
};


#endif //YYS_FZ_SPTUPOBATTLETASK_H
