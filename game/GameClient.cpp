#include "GameClient.h"

using namespace std;

GameClient::GameClient(HWND hwnd, std::string hexHwnd) {
    this->hwnd = hwnd;
    this->hexHwnd = std::move(hexHwnd);
    this->currentPosition = -1;
    this->currentTaskId = -1;
    this->clientStatus = 1;
    this->initClientSize();
}

GameClient::~GameClient()
= default;

HWND &GameClient::getHwnd() {
    return this->hwnd;
}

void GameClient::initClientSize() {
    RECT rect;
    GetWindowRect(this->hwnd, &rect);
    SetWindowPos(this->hwnd, nullptr, rect.left, rect.top, 1152, 679, SWP_SHOWWINDOW);
}

std::string GameClient::getHexHwnd() {
    return this->hexHwnd;
}

int GameClient::getCurrentTaskId() const {
    return this->currentTaskId;
}

void GameClient::setCurrentTaskId(int taskId) {
    this->currentTaskId = taskId;
}

int GameClient::getCurrentPosition() const {
    return this->currentPosition;
}

void GameClient::setCurrentPosition(int position) {
    this->currentPosition = position;
}

std::string GameClient::getCurrentGroup() const {
    return this->group;
}

void GameClient::setCurrentGroup(std::string newGroup) {
    this->group = std::move(newGroup);
}

bool GameClient::isRun() {
    return this->run.load();
}

void GameClient::setRun(bool newStatus) {
    this->run.store(newStatus);
}

int GameClient::getClientStatus() const {
    return this->clientStatus;
}

void GameClient::setClientStatus(int newClientStatus) {
    this->clientStatus = newClientStatus;
}
