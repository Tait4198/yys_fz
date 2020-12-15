

#include "ocr/OcrLite.h"
#include "Winsock2.h"
#include "windows.h"
#include "windef.h"
#include <iostream>
#include <drogon/HttpAppFramework.h>
#include "controller/yys_YysTest.h"
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
	for (int i = 0; i < hwnds.size(); i++) {
		HWND hwnd = hwnds[i];
		string cpName = sel;
		int result = fzManager.compare(hwnd, move(cpName)).pv;
		cout << "结果" << result << endl;
	}

//	app().addListener("0.0.0.0", 52114)
//		.setThreadNum(16)
//		.run();

	/*
	* string modelsDir = "D:/cpp/yys_fz/x64/Release/models";
	OcrLite ocrLite(4);
	ocrLite.initLogger(
		true,//isOutputConsole
		false,//isOutputPartImg
		true);
	ocrLite.initModels(modelsDir.c_str());

	vector<HWND> hwnds = findYysHwnds();
	for (int j = 0; j < 60; j++) {
		cout << j << endl;
		for (int i = 0; i < hwnds.size(); i++) {
			HWND hwnd = hwnds[i];
			Mat mat = getScreenshotMat(hwnd);
			//imwrite("D:/YYS/battle.jpg", mat);
			ocrLite.detect(mat);
			mat.release();
		}
		Sleep(1000);
	}
	*/
	return 0;
}