#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include "GameClient.h"
#include "GameUtil.h"
#include "GroupManager.h"
#include "task/GameTaskManager.h"

struct GameCompare {
    int x;
    int y;
    int w;
    int h;
    std::set<int> position;
    int r;
    std::string hash;
    std::string name;
};

struct CompareResult {
    int x;
    int y;
    int w;
    int h;
    int r;
    int pv;
    std::string name;
};

struct CompareLocation {
    int x;
    int y;
    int w;
    int h;
    bool valid;
};