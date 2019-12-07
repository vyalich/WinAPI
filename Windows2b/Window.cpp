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
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		MinX = CreateWindowExW(WS_EX_RIGHT, L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 10, rc.bottom - 30, 70, 20, m_hwnd, NULL, NULL, NULL);
		//ShowWindow(MinX, SW_SHOWNORMAL);


		//load file on first run
		PWSTR pPath = NULL;
		MessageBox(NULL, L"Чтобы начать со стандартного холста нажмите кнопку отмена", L"Выберите начальный файл", MB_OK);
		COM::com.GetFile(&pPath);
		m_Graph.Read(pPath);
		CoTaskMemFree(pPath);

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
	{
		//DRAWER::DRAW.CleanUP();	//TODO
		//COM::com.CleanUp();	//TODO
		PWSTR pPath = NULL;
		MessageBox(NULL, L"Или нажмите отмена, чтобы не сохранять", L"Выберите файл для сохранения", MB_OK);
		COM::com.GetFile(&pPath);
		m_Graph.Write(pPath);
		PostQuitMessage(0);
		return 0;
	}

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
	DRAWER::DRAW.PaintNet(m_Graph.GetMinMax());
	m_Graph.Draw();
	DRAWER::DRAW.EndPaint();
	//TODO: draw dots, ui
	EndPaint(m_hwnd, &ps);
	//ShowWindow(MinX, SW_SHOWNORMAL);
	//BringWindowToTop(MinX);
	InvalidateRect(MinX, NULL, TRUE);
	UpdateWindow(MinX);
}

void MainWindow::Resize()
{
	DRAWER::DRAW.Resize(m_hwnd);
	m_Graph.SetScale();
	m_Graph.SetAxis();
}

void TextWindow::Create(FLOAT def, D2D1_RECT_U rect)
{
	WCHAR buf[12];
	swprintf_s(buf, L"%.2f", def);
	rc = rect;

	m_hwnd = CreateWindowExW(WS_EX_RIGHT, L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, rc.left, rc.top, rc.right, rc.bottom, m_hwnd, NULL, NULL, NULL);
}