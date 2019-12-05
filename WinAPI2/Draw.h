#pragma once

#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <dwrite.h>
#pragma comment(lib, "Dwrite")

class Drawer
{
private:
	//drawing resources
	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1SolidColorBrush* pBrush;

	//drawing text resources
	IDWriteFactory* pDWriteFactory;
	IDWriteTextFormat* pTextFormat;
	ID2D1SolidColorBrush* pBlackBrush;
public:
	static Drawer m_Drawer;
};

