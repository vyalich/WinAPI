#include "Draw.h"

DRAWER DRAWER::DRAW;

int DRAWER::Init(HWND hwnd)
{
	//create d2 factory
	if (SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
	{
		//create dwrite factory
		if (SUCCEEDED(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown * *>(&pDWriteFactory)
		)))
			{
				HRESULT hr = S_OK;
				RECT rc;
				GetClientRect(hwnd, &rc);

				D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

				hr = pFactory->CreateHwndRenderTarget(
					D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties(hwnd, size),
					&pRenderTarget);

				SetDIPScale(hwnd);

				if (SUCCEEDED(hr))
				{
					if (SUCCEEDED(pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0, 0), &pRedBrush)) &&
						SUCCEEDED(pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &pBlackBrush)) &&
						SUCCEEDED(pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.4f, 0.4f, 0.4f), &pGrayBrush)) )
					{
						hr = pDWriteFactory->CreateTextFormat(
							L"Arial Black",
							NULL,
							DWRITE_FONT_WEIGHT_REGULAR,
							DWRITE_FONT_STYLE_NORMAL,
							DWRITE_FONT_STRETCH_NORMAL,
							10.0f,
							L"en-us",
							&pTextFormat
						);
						if (SUCCEEDED(hr))
						{
							pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
						}
						return 0;
					}
				}
			}
			//return 0;
	}
	return -1;
}

void DRAWER::Resize(HWND hwnd)
{
	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		SetDIPScale(hwnd);
		InvalidateRect(hwnd, NULL, FALSE);
	}
}

void DRAWER::PaintBG()
{
	pRenderTarget->Clear(D2D1::ColorF(0.8f, 0.8f, 0.8f));
	//PaintNet();
}

void DRAWER::PaintNet(D2D1_RECT_F minmax)
{
	D2D1_SIZE_F size = pRenderTarget->GetSize();
	FLOAT dx = size.width * 0.95 / 20.0;
	FLOAT dy = size.height * 0.85 / 20.0;
	FLOAT startY = size.height * 0.025;
	FLOAT startX = size.width * 0.025;
	FLOAT startTX = 0;
	FLOAT startTY = 0;
	FLOAT Tdx = (minmax.right - minmax.left) / 20;
	FLOAT Tdy = (minmax.top - minmax.bottom) / 20;
	FLOAT shiftX = 0;

	if (axis.x > 0)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(axis.x, 0), D2D1::Point2F(axis.x, size.height), pBlackBrush, 2);
		pRenderTarget->DrawLine(D2D1::Point2F(axis.x, 0), D2D1::Point2F(axis.x - size.height * 0.005, size.width * 0.01), pBlackBrush, 2);
		pRenderTarget->DrawLine(D2D1::Point2F(axis.x, 0), D2D1::Point2F(axis.x + size.height * 0.005, size.width * 0.01), pBlackBrush, 2);
		startX = axis.x;
		if (startX > size.width * 0.95)
		{
			shiftX = -size.width * 0.06;
			pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		}
	}
	else if (axis.x == AXIS_LEFT)
	{
		startTX = minmax.left;
		//shiftX = size.width * 0.91;
	}
	else if (axis.x == AXIS_RIGHT)
	{
		startX = size.width * 0.975;
		startTX = minmax.right;
		shiftX = - size.width * 0.06;
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	}
	
	if (axis.y > 0)	//TODO: fix HINTS
	{
		pRenderTarget->DrawLine(D2D1::Point2F(0, axis.y), D2D1::Point2F(size.width, axis.y), pBlackBrush, 2);
		pRenderTarget->DrawLine(D2D1::Point2F(size.width * 0.99, axis.y + size.height * 0.005), D2D1::Point2F(size.width, axis.y), pBlackBrush, 2);
		pRenderTarget->DrawLine(D2D1::Point2F(size.width * 0.99, axis.y - size.height * 0.005), D2D1::Point2F(size.width, axis.y), pBlackBrush, 2);
		startY = axis.y;
	}
	else if (axis.y == AXIS_TOP)
	{
		startTY = minmax.top;
		
	}
	else if (axis.y == AXIS_BOTTOM)
	{
		//startX = size.height * 0.975;
		startTY = minmax.bottom;
		startY = size.height * 0.85;
	}

	FLOAT n;
	
	//horizontal lines draw
	for (int i = 0; (n = startY + i * dy) < size.height; i++)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(0, n), D2D1::Point2F(size.width, n), pGrayBrush);
		DrawNum(startTY - i * Tdy, startX + shiftX + 3, n - 13);
	}
	for (int i = 1; (n = startY - i * dy) > 0; i++)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(0, n), D2D1::Point2F(size.width, n), pGrayBrush);
		DrawNum(startTY + i * Tdy, startX + shiftX + 3, n - 13);
	}

	//vertical lines draw
	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	for (int i = 0; (n = startX + i * dx) < size.width; i++)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(n, 0), D2D1::Point2F(n, size.height), pGrayBrush);
		DrawNum(startTX + i * Tdx, n + 3, startY + 3);
	}
	for (int i = 1; (n = startX - i * dx) > 0; i++)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(n, 0), D2D1::Point2F(n, size.height), pGrayBrush);
		DrawNum(startTX - i * Tdx, n + 3, startY + 3);
	}
}

void DRAWER::PaintDot(D2D1_POINT_2F center)
{
	pRenderTarget->FillEllipse(D2D1::Ellipse(center, 3, 3), pRedBrush);
}

void DRAWER::PaintLine(D2D1_POINT_2F first, D2D1_POINT_2F second)
{
	pRenderTarget->DrawLine(first, second, pRedBrush);
}

void DRAWER::SetDIPScale(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	DIPscale.x = GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
	DIPscale.y = GetDeviceCaps(hdc, LOGPIXELSY) / 96.0f;
	ReleaseDC(hwnd, hdc);
}

void DRAWER::DrawNum(FLOAT n, FLOAT x, FLOAT y)
{
	WCHAR buf[10];
	D2D1_SIZE_F size = pRenderTarget->GetSize();
	swprintf_s(buf, L"%.2f", n);
	pRenderTarget->DrawText(buf, lstrlen(buf), pTextFormat, D2D1::RectF(x, y, x + size.width*0.05, y + size.height*0.05), pBlackBrush);
}