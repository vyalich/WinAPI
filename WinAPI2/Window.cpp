#include "Window.h"

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:

		return 0; // Fail CreateWindowEx.

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		return 0;
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}