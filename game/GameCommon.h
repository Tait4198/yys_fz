#pragma once
#include <string>
#include <set>

struct GameCompare
{
	int x;
	int y;
	int w;
	int h;
	std::set<int> position;
	int r;
	std::string hash;
	std::string name;
};

struct CompareResult {
	int x;
	int y;
	int w;
	int h;
	int r;
	int pv;
	std::string name;
};
