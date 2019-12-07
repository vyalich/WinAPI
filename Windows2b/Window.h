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

//#include "SafeRelease.h"
#include "Draw.h"
#include "COM.h"
#include "Graph.h"

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

	virtual LPCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};

class MainWindow : public BaseWindow<MainWindow>
{
private:
	LPWSTR pFileName;
	Graph  m_Graph;
	HWND MinX, MaxX, MinY, MaxY;

	void	Paint();
	void    Resize();

public:
	MainWindow() {}

	LPCWSTR  ClassName() const { return L"Graph editor class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class TextWindow
{
private:
	D2D1_RECT_U rc;
	HWND m_hwnd;

public:
	void Create(FLOAT def, D2D1_RECT_U rect);
};