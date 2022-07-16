#pragma once

//--------------------------------------------------------------------

struct CommandID
{
	static const UINT CONFIG = 2022;
};

struct DragMode
{
	static const int minRange = 0;
	static const int maxRange = 1;
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
