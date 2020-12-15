#pragma once
#include "GameTask.h"

class CommonTask : public GameTask
{
public:
	void offerAReward(GameClient* client, bool accept);
public:
	void exec(GameClient* client) override;
	~CommonTask() override;
	int getTaskId() override;
	int getGroupTaskId() override;
};

