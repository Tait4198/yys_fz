#include "Winsock2.h"
#include "windef.h"
#include <iostream>
#include <drogon/HttpAppFramework.h>
#include "game/FzManager.h"
#include "game/GameUtil.h"
#include <filesystem>
#include <fstream>

extern FzManager fzManager;

using namespace std;
using namespace cv;
using namespace drogon;
namespace fs = std::filesystem;

int main(int argc, char **argv) {
    std::ifstream ifs("config.json");
    std::string jsonStr((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
//
    for (auto &hexHwnd : fzManager.getHexHwndList()) {
//        string cpName = sel;
//        string hx = hexHwnd;
//        int result = fzManager.compare(move(hx), move(cpName)).pv;
//        cout << "结果" << result << endl;
        fzManager.execTasks(hexHwnd, jsonStr);
    }
    // buff拖动
//    HWND hwnd = (HWND)0x008F1072;
//    mouseDrag(hwnd,485,230,485,176,true);

    while (true) {
        this_thread::sleep_for(std::chrono::seconds(1));
    }
//	app().addListener("0.0.0.0", 52114)
//		.setThreadNum(16)
//		.run();
    return 0;
}