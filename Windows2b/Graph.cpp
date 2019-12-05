#include "Graph.h"

bool DotCompare(const Dot& c1, const Dot& c2)
{
	return c1.x < c2.x;
}


int Graph::Read(LPWSTR pPath)
{
	if (pPath)
	{
		//read dots from file
		std::ifstream in(pPath, std::ios_base::binary);
		Dot n_Dot;

		if (in.tellg() > 0)
		{
			while (in.read((char*)& n_Dot, sizeof(Dot)))
			{
				l_Dots.emplace_back(n_Dot);
			}
			in.close();

			//sort dots
			l_Dots.sort(DotCompare);

			//seearch min/max
			minX = l_Dots.front().x;
			maxX = l_Dots.back().x;
			minY = maxY = l_Dots.front().y;

			for (auto& li : l_Dots)
			{
				if (minY > li.y)
					minY = li.y;
				if (maxY < li.y)
					maxY = li.y;
			}
		}
	}
	else
	{
		minX = minY = -20;
		maxX = maxY = 20;
	}

	SetScale();
	SetAxis();

	//WCHAR buf[100];
	/*for (auto& li : l_Dots)
	{
		swprintf_s(buf, L"%f %f", li.x, li.y);
		MessageBox(NULL, buf, L"x y", MB_OK);
	}*/
	//swprintf_s(buf, L"%f %f %f %f",minX, maxX, minY, maxY);
	//MessageBox(NULL, buf, L"x y", MB_OK);

	return 0;
}

//TODO
int Graph::Write(LPWSTR pPath)
{
	return 0;
}

void Graph::Draw()
{
	for (auto& li : l_Dots)
	{
		DRAWER::DRAW.PaintDot(XYtoDIP(li));
	}
}

void Graph::SetScale()
{
	D2D1_SIZE_F size = DRAWER::DRAW.GetSizeF();
	scale.x = size.width * 0.95 / (maxX - minX);
	scale.y = size.height * 0.85 / (maxY - minY);
}

D2D1_POINT_2F Graph::XYtoDIP(Dot& dot)
{
	D2D1_SIZE_F size = DRAWER::DRAW.GetSizeF();
	return D2D1::Point2F(size.width*0.025 + (dot.x - minX) * scale.x, size.height*0.875 - (dot.y - minY) * scale.y);
}

D2D1_POINT_2F Graph::XYtoDPI(Dot& dot)
{
	D2D1_SIZE_U size = DRAWER::DRAW.GetSizeU();
	return D2D1::Point2F(size.width * 0.025 + (dot.x - minX) * size.width * 0.95 / (maxX - minX), 
		                 size.height * 0.875 - (dot.y - minY) * size.height * 0.85 / (maxY - minY));
}

Dot Graph::DPItoXY(INT x, INT y)
{

	D2D1_SIZE_U size = DRAWER::DRAW.GetSizeU();
	Dot dot;
	dot.x = (static_cast<FLOAT>(x) - size.width * 0.025) * (maxX - minX) / 0.95 / size.width + minX;
	dot.y = (static_cast<FLOAT>(y) - size.height * 0.875) * (minY - maxY) / 0.85 / size.height + minY;
	return dot;
}

void Graph::SetAxis()
{
	D2D1_SIZE_F size = DRAWER::DRAW.GetSizeF();
	
	if (minX <= 0 && maxX >= 0)
		DRAWER::DRAW.SetAxisX(size.width * 0.025 + (0 - minX) * scale.x);
	else if (minX <= 0 && maxX <= 0)
		DRAWER::DRAW.SetAxisX(AXIS_RIGHT);
	else
		DRAWER::DRAW.SetAxisX(AXIS_LEFT);
	
	if (minY <= 0 && maxY >= 0)
		DRAWER::DRAW.SetAxisY(size.height * 0.875 - (0 - minY) * scale.y);
	else if (minY <= 0 && maxY <= 0)
		DRAWER::DRAW.SetAxisY(AXIS_TOP);
	else
		DRAWER::DRAW.SetAxisY(AXIS_BOTTOM);

}

void Graph::Search(INT x, INT y)
{
	D2D1_POINT_2F dot;
	for (auto it = l_Dots.begin(); it != l_Dots.end(); it++)
	{
		dot = XYtoDPI(*it);
		if (dot.x >= x - 3 && dot.x <= x + 3)
		{
			l_Dots.erase(it);
			if(dot.y >= y - 3 && dot.y <= y + 3)
				return;
			break;
		}
	}
	l_Dots.emplace_back(DPItoXY(x, y));
	l_Dots.sort(DotCompare);
}