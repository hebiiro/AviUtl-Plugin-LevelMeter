#include "pch.h"
#include "App.h"

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	App app;

	return app.WinMain(instance, prevInstance, cmdLine, cmdShow);
}
