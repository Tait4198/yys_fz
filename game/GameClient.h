#pragma once

#include "Windows.h"
#include "windef.h"
#include "string"
#include "atomic"

class GameClient {
public:
    struct ClientStatus {
        bool inRoom;
        bool inBattle;
    };

    GameClient(HWND hwnd, std::string);

    ~GameClient();

    HWND &getHwnd();

    std::string getHexHwnd();

    [[nodiscard]] int getCurrentTaskId() const;

    void setCurrentTaskId(int taskId);

    [[nodiscard]] int getCurrentPosition() const;

    void setCurrentPosition(int position);

    [[nodiscard]] std::string getCurrentGroup() const;

    void setCurrentGroup(std::string newGroup);

    bool isRun();

    void setRun(bool status);

    ClientStatus* getClientStatus();

private:
    HWND hwnd;
    std::string hexHwnd;
    std::atomic<bool> run;

    // 全自动时用于定位页面用
    int currentPosition;
    int currentTaskId;
    std::string group;

    ClientStatus clientStatus;

    void initClientSize();
};

