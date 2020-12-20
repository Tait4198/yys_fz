#pragma once

#include "Windows.h"
#include "windef.h"
#include "string"
#include "atomic"

class GameClient {
public:


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

    [[nodiscard]] int getClientStatus() const;

    void setClientStatus(int clientStatus);

    bool isRun();

    void setRun(bool status);

private:
    HWND hwnd;
    std::string hexHwnd;
    std::atomic<bool> run;

    int clientStatus;
    int currentPosition;
    int currentTaskId;
    std::string group;

    void initClientSize();
};

