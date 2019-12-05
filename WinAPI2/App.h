#pragma once
#include "Window.h"

class App
{
private:
	MainWindow MainWin;

public:
	App() {}
	int	Init();
	int OnExec(int nCmdShow);
	int CleanUp();
};

