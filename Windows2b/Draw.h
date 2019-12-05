#pragma once

#include <windowsx.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <dwrite.h>
#pragma comment(lib, "Dwrite")

class DRAWER
{
private:
	//drawing resources
	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRenderTarget;

	//drawing text resources
	IDWriteFactory* pDWriteFactory;
	IDWriteTextFormat* pTextFormat;

	//brushes to draw
	ID2D1SolidColorBrush* pBlackBrush;
	ID2D1SolidColorBrush* pRedBrush;
	ID2D1SolidColorBrush* pGrayBrush;

	D2D1_POINT_2F axis;
	D2D1_POINT_2F DIPscale;

public:
	static DRAWER DRAW;

	int  Init(HWND hwnd);
	void Resize(HWND hwnd);
	void BeginPaint() { pRenderTarget->BeginDraw(); }
	void EndPaint() { pRenderTarget->EndDraw(); }
	void PaintBG();
	void PaintNet();
	void PaintDot(D2D1_POINT_2F center);
	D2D1_SIZE_F GetSizeF() { return pRenderTarget->GetSize(); }
	D2D1_SIZE_U GetSizeU() { return pRenderTarget->GetPixelSize(); }
	void SetAxisX(FLOAT x) { axis.x = x; }
	void SetAxisY(FLOAT y) { axis.y = y; }
	void SetDIPScale(HWND hwnd);
	FLOAT GetDIPX(int x) { return static_cast<float>(x) / DIPscale.x; }
	FLOAT GetDIPY(int y) { return static_cast<float>(y) / DIPscale.y; }
};

