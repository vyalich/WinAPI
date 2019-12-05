#include "Window.h"

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		//initialise
		if (DRAWER::DRAW.Init(m_hwnd) < 0 || FAILED(COM::com.Init()))
			return -1;// Fail CreateWindowEx.

		MinX.Create(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 10, 100, 10, m_hwnd);

		//load file on first run
		PWSTR pPath = NULL;
		MessageBox(NULL, L"Чтобы начать со стандартного холста нажмите кнопку отмена", L"Выберите начальный файл", MB_OK);
		COM::com.GetFile(&pPath);
		m_Graph.Read(pPath);
		CoTaskMemFree(pPath);

		//MinX.Create("MinX", )

		return 0; 
	}

	case WM_LBUTTONDOWN:
	{
		INT xPos = GET_X_LPARAM(lParam);
		INT yPos = GET_Y_LPARAM(lParam);

		m_Graph.Search(xPos, yPos);

		Paint();

		return 0;
	}

	case WM_SIZE:
		Resize();
		return 0;

	case WM_DESTROY:
		//DRAWER::DRAW.CleanUP();	//TODO
		//COM::com.CleanUp();	//TODO
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		Paint();
		return 0;
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void MainWindow::Paint()
{
	PAINTSTRUCT ps;
	BeginPaint(m_hwnd, &ps);
	DRAWER::DRAW.BeginPaint();
	DRAWER::DRAW.PaintBG();
	m_Graph.Draw();
	DRAWER::DRAW.EndPaint();
	//TODO: draw dots, ui
	EndPaint(m_hwnd, &ps);
}

void MainWindow::Resize()
{
	DRAWER::DRAW.Resize(m_hwnd);
	m_Graph.SetScale();
	m_Graph.SetAxis();
}

LRESULT TextInputWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*switch (uMsg)
	{
	case WM_CREATE:
	{
		return 0;
	}
	case WM_DESTROY:
		//DRAWER::DRAW.CleanUP();	//TODO
		//COM::com.CleanUp();	//TODO
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		return 0;
	}*/

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}