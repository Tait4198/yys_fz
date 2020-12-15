#pragma once

#include "Windows.h"
#include "windef.h"
#include <vector>
#include <map>
#include <set>
#include <string>
#include "GameClient.h"
#include "GameCommon.h"

class FzManager {
public:

    FzManager();

    ~FzManager();

    int updateClients();

    std::vector<std::string> getHexHwnds();

    HWND getHwndByHexHwnd(std::string &&hexHwnd);

    CompareResult compare(HWND &hwnd, std::string &&cpName);

    CompareResult compare(std::string &&hexHwnd, std::string &&cpName);

private:
    std::map<std::string, GameClient*> clientMap;
    std::map<std::string, GameCompare> cpMap;

    void initCpMap();

    int cleanClients(bool cleanAll);
};