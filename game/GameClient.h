#pragma once

#include "Windows.h"
#include "windef.h"
#include "GameCommon.h"
#include "../ocr/OcrLite.h"

class GameClient {
public:
    struct GameTaskParam {
        std::string configJson;
        std::string taskName;
    };

    GameClient(HWND, std::string, std::map<std::string, GameCompare> *,
               OcrLite *, JsonConvert *, GameTaskManager *, GroupManager *);

    ~GameClient();

    HWND getHwnd();

    std::string getHexHwnd();

    CompareResult compare(std::string &&cpName);

    CompareResult compare(std::string &&cpName, bool missPosition);

    void checkModal(bool accept, void(*CheckModalCallback)(bool, GameClient *));

    bool backToHome();

    void execTask(const std::string &configJson);

    OcrLite *getOcrLite();

    int getCurrentTaskId() const;

    CompareLocation getCompareLocation(std::string &&cpName);

private:
    HWND hwnd;
    std::string hexHwnd;
    std::map<std::string, GameCompare> *cpMapPtr;
    OcrLite *ocrLite;
    GameTaskManager *taskManager;
    GroupManager *groupManager;
    JsonConvert *jsonConvert;

    int currentPosition;
    int currentTaskId;
    std::string group;

    void initClientSize();

    CompareResult inCompare(GameCompare &cp, bool missPosition);

    bool inCompareValid(GameCompare &cp, bool missPosition);

    bool onTheHome();

    void taskFunc(std::vector<GameTaskParam> &);
};

