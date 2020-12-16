#pragma once

#include "Windows.h"
#include "windef.h"
#include <map>
#include <string>
#include "GameCommon.h"
#include "../ocr/OcrLite.h"


class GameClient {
public:
    GameClient(HWND hwnd, std::string hexHwnd, std::map<std::string, GameCompare> *cpMapPtr, OcrLite *ocrLite);

    ~GameClient();

    HWND getHwnd();

    CompareResult compare(std::string &&cpName);

    CompareResult compare(std::string &&cpName, bool missPosition);

    void checkModal(std::string &&message, bool accept, void(*CheckModalCallback)(std::string &, bool, GameClient *));

    bool backToHome();

    void execTask();

    OcrLite *getOcrLite();

    CompareLocation getCompareLocation(std::string &&cpName);

private:
    HWND hwnd;
    std::string hexHwnd;
    std::map<std::string, GameCompare> *cpMapPtr;
    OcrLite *ocrLite;

    int currentPosition;
    int currentTaskId;
    std::string group;

    void initClientSize();

    CompareResult inCompare(GameCompare &cp, bool missPosition);

    bool inCompareValid(GameCompare &cp, bool missPosition);

    bool onTheHome();

    void threadFunc();
};

