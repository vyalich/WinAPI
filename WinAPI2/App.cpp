#include "App.h"

int App::Init()
{
	if(!MainWin.Create(L"Graph editor", WS_OVERLAPPEDWINDOW))
		return -1;
	return 0;
}

int App::OnExec(int nCmdShow)
{
	if (Init() < 0)
		return -1;

	ShowWindow(MainWin.Window(), nCmdShow);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CleanUp();

	return 0;
}

int App::CleanUp()
{

}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	App app;

	return app.OnExec(nCmdShow);
}