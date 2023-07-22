#include "pch.h"
#include "App.h"

#define PLUGIN_VERSION "2.2.0"

// デバッグ用コールバック関数。デバッグメッセージを出力する。
void ___outputLog(LPCTSTR text, LPCTSTR output)
{
	::OutputDebugString(output);
}

BOOL func_init(AviUtl::FilterPlugin* fp)
{
	return app.func_init(fp);
}

BOOL func_exit(AviUtl::FilterPlugin* fp)
{
	return app.func_exit(fp);
}

BOOL func_proc(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip)
{
	return app.func_proc(fp, fpip);
}

BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	return app.func_WndProc(hwnd, message, wParam, lParam, editp, fp);
}

EXTERN_C AviUtl::FilterPluginDLL** WINAPI GetFilterTableList()
{
	static AviUtl::FilterPluginDLL filter_audio =
	{
		.flag =
			AviUtl::FilterPlugin::Flag::AudioFilter |
			AviUtl::FilterPlugin::Flag::AlwaysActive |
			AviUtl::FilterPlugin::Flag::WindowThickFrame |
			AviUtl::FilterPlugin::Flag::WindowSize |
			AviUtl::FilterPlugin::Flag::ExInformation,
		.x = 200,
		.y = 400,
		.name = "レベルメーター(音声)",
		.func_proc = func_proc,
		.func_init = func_init,
		.func_exit = func_exit,
		.func_WndProc = func_WndProc,
		.information = "レベルメーター(音声) " PLUGIN_VERSION " by 蛇色",
	};

	static AviUtl::FilterPluginDLL* filters[] =
	{
		&filter_audio,
		0,
	};

	return filters;
}
