#pragma once

//--------------------------------------------------------------------

struct CommandID
{
	static const UINT ENABLE_MODE_OFF = 1000;
	static const UINT ENABLE_MODE_ON = 1001;
	static const UINT ENABLE_MODE_ON_WITHOUT_PLAYING = 1002;
	static const UINT CONFIG = 2022;
};

struct EnableMode
{
	static const int OFF = 0;
	static const int ON = 1;
	static const int ON_WITHOUT_PLAYING = 2;
};

//--------------------------------------------------------------------

extern AviUtl::FilterPlugin* g_fp;
extern int g_enableMode;

//--------------------------------------------------------------------

void onContextMenu(HWND hwnd);
void loadConfig();
void saveConfig();

//--------------------------------------------------------------------
