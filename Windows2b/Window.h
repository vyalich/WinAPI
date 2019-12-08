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

enum BTN_ID
{
	BTN_CHANGE	= 1,
	BTN_SAVE	= 2,
	BTN_LOAD	= 3
};

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

class UIWindow
{
protected:
	HWND m_hwnd;

public:
	UIWindow() {}

	void Paint();
	void Move(D2D1_RECT_U rect);
};

class StaticWindow : public UIWindow
{
public:
	StaticWindow() {}

	void Create(LPCWSTR title, D2D1_RECT_U rect, HWND hParent);
	void ChangeTitle(LPWSTR nTitle);
};

class TextInputWindow: public UIWindow
{
public:
	TextInputWindow() {}

	void Create(FLOAT def, D2D1_RECT_U rect, HWND hParent);
	HWND GetHWND() { return m_hwnd; }
};

class ButtonWindow : public UIWindow
{
public:
	ButtonWindow() {}

	void Create(INT ID, LPCWSTR title, D2D1_RECT_U rect, HWND hParent);
};

class MainWindow : public BaseWindow<MainWindow>
{
private:
	//LPWSTR	pFileName;
	Graph	m_Graph;
	TextInputWindow		MinX, MaxX, MinY, MaxY;
	ButtonWindow		bChange, bSave, bLoad;
	StaticWindow		FileName;

	void	Paint();
	void    Resize();
	void    GetBounds();

public:
	MainWindow() {}

	LPCWSTR  ClassName() const { return L"Graph editor class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

