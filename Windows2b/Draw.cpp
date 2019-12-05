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
							hr = pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
						}
						if (SUCCEEDED(hr))
						{
							hr = pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
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
	PaintNet();
}

void DRAWER::PaintNet()
{
	D2D1_SIZE_F size = pRenderTarget->GetSize();
	FLOAT dx = size.width * 0.95 / 20.0;
	FLOAT dy = size.height * 0.85 / 20.0;
	FLOAT startY = size.height * 0.025;
	FLOAT startX = size.height * 0.025;

	if (axis.x > 0)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(axis.x, 0), D2D1::Point2F(axis.x, size.height), pBlackBrush, 2);
		pRenderTarget->DrawLine(D2D1::Point2F(axis.x, 0), D2D1::Point2F(axis.x - size.height * 0.005, size.width * 0.01), pBlackBrush, 2);
		pRenderTarget->DrawLine(D2D1::Point2F(axis.x, 0), D2D1::Point2F(axis.x + size.height * 0.005, size.width * 0.01), pBlackBrush, 2);
		startX = axis.x;
	}
	
	if (axis.y > 0)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(0, axis.y), D2D1::Point2F(size.width, axis.y), pBlackBrush, 2);
		pRenderTarget->DrawLine(D2D1::Point2F(size.width * 0.99, axis.y + size.height * 0.005), D2D1::Point2F(size.width, axis.y), pBlackBrush, 2);
		pRenderTarget->DrawLine(D2D1::Point2F(size.width * 0.99, axis.y - size.height * 0.005), D2D1::Point2F(size.width, axis.y), pBlackBrush, 2);
		startY = axis.y;
	}

	FLOAT n;
	for (int i = 1; (n = startY + i * dy) < size.height; i++)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(0, n), D2D1::Point2F(size.width, n), pGrayBrush);
	}
	for (int i = 1; (n = startY - i * dy) > 0; i++)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(0, n), D2D1::Point2F(size.width, n), pGrayBrush);
	}
	for (int i = 1; (n = startX + i * dx) < size.width; i++)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(n, 0), D2D1::Point2F(n, size.height), pGrayBrush);
	}
	for (int i = 1; (n = startX - i * dx) > 0; i++)
	{
		pRenderTarget->DrawLine(D2D1::Point2F(n, 0), D2D1::Point2F(n, size.height), pGrayBrush);
	}
}

void DRAWER::PaintDot(D2D1_POINT_2F center)
{
	pRenderTarget->FillEllipse(D2D1::Ellipse(center, 3, 3), pRedBrush);
}

void DRAWER::SetDIPScale(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	DIPscale.x = GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
	DIPscale.y = GetDeviceCaps(hdc, LOGPIXELSY) / 96.0f;
	ReleaseDC(hwnd, hdc);
}