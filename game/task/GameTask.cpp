#include "GameTask.h"

GameTask::GameTask(GameClient *client, CompareManager *compareManager) {
    this->client = client;
    this->compareManager = compareManager;
}
