#include "Window.h"



// Recalculate drawing layout when the size of the window changes.
void MainWindow::CalculateLayout()
{
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		el_x = size.width / 2;
		el_y = size.height / 2;
		if (size.width && size.height)
		{
			el_a = size.width / 4 - rand() % (int)(size.width / 5);
			el_b = size.height / 4 - rand() % (int)(size.height / 5);
		}
		ellipse = D2D1::Ellipse(D2D1::Point2F(el_x, el_y), el_a, el_b);

		layout.left = 0; layout.right = size.width;
		layout.top = 0; layout.bottom = size.height / 4;
	}
}

HRESULT MainWindow::CreateGraphicsResources()
{
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&pRenderTarget);

		if (SUCCEEDED(hr))
		{
			if (SUCCEEDED(pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0), &pBrush)) &&
				SUCCEEDED(pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &pBlackBrush)))
			{
				hr = pDWriteFactory->CreateTextFormat(
					L"Arial Black",
					NULL,
					DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					46.0f,
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
				CalculateLayout();
			}
		}
	}
	return hr;
}

void MainWindow::DiscardGraphicsResources()
{
	SafeRelease(&pTextFormat);
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
}

void MainWindow::OnPaint()
{
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);

		pRenderTarget->BeginDraw();

		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

		pRenderTarget->DrawText(author, lstrlen(author), pTextFormat, &layout, pBlackBrush);

		D2D1_SIZE_F size = pRenderTarget->GetSize();
		layout.top = size.height * 3 / 4;
		layout.bottom = size.height;
		pRenderTarget->DrawText(version, lstrlen(version), pTextFormat, &layout, pBlackBrush);
		
		pRenderTarget->FillEllipse(ellipse, pBrush);

		hr = pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}
}

void MainWindow::Resize()
{
	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		CalculateLayout();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
		{
			if (SUCCEEDED(DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown * *>(&pDWriteFactory)
			)))
			{
				/*DWORD dwVersion = GetVersionEx(;
				DWORD dwMajor = (DWORD)(LOBYTE(LOWORD(dwVersion)));
				DWORD dwMinor = (DWORD)(HIBYTE(LOWORD(dwVersion)));
				DWORD dwBuild = 0;
				if (dwVersion < 0x80000000)
					dwBuild = (DWORD)(HIWORD(dwVersion));
				wchar_t ver[100];
				swprintf_s(ver, L"Версия WinAPI: %d.%d (%d)", dwMajor, dwMinor, dwBuild);
				version = ver;*/
				version = L"Версия WinAPI: GetVersion deprecated";
				author = L"Дмитрий Вялков И983";
				return 0;
			}
		}
		
		return -1; // Fail CreateWindowEx.

	case WM_DESTROY:
		DiscardGraphicsResources();
		SafeRelease(&pFactory);
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_LBUTTONDOWN:
	{
		INT xPos = GET_X_LPARAM(lParam);
		INT yPos = GET_Y_LPARAM(lParam);

		if ((xPos - el_x)*(xPos - el_x) /el_a/el_a + (yPos - el_y)* (yPos - el_y) /el_b/el_b < 1)
		{
			CalculateLayout();
			OnPaint();
		}

		return 0;
	}

	case WM_SIZE:
		Resize();
		return 0;
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}