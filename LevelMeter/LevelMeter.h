#pragma once

//--------------------------------------------------------------------

struct TimerID
{
	static const UINT REDRAW = 2022;
};

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

struct DragMode
{
	static const int MIN_RANGE = 0;
	static const int MAX_RANGE = 1;
};

struct AudioData
{
	int32_t frame_n;
	int32_t audio_rate;
	int32_t audio_ch;
	int32_t audio_n;
	short* audiop;
};

//--------------------------------------------------------------------

extern AviUtl::FilterPlugin* g_fp;
extern HTHEME g_theme;

extern double g_level[2];
extern double g_peak[2];

extern int g_enableMode;
extern COLORREF g_fillColor;
extern COLORREF g_rmsColor0;
extern COLORREF g_rmsColor1;
extern COLORREF g_peakColor;
extern int g_peakWidth;
extern COLORREF g_guageColor;
extern int g_guageWidth;
extern COLORREF g_guageTextColor;
extern COLORREF g_separatorColor;
extern int g_zebraStep;
extern int g_zebraWidth;
extern COLORREF g_zebraColor;
extern int g_minRange;
extern int g_maxRange;
extern BOOL g_useTheme;
extern BOOL g_immediate;

extern int g_dragMode;
extern POINT g_dragOriginPoint;
extern int g_dragOriginRange;

//--------------------------------------------------------------------

BOOL onProc(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip, AudioData* data);
void onPaint(HWND hwnd, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp);
void onContextMenu(HWND hwnd);
void onConfigDialog(HWND hwnd);

void loadConfig();
void saveConfig();

//--------------------------------------------------------------------
