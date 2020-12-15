#include "GameUtil.h"
#include <random>

using namespace std;
using namespace cv;

const int DRAG_MAX_SIZE = 20;

random_device rd;
default_random_engine dre = default_random_engine(rd());
uniform_int_distribution<int> clickDis(200, 600);


vector<HWND> hwnds;

BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam)
{
	int length = GetWindowTextLength(hWnd);
	if (IsWindowVisible(hWnd) && length != 0)
	{
		length = length + 1;
		auto buffer = new char[length];
		GetWindowText(hWnd, buffer, length);
		string windowTitle(buffer);
		delete[]buffer;
		if (windowTitle.find("阴阳师-网易游戏") != string::npos)
		{
			hwnds.push_back(hWnd);
		}
	}
	return TRUE;
}

vector<HWND> findYysHwnds() {
	hwnds.clear();
	EnumWindows(enumWindowCallback, 0);
	return hwnds;
}

Mat getScreenshotMat(const HWND& hWnd, int x, int y, int width, int height) {
	HDC hWndDc, hWndCDC;
	HBITMAP hbWin;
	Mat src;
	BITMAPINFOHEADER bi;

	hWndDc = GetDC(hWnd);
	hWndCDC = CreateCompatibleDC(hWndDc);

	SetStretchBltMode(hWndCDC, COLORONCOLOR);

	src.create(height, width, CV_8UC4);

	hbWin = CreateCompatibleBitmap(hWndDc, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	SelectObject(hWndCDC, hbWin);

	//StretchBlt(hWndCDC, 0, 0, width, height, hWndDc, 0, 0, srcWidth, srcHeight, SRCCOPY);
	BitBlt(hWndCDC, 0, 0, width, height, hWndDc, x, y, SRCCOPY);
	GetDIBits(hWndCDC, hbWin, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	DeleteObject(hbWin);
	DeleteDC(hWndCDC);
	ReleaseDC(hWnd, hWndDc);

	//imshow("img", src);
	//imwrite("temp.jpg", src);
	//waitKey(0);
	return src;
}

Mat getScreenshotMat(const HWND& hWnd) {
	RECT hWndSize;
	GetWindowRect(hWnd, &hWndSize);
	int widthOffset = 16;
	int heightOffset = 39;
	int width, height;
	height = hWndSize.bottom - hWndSize.top - heightOffset;
	width = hWndSize.right - hWndSize.left - widthOffset;
	return getScreenshotMat(hWnd, 0, 0, width, height);
}

string getScreenshotPHash(const HWND& hWnd, int x, int y, int width, int height) {
	return getMatPHash(getScreenshotMat(hWnd, x, y, width, height));
}

string getMatPHash(Mat&& src) {
	Mat img, dst, gray;
	Mat inSrc = forward<Mat>(src);
	string result(64, '\0');
	double dIdex[64]{};
	double mean = 0.0;
	int k = 0;

	cvtColor(inSrc, inSrc, COLOR_BGR2GRAY);
	img = Mat_<double>(inSrc);

	resize(img, img, Size(8, 8));
	dct(img, dst);

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			dIdex[k] = dst.at<double>(i, j);
			mean += dst.at<double>(i, j) / 64;
			k++;
		}
	}

	for (int i = 0; i < 64; ++i) {
		if (dIdex[i] >= mean) {
			result[i] = '1';
		}
		else {
			result[i] = '0';
		}
	}
	return result;
}

int hammingDistance(string& hash1, string& hash2) {
	if ((hash1.size() != 64) || (hash2.size() != 64)) {
		return -1;
	}
	int difference = 0;
	for (int i = 0; i < 64; i++) {
		if (hash1[i] != hash2[i]) {
			difference++;
		}
	}
	return difference;
}

MouseClick mouseLbClick(const HWND& hWnd, int x, int y) {
	SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
	int duration = clickDis(dre);
	this_thread::sleep_for(chrono::milliseconds(duration));
	SendMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
	return MouseClick{ x,y,duration };
}

MouseClick rangeMouseLbClick(const HWND& hwnd, int sx, int sy, int ex, int ey) {
	uniform_int_distribution<int> rx(sx, ex);
	uniform_int_distribution<int> ry(sy, ey);
	return mouseLbClick(hwnd, rx(dre), ry(dre));
}

vector<MouseDrag> mouseDrag(const HWND& hWnd, int sx, int sy, int ex, int ey, bool horizontal) {
	int curX = sx, curY = sy, duration;

	uniform_int_distribution<int> dragDis(10, 50);
	uniform_int_distribution<int> dragSizeDis(5, DRAG_MAX_SIZE);
	uniform_int_distribution<int> dragOffsetDis(-2, 2);

	vector<MouseDrag> mdVector;
	SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(sx, sy));
	this_thread::sleep_for(chrono::milliseconds(clickDis(dre)));
	while (abs(curX - ex) > DRAG_MAX_SIZE || abs(curY - ey) > DRAG_MAX_SIZE) {
		MouseDrag md{};
		md.sx = curX;
		md.sy = curY;
		duration = dragDis(dre);
		if (abs(curX - ex) > DRAG_MAX_SIZE) {
			if (sx > ex) {
				curX -= dragSizeDis(dre);
			}
			else {
				curX += dragSizeDis(dre);
			}
		}
		if (abs(curY - ey) > DRAG_MAX_SIZE) {
			if (sy > ey) {
				curY -= dragSizeDis(dre);
			}
			else {
				curY += dragSizeDis(dre);
			}
		}
		if (horizontal) {
			curY += dragOffsetDis(dre);
		}
		else {
			curX += dragOffsetDis(dre);
		}
		md.duration = dragDis(dre);
		md.ex = curX;
		md.ey = curY;
		mdVector.push_back(md);
		SendMessage(hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(curX, curY));
		this_thread::sleep_for(chrono::milliseconds(md.duration));
	}
	SendMessage(hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(ex, ey));
	SendMessage(hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(ex, ey));
	return mdVector;
}

string getExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}

bool dirExists(const std::string& dirPath){
	DWORD ftyp = GetFileAttributesA(dirPath.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES) {
		return false;
	}
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
		return true;
	}
	return false;
}