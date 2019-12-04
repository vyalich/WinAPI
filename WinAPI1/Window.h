#pragma once

#include <windowsx.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <dwrite.h>
#pragma comment(lib, "Dwrite")
#include <stdlib.h>
#include <ctime>
#include <string>
#include <sysinfoapi.h>
#include <WinBase.h>

#include "SafeRelease.h"

template <class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE* pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	BaseWindow() : m_hwnd(NULL) { }

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
	)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);

		m_hwnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
		);

		return (m_hwnd ? TRUE : FALSE);
	}

	HWND Window() const { return m_hwnd; }

protected:

	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};

class MainWindow : public BaseWindow<MainWindow>
{
	//drawing resources
	ID2D1Factory*			pFactory;
	ID2D1HwndRenderTarget*	pRenderTarget;
	ID2D1SolidColorBrush*	pBrush;

	//drawing text resources
	IDWriteFactory*			pDWriteFactory;
	IDWriteTextFormat*		pTextFormat;
	ID2D1SolidColorBrush*	pBlackBrush;
	LPCWSTR					author;
	LPCWSTR					version;
	D2D1_RECT_F				layout;

	//ellipse resources
	D2D1_ELLIPSE	ellipse;
	DOUBLE			el_x, el_y, el_a, el_b;

	void    CalculateLayout();
	HRESULT CreateGraphicsResources();
	void    DiscardGraphicsResources();
	void    OnPaint();
	void    Resize();

public:

	MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
	{
	}

	PCWSTR  ClassName() const { return L"Circle Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};