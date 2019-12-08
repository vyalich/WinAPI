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

		m_Graph.SetMinMax(D2D1::RectF(-20, 20, 20, -20));

		//create ui elements
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_RECT_F minmax = m_Graph.GetMinMax();
		MinX.Create(minmax.left, D2D1::RectU(10, rc.bottom - 60, 80, 20), m_hwnd);
		MaxX.Create(minmax.right, D2D1::RectU(100, rc.bottom - 60, 80, 20), m_hwnd);
		MinY.Create(minmax.bottom, D2D1::RectU(10, rc.bottom - 30, 80, 20), m_hwnd);
		MaxY.Create(minmax.top, D2D1::RectU(100, rc.bottom - 30, 80, 20), m_hwnd);
		bChange.Create(BTN_CHANGE, L"Масштаб", D2D1::RectU(190, rc.bottom - 60, 70, 50), m_hwnd);
		bSave.Create(BTN_SAVE, L"Сохранить", D2D1::RectU(rc.right - 180, rc.bottom - 60, 80, 20), m_hwnd);
		bLoad.Create(BTN_LOAD, L"Загрузить", D2D1::RectU(rc.right - 90, rc.bottom - 60, 80, 20), m_hwnd);
		FileName.Create(L"Безымянный", D2D1::RectU(rc.right - 410, rc.bottom - 30, 400, 20), m_hwnd);

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

	case WM_COMMAND:
		//MessageBox(NULL, L"Или нажмите отмена, чтобы не сохранять", L"Выберите файл для сохранения", MB_OK);
		switch (LOWORD(wParam)) {
		case BTN_CHANGE:
			GetBounds();
			Paint();
			break;

		case BTN_SAVE:
		{
			PWSTR pPath = NULL;
			COM::com.GetFile(&pPath);
			m_Graph.Write(pPath);
			FileName.ChangeTitle(pPath);
			CoTaskMemFree(pPath);
		}
			break;

		case BTN_LOAD:
		{
			PWSTR pPath = NULL;
			COM::com.GetFile(&pPath);
			m_Graph.Read(pPath);
			FileName.ChangeTitle(pPath);
			CoTaskMemFree(pPath);

			Paint();
			break;
		}
		}

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
	DRAWER::DRAW.PaintNet(m_Graph.GetMinMax());
	m_Graph.Draw();
	DRAWER::DRAW.EndPaint();
	EndPaint(m_hwnd, &ps);
	MinX.Paint();
	MinY.Paint();
	MaxX.Paint();
	MaxY.Paint();
	bChange.Paint();
	bSave.Paint();
	bLoad.Paint();
	FileName.Paint();
}

void MainWindow::Resize()
{
	DRAWER::DRAW.Resize(m_hwnd);
	m_Graph.SetScale();
	m_Graph.SetAxis();

	RECT rc;
	GetClientRect(m_hwnd, &rc);
	D2D1_RECT_F minmax = m_Graph.GetMinMax();
	MinX.Move(D2D1::RectU(10, rc.bottom - 60, 80, 20));
	MaxX.Move(D2D1::RectU(100, rc.bottom - 60, 80, 20));
	MinY.Move(D2D1::RectU(10, rc.bottom - 30, 80, 20));
	MaxY.Move(D2D1::RectU(100, rc.bottom - 30, 80, 20));
	bChange.Move(D2D1::RectU(190, rc.bottom - 60, 70, 50));
	bSave.Move(D2D1::RectU(rc.right - 180, rc.bottom - 60, 80, 20));
	bLoad.Move(D2D1::RectU(rc.right - 90, rc.bottom - 60, 80, 20));
	FileName.Move(D2D1::RectU(rc.right - 410, rc.bottom - 30, 400, 20));
}

void MainWindow::GetBounds()
{
	WCHAR* pEnd = NULL;
	WCHAR bounds[4][20];
	FLOAT fbounds[4] = { 0, 0, 0, 0 };
	GetWindowText(MinX.GetHWND(), bounds[0], 9);
	GetWindowText(MaxX.GetHWND(), bounds[1], 9);
	GetWindowText(MinY.GetHWND(), bounds[2], 9);
	GetWindowText(MaxY.GetHWND(), bounds[3], 9);

	//MessageBox(m_hwnd, L"EROR", L"EROR", MB_OK);
	
	for (int i = 0; i < 4; i++)
	{
		fbounds[i] = wcstof(bounds[i], &pEnd);
	}

	m_Graph.SetMinMax(D2D1::RectF(fbounds[0], fbounds[3], fbounds[1], fbounds[2]));
	m_Graph.SetScale();
	m_Graph.SetAxis();
}

void TextInputWindow::Create(FLOAT def, D2D1_RECT_U rect, HWND hParent)
{
	WCHAR buf[12];
	swprintf_s(buf, L"%.2f", def);


	m_hwnd = CreateWindowExW(WS_EX_RIGHT, L"EDIT", buf, WS_BORDER | WS_CHILD | WS_VISIBLE, 
		                     rect.left, rect.top, rect.right, rect.bottom, hParent, NULL, NULL, NULL);
}

void ButtonWindow::Create(INT ID, LPCWSTR title, D2D1_RECT_U rect, HWND hParent)
{
	m_hwnd = CreateWindow(L"BUTTON", title, BS_CENTER | WS_BORDER | WS_CHILD | WS_VISIBLE,
		rect.left, rect.top, rect.right, rect.bottom, hParent, (HMENU)ID, NULL, NULL);
}

void StaticWindow::Create(LPCWSTR title, D2D1_RECT_U rect, HWND hParent)
{
	m_hwnd = CreateWindowEx(WS_EX_RIGHT, L"STATIC", title, WS_BORDER | WS_CHILD | WS_VISIBLE,
		rect.left, rect.top, rect.right, rect.bottom, hParent, NULL, NULL, NULL);
}

void StaticWindow::ChangeTitle(LPWSTR nTitle)
{
	WCHAR* next_token = NULL;
	WCHAR* token = wcstok_s(nTitle, L"\\", &next_token);
	LPWSTR result = nTitle;

	while (token != NULL)
	{
		result = token;
		token = wcstok_s(NULL, L"\\", &next_token);
	}

	SetWindowTextW(m_hwnd, result);
}

void UIWindow::Paint()
{
	InvalidateRect(m_hwnd, NULL, TRUE);
	UpdateWindow(m_hwnd);
}

void UIWindow::Move(D2D1_RECT_U rect)
{
	MoveWindow(m_hwnd, rect.left, rect.top, rect.right, rect.bottom, FALSE);
}
