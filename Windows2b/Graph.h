#pragma once

#include <windows.h>
#include <list>
#include <fstream>

#include "Draw.h"

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
	int		Read(LPWSTR pPath);
	int		Write(LPWSTR pPath);

	void	SetScale();
	void	SetAxis();

	D2D1_POINT_2F	XYtoDIP(Dot& dot);
	D2D1_POINT_2F	XYtoDPI(Dot& dot);
	Dot				DPItoXY(INT x, INT y);
	
	void	Search(INT x, INT y);
	void	Draw();

	void		SetMinMax(D2D1_RECT_F minmax);
	D2D1_RECT_F GetMinMax() { return D2D1::RectF(minX, maxY, maxX, minY); }
};

