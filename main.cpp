#include "Winsock2.h"
#include "windef.h"
#include <iostream>
#include <drogon/HttpAppFramework.h>
#include "game/FzManager.h"
#include "game/GameUtil.h"

extern FzManager fzManager;

using namespace std;
using namespace cv;
using namespace drogon;

int main(int argc, char** argv) {
	string sel = "back_02";
	if (argc > 1) {
		sel = argv[1];
	}
	vector<HWND> hwnds = findYysHwnds();
	for (auto hwnd : hwnds) {
			string cpName = sel;
		int result = fzManager.compare(hwnd, move(cpName)).pv;
		cout << "结果" << result << endl;
	}

//	app().addListener("0.0.0.0", 52114)
//		.setThreadNum(16)
//		.run();
	return 0;
}