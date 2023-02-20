#include "pch.h"
#include "LevelMeter.h"
#include "OutProcess.h"

//--------------------------------------------------------------------

// デバッグ用コールバック関数。デバッグメッセージを出力する。
void ___outputLog(LPCTSTR text, LPCTSTR output)
{
	::OutputDebugString(output);
}

//--------------------------------------------------------------------

AviUtl::FilterPlugin* g_fp = 0;
int g_enableMode = EnableMode::ON;

//--------------------------------------------------------------------

void onContextMenu(HWND hwnd)
{
	POINT cursorPos; ::GetCursorPos(&cursorPos);

	HMENU menu = ::CreatePopupMenu();

	::AppendMenu(menu, MF_STRING, CommandID::ENABLE_MODE_ON, _T("有効"));
	::AppendMenu(menu, MF_STRING, CommandID::ENABLE_MODE_ON_WITHOUT_PLAYING, _T("有効(再生時以外)"));
	::AppendMenu(menu, MF_STRING, CommandID::ENABLE_MODE_OFF, _T("無効"));
	::AppendMenu(menu, MF_SEPARATOR, 0, 0);
	::AppendMenu(menu, MF_STRING, CommandID::CONFIG, _T("設定"));

	switch (g_enableMode)
	{
	case EnableMode::OFF: ::CheckMenuItem(menu, CommandID::ENABLE_MODE_OFF, MF_CHECKED); break;
	case EnableMode::ON: ::CheckMenuItem(menu, CommandID::ENABLE_MODE_ON, MF_CHECKED); break;
	case EnableMode::ON_WITHOUT_PLAYING: ::CheckMenuItem(menu, CommandID::ENABLE_MODE_ON_WITHOUT_PLAYING, MF_CHECKED); break;
	}

	int id = ::TrackPopupMenu(menu, TPM_NONOTIFY | TPM_RETURNCMD, cursorPos.x, cursorPos.y, 0, hwnd, 0);

	switch (id)
	{
	case CommandID::ENABLE_MODE_OFF:
		{
			g_enableMode = EnableMode::OFF;

			break;
		}
	case CommandID::ENABLE_MODE_ON:
		{
			g_enableMode = EnableMode::ON;

			break;
		}
	case CommandID::ENABLE_MODE_ON_WITHOUT_PLAYING:
		{
			g_enableMode = EnableMode::ON_WITHOUT_PLAYING;

			break;
		}
	case CommandID::CONFIG:
		{
			g_outProcess.config(g_fp);

			break;
		}
	}

	::DestroyMenu(menu);
}

//--------------------------------------------------------------------

void loadConfig()
{
	WCHAR fileName[MAX_PATH] = {};
	::GetModuleFileNameW(g_fp->dll_hinst,  fileName, MAX_PATH);
	::PathRemoveExtensionW(fileName);
	::PathAppendW(fileName, L"LevelMeter.ini");
	MY_TRACE_WSTR(fileName);

	getPrivateProfileInt(fileName, L"Config", L"enableMode", g_enableMode);
}

void saveConfig()
{
	WCHAR fileName[MAX_PATH] = {};
	::GetModuleFileNameW(g_fp->dll_hinst,  fileName, MAX_PATH);
	::PathRemoveExtensionW(fileName);
	::PathAppendW(fileName, L"LevelMeter.ini");
	MY_TRACE_WSTR(fileName);

	setPrivateProfileInt(fileName, L"Config", L"enableMode", g_enableMode);
}

//--------------------------------------------------------------------
