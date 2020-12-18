#include "CompareManager.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;

CompareManager::CompareManager(JsonConvert *jsonConvert) {
    this->jsonConvert = jsonConvert;
    this->ocrLite = new OcrLite(4);
    this->ocrLite->initLogger(true, false, true);
    this->ocrLite->initModels("./models");

    this->initCpMap();
}

void CompareManager::initCpMap() {
    std::stringstream ss;
    ss << getExePath() << "\\cp_json";
    if (!dirExists(ss.str())) {
        printf("cp_json 目录不存在\n");
        return;
    }

    for (const auto &entry : fs::directory_iterator(ss.str())) {
        if (entry.path().extension() == ".json") {
            std::ifstream ifs(entry.path());
            std::string jsonStr((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
            Json::Value cpJson;
            if (this->jsonConvert->convert(jsonStr, &cpJson)) {
                std::string filename = entry.path().filename().string();
                std::string cpName = filename.substr(0, filename.find_last_of('.'));
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
}

std::map<std::string, GameCompare> &CompareManager::getCpMap() {
    return this->cpMap;
}

CompareLocation CompareManager::getCompareLocation(std::string &&cpName) {
    std::string iCpName = std::forward<std::string>(cpName);
    if (this->cpMap.count(iCpName) == 0) {
        return CompareLocation{0, 0, 0, 0, false};
    }
    GameCompare cp = this->cpMap[iCpName];
    return CompareLocation{cp.x, cp.y, cp.w, cp.h, true};
}

CompareResult CompareManager::compare(HWND hwnd, int currentPosition, GameCompare &cp) {
    if (currentPosition != -1 && cp.position.find(0) == cp.position.end() &&
        !(cp.position.find(currentPosition) != cp.position.end())) {
        std::ostringstream ss;
        std::copy(cp.position.begin(), cp.position.end(), std::ostream_iterator<int>(ss, ","));
        printf("无效Position: %d -> %s\n", currentPosition, ss.str().c_str());
        return CompareResult{0, 0, 0, 0, 0, -1, ""};
    }
    std::string hash = getScreenshotPHash(hwnd, cp.x, cp.y, cp.w, cp.h);
    int pv = hammingDistance(hash, cp.hash);
    printf("N %s C %d H %s\n", cp.name.c_str(), pv, hash.c_str());
    return CompareResult{cp.x, cp.y, cp.w, cp.h, cp.r, pv, cp.name};
}

bool CompareManager::compareValid(HWND hwnd, int currentPosition, GameCompare &cp) {
    int cpv = compare(hwnd, currentPosition, cp).pv;
    printf("N %s R %d C %d\n", cp.name.c_str(), cp.r, cpv);
    return cpv <= cp.r;
}

bool CompareManager::compareValid(HWND hwnd, int currentPosition, std::string &&cpName) {
    std::string iCpName = std::forward<std::string>(cpName);
    if (this->cpMap.count(iCpName) == 0) {
        printf("无效CpName -> %s\n", iCpName.c_str());
        return false;
    }
    GameCompare cp = this->cpMap[iCpName];
    return this->compareValid(hwnd, currentPosition, cp);
}

CompareResult CompareManager::compare(HWND hwnd, int currentPosition, std::string &&cpName) {
    std::string iCpName = std::forward<std::string>(cpName);
    if (this->cpMap.count(iCpName) == 0) {
        printf("无效CpName -> %s\n", iCpName.c_str());
        return CompareResult{0, 0, 0, 0, 0, -1, ""};
    }
    GameCompare cp = this->cpMap[iCpName];
    return this->compare(hwnd, currentPosition, cp);
}

OcrLite *CompareManager::getOcrLite() {
    return this->ocrLite;
}

bool CompareManager::backToHome(HWND &hwnd, const std::string &clientName) {
    if (checkMain(hwnd)) {
        printf("[%s]已在首页\n", clientName.c_str());
        return true;
    }
    int curBack = 1;
    while (curBack <= 15) {
        printf("[%s]返回首页中第[%d]次\n", clientName.c_str(), curBack);
        bool wait = false;
        for (auto &iter : this->cpMap) {
            if (iter.first.find("back_") != std::string::npos) {
                GameCompare cp = iter.second;
                if (compareValid(hwnd, -1, cp)) {
                    rangeMouseLbClick(hwnd, cp.x, cp.y, cp.x + cp.w, cp.y + cp.h);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    checkModal(hwnd, true, nullptr);
                    wait = true;
                    break;
                }
            }
        }
        if (wait) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            if (checkMain(hwnd)) {
                printf("[%s]已返回首页\n", clientName.c_str());
                return true;
            }
        }
        curBack++;
    }
    printf("[%s]无法返回首页\n", clientName.c_str());
    return false;
}

bool CompareManager::checkMain(HWND &hwnd) {
    return (compareValid(hwnd, -1, cpMap["home_not_expanded"]) ||
            compareValid(hwnd, -1, cpMap["home_expanded"])) &&
           compareValid(hwnd, -1, cpMap["buff_01"]);
}

void CompareManager::checkModal(HWND &hwnd, bool accept,
                                const std::function<bool(HWND & , bool, CompareManager * )> &callback) {
    CompareResult aCr = compare(hwnd, -1, "accept_02");
    CompareResult rCr = compare(hwnd, -1, "refuse_02");
    if (aCr.pv <= aCr.r && rCr.pv <= rCr.r) {
        if (callback != nullptr) {
            accept = callback(hwnd, accept, this);
        }
        if (accept) {
            rangeMouseLbClick(hwnd, aCr.x, aCr.y, aCr.x + aCr.w, aCr.y + aCr.h);
        } else {
            rangeMouseLbClick(hwnd, rCr.x, rCr.y, rCr.x + rCr.w, rCr.y + rCr.h);
        }
    }
}

CompareManager::~CompareManager() {
    delete this->ocrLite;
}
