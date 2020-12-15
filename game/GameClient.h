#pragma once
#include "Windows.h"
#include "windef.h"
#include <map>
#include <string>
#include "GameCommon.h"

class GameClient
{
public:
	GameClient(HWND hwnd, std::string hexHwnd, std::map<std::string, GameCompare>* cpMapPtr);
	~GameClient();
	HWND getHwnd();
	CompareResult compare(std::string&& cpName);
	CompareResult compare(std::string&& cpName, bool missPosition);

	bool backToHome();
	void execTask();

private:
	HWND hwnd;
	std::string hexHwnd;
	std::map<std::string, GameCompare>* cpMapPtr;
	/// <summary>
	/// 游戏当前所在位置
	/// </summary>
	int currentPosition;
	int currentTaskId;
	std::string group;

	void initClientSize();

	/// <summary>
	/// 返回主页
	/// </summary>
	/// <param name="cp"></param>
	/// <param name="missPosition"></param>
	/// <returns></returns>
	CompareResult inCompare(GameCompare& cp, bool missPosition);

	bool inCompareValid(GameCompare& cp, bool missPosition);

	void threadFunc();
};

