#pragma once
#include "GameClient.h"

class GameTask
{
public:
	virtual void exec(GameClient* client) = 0;
	virtual int getGroupTaskId() = 0;
	virtual int getTaskId() = 0;
	virtual ~GameTask() = default;
};