#ifndef YYS_FZ_COMPAREMANAGER_H
#define YYS_FZ_COMPAREMANAGER_H

#include "GameCommon.h"
#include "Windows.h"
#include "windef.h"
#include "../ocr/OcrLite.h"
#include "../common/JsonConvert.h"

class CompareManager {
public:
    explicit CompareManager(JsonConvert *jsonConvert);

    ~CompareManager();

    std::map<std::string, GameCompare> &getCpMap();

    CompareLocation getCompareLocation(std::string &&cpName);

    CompareResult compare(HWND hwnd, int currentPosition, GameCompare &cp);

    bool compareValid(HWND hwnd, int currentPosition, GameCompare &cp);

    CompareResult compare(HWND hwnd, int currentPosition, std::string &&cpName);

    OcrLite *getOcrLite();

    bool backToHome(HWND &hwnd, const std::string &clientName);

    bool onTheHome(HWND &hwnd);

    void checkModal(HWND &hwnd, bool accept, const std::function<bool(HWND &, bool, CompareManager *)> &callback);

private:
    JsonConvert *jsonConvert;
    OcrLite *ocrLite;

    std::map<std::string, GameCompare> cpMap;

    void initCpMap();
};


#endif //YYS_FZ_COMPAREMANAGER_H
