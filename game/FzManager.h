#pragma once

#include "Windows.h"
#include "windef.h"
#include "GameCommon.h"
#include "../ocr/OcrLite.h"

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
    std::map<std::string, GameClient *> clientMap;
    std::map<std::string, GameCompare> cpMap;
    OcrLite *ocrLite;
    GameTaskManager *gameTaskManager;
    GroupManager* groupManager;

    void initCpMap();

    int cleanClients(bool cleanAll);
};