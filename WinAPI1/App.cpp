#include "App.h"

int App::Init()
{
	srand(time(0));
	if (!win.Create(L"Hello WinAPI", WS_OVERLAPPEDWINDOW))
	{
		return 1;
	}
	return 0;
}

int App::OnExec(int nCmdShow)
{
	if (Init())
		return 1;

	ShowWindow(win.Window(), nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	App app;

	return app.OnExec(nCmdShow);
}