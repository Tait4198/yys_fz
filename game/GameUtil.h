#pragma once
#include "Windows.h"
#include "windef.h"
#include <iostream>
#include <opencv2/opencv.hpp>

struct MouseClick {
	int x;
	int y;
	int duration;
};

struct MouseDrag {
	int sx;
	int sy;
	int ex;
	int ey;
	int duration;
};

std::vector<HWND> findYysHwnds();

cv::Mat getScreenshotMat(const HWND&, int, int, int, int);

cv::Mat getScreenshotMat(const HWND&);

std::string getScreenshotPHash(const HWND&, int, int, int, int);

std::string getMatPHash(cv::Mat&&);

int hammingDistance(std::string&, std::string&);

MouseClick mouseLbClick(const HWND&, int, int);

std::vector<MouseClick> rangeMouseLbClick(const HWND &hwnd, int sx, int sy, int ex, int ey, int count);

MouseClick rangeMouseLbClick(const HWND& hwnd, int sx, int sy, int ex, int ey);

std::vector<MouseDrag> mouseDrag(const HWND&, int, int, int, int, bool);

std::string getExePath();

bool dirExists(const std::string& dirPath);