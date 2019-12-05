#pragma once

#include <windows.h>
#include <list>
#include <fstream>

#include "Draw.h"

enum AXIS_POS
{
	AXIS_RIGHT = -2,
	AXIS_LEFT = -1,
	AXIS_TOP = -3,
	AXIS_BOTTOM = -4
};

struct Dot
{
	FLOAT x;
	FLOAT y;
};

class Graph
{
private:
	std::list<Dot> l_Dots;
	FLOAT minX, maxX, minY, maxY;
	D2D1_POINT_2F scale;

public:
	int Read(LPWSTR pPath);
	int Write(LPWSTR pPath);
	void Draw();
	void SetScale();
	D2D1_POINT_2F XYtoDIP(Dot& dot);
	D2D1_POINT_2F XYtoDPI(Dot& dot);
	Dot DPItoXY(INT x, INT y);
	void SetAxis();
	void Search(INT x, INT y);
};

