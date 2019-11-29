#include "Window.h"

template <class DERIVED_TYPE>
static LRESULT CALLBACK BaseWindow<DERIVED_TYPE>::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

template <class DERIVED_TYPE>
BOOL BaseWindow<DERIVED_TYPE>::Create(
	PCWSTR lpWindowName,
	DWORD dwStyle,
	DWORD dwExStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu)
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

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(m_hwnd, &ps);
	}
	return 0;

	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}
	return TRUE;
}