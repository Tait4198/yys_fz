#include "FzManager.h"
#include <filesystem>
#include <fstream>
#include <json/json.h>

using namespace std;
namespace fs = filesystem;

FzManager fzManager;

FzManager::FzManager() {
    system("chcp 65001");
    // CpMap初始化要先于clientMap
    this->initCpMap();
    this->ocrLite = new OcrLite(4);
    this->ocrLite->initLogger(true, false, true);
    this->ocrLite->initModels("./models");

    this->gameTaskManager = new GameTaskManager;
    this->groupManager = new GroupManager;

    // 客户端必须最后初始化
    this->updateClients();
}

FzManager::~FzManager() {
    this->cleanClients(true);
    delete this->ocrLite;
}


int FzManager::updateClients() {
    vector<HWND> hwnds = findYysHwnds();
    int newHwnd = -this->cleanClients(false);
    for (auto &hwnd : hwnds) {
        stringstream ss;
        ss << "0x" << hwnd;
        if (!this->clientMap.count(ss.str())) {
            this->clientMap[ss.str()] = new GameClient(hwnd, ss.str(), &this->cpMap, this->ocrLite,
                                                       this->gameTaskManager, this->groupManager);
            this->clientMap[ss.str()]->backToHome();
//            this->clientMap[ss.str()]->execTask();
//            this->clientMap[ss.str()]->checkModal(true, &callback);
            newHwnd++;
        }
    }
    return newHwnd;
}

HWND FzManager::getHwndByHexHwnd(string &&hexHwnd) {
    if (this->clientMap.count(hexHwnd)) {
        return this->clientMap[hexHwnd]->getHwnd();
    }
    return nullptr;
}

CompareResult FzManager::compare(HWND &hwnd, std::string &&cpName) {
    stringstream ss;
    ss << "0x" << hwnd;
    if (this->clientMap.count(ss.str())) {
        return this->clientMap[ss.str()]->compare(move(cpName));
    }
    return CompareResult{0, 0, 0, 0, 0, -1, ""};
}

CompareResult FzManager::compare(std::string &&hexHwnd, std::string &&cpName) {
    std::string iHexHwnd = forward<std::string>(hexHwnd);
    if (this->clientMap.count(iHexHwnd)) {
        return this->clientMap[iHexHwnd]->compare(move(cpName));
    }
    return CompareResult{0, 0, 0, 0, 0, -1, ""};
}

vector<string> FzManager::getHexHwnds() {
    vector<string> hwnds;
    for (auto &iter : this->clientMap) {
        hwnds.push_back(iter.first);
    }
    return hwnds;
}


void FzManager::initCpMap() {
    stringstream ss;
    ss << getExePath() << "\\cp_json";
    if (!dirExists(ss.str())) {
        printf("cp_json 目录不存在\n");
        return;
    }
    Json::CharReaderBuilder b;
    Json::CharReader *reader(b.newCharReader());
    for (const auto &entry : fs::directory_iterator(ss.str())) {
        if (entry.path().extension() == ".json") {
            ifstream ifs(entry.path());
            string jsonStr((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
            Json::Value cpJson;
            JSONCPP_STRING errs;
            bool success = reader->parse(jsonStr.c_str(), jsonStr.c_str() + strlen(jsonStr.c_str()), &cpJson, &errs);
            if (success && errs.empty()) {
                string filename = entry.path().filename().string();
                string cpName = filename.substr(0, filename.find_last_of('.'));
                int r = cpJson["r"].asInt();
                std::set<int> pSet;
                int size = cpJson["position"].size();
                for (int i = 0; i < size; i++) {
                    pSet.insert(cpJson["position"][i].asInt());
                }
                this->cpMap[cpName] = GameCompare{cpJson["x"].asInt(), cpJson["y"].asInt(), cpJson["w"].asInt(),
                                                  cpJson["h"].asInt(),
                                                  pSet, r ? r : 3, cpJson["hash"].asString(),
                                                  cpJson["name"].asString()};
            }
        }
    }
    delete reader;
}

int FzManager::cleanClients(bool cleanAll) {
    int cleanCount = 0;
    for (auto &iter : this->clientMap) {
        if (!IsWindow(iter.second->getHwnd()) || cleanAll) {
            delete iter.second;
            this->clientMap.erase(iter.first);
            cleanCount++;
        }
    }
    return cleanCount;
}
