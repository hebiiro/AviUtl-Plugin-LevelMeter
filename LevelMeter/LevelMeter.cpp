#include "pch.h"
#include "LevelMeter.h"
#include "ConfigDialog.h"

//--------------------------------------------------------------------

// デバッグ用コールバック関数。デバッグメッセージを出力する。
void ___outputLog(LPCTSTR text, LPCTSTR output)
{
	::OutputDebugString(output);
}

//--------------------------------------------------------------------

AviUtl::FilterPlugin* g_fp = 0;
HTHEME g_theme = 0;

double g_level[2] = {};
double g_peak[2] = {};

int g_dragMode = DragMode::minRange;
POINT g_dragOriginPoint = {};
int g_dragOriginRange = 0;

COLORREF g_fillColor = RGB(20, 60, 200);
COLORREF g_rmsColor0 = RGB(251, 218, 17);
COLORREF g_rmsColor1 = RGB(183, 249, 19);
COLORREF g_peakColor = RGB(255, 0, 0);
int g_peakWidth = 2;
COLORREF g_guageColor = RGB(220, 220, 220);
int g_guageWidth = 1;
COLORREF g_guageTextColor = RGB(220, 220, 220);
COLORREF g_separatorColor = RGB(0, 0, 0);
int g_minRange = -33;
int g_maxRange = 14;
BOOL g_useTheme = FALSE;

//--------------------------------------------------------------------

inline double normalizedSqr(short pcm)
{
	double normalized = pcm / 32768.0; // -1.0 ～ 1.0に正規化
	return normalized * normalized;
}

inline double normalizedAbs(short pcm)
{
	double normalized = pcm / 32768.0; // -1.0 ～ 1.0に正規化
	return fabs(normalized);
}

BOOL onProc(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip, AudioData* data)
{
	if (data->audio_ch != 1 && data->audio_ch != 2)
		return FALSE;

	for (int ch = 0; ch < data->audio_ch; ch++)
	{
		g_level[ch] = 0.0;
		g_peak[ch] = 0.0;

		for (int i = 0; i < data->audio_n * 2; i += 2)
		{
			g_level[ch] += normalizedSqr(data->audiop[i + ch]);

			double na = normalizedAbs(data->audiop[i + ch]);

			if (g_peak[ch] < na) g_peak[ch] = na;
		}

		g_level[ch] = sqrt(g_level[ch] / data->audio_n);

		g_level[ch] = 20 * log10(g_level[ch]);
		g_peak[ch] = 20 * log10(g_peak[ch]);
	}

	onPaint(fp->hwnd, fpip->editp, fp);

	return TRUE;
}

//--------------------------------------------------------------------

inline COLOR16 getR(COLORREF color)
{
	return (COLOR16)(GetRValue(color) << 8);
}

inline COLOR16 getG(COLORREF color)
{
	return (COLOR16)(GetGValue(color) << 8);
}

inline COLOR16 getB(COLORREF color)
{
	return (COLOR16)(GetBValue(color) << 8);
}

inline void fill(HDC dc, LPCRECT rc, COLORREF begin, COLORREF end)
{
	TRIVERTEX av[2] =
	{
		rc->left, rc->top, getR(begin), getG(begin), getB(begin), 0xff00,
		rc->right, rc->bottom, getR(end), getG(end), getB(end), 0xff00,
	};
	GRADIENT_RECT grc = { 0, 1 };
	::GradientFill(dc, av, 2, &grc, 1, GRADIENT_FILL_RECT_V);
}

void onPaint(HWND hwnd, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	ClientDC _dc(hwnd);
	RECT rc; ::GetClientRect(hwnd, &rc);
	UxDC dc(_dc, &rc);

	if (!dc.isValid())
		return;

	AviUtl::SysInfo si = {};
	fp->exfunc->get_sys_info(editp, &si);

	GdiObjSelector fontSelector(dc, si.hfont);

	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;

	if (g_useTheme)
	{
		// テーマ API を使用して背景を描画する。
		::DrawThemeBackground(g_theme, dc, WP_DIALOG, ETS_NORMAL, &rc, 0);
	}
	else
	{
		// 背景を描画する。

		GdiObj<HBRUSH> brush = ::CreateSolidBrush(g_fillColor);

		::FillRect(dc, &rc, brush);
	}

	{
		// RMS とピークを描画する。

		RECT rects[2] = { rc, rc };
		rects[0].left = rc.left + ::MulDiv(w, 1, 4);
		rects[0].right = rc.left + ::MulDiv(w, 2, 4);
		rects[1].left = rc.left + ::MulDiv(w, 2, 4);
		rects[1].right = rc.left + ::MulDiv(w, 3, 4);

		for (int i = 0; i < 2; i++)
		{
			int top = rects[i].top;
			int bottom = rects[i].bottom;
			int h = rects[i].bottom - rects[i].top;

			if (g_level[i] > g_minRange)
			{
				double level = (g_level[i] - g_minRange) / (g_maxRange - g_minRange);

				rects[i].top = bottom - (int)(h * level);

				fill(dc, &rects[i], g_rmsColor0, g_rmsColor1);
			}

			if (g_peak[i] > g_minRange)
			{
				double peak = (g_peak[i] - g_minRange) / (g_maxRange - g_minRange);

				GdiObj<HPEN> pen = ::CreatePen(PS_SOLID, g_peakWidth, g_peakColor);
				GdiObjSelector penSelector(dc, pen);

				int peakY = bottom - (int)(h * peak);

				::MoveToEx(dc, rects[i].left, peakY, 0);
				::LineTo(dc, rects[i].right, peakY);
			}
		}
	}

	{
		// ゲージを描画する。

		RECT rcSize = {}; ::DrawText(dc, _T("g"), 1, &rcSize, DT_CALCRECT);
		SIZE size =
		{
			rcSize.right - rcSize.left,
			rcSize.bottom - rcSize.top,
		};

		int range = g_maxRange - g_minRange;
		int freq = range * size.cy / h + 1;

		GdiObj<HPEN> pen = ::CreatePen(PS_SOLID, g_guageWidth, g_guageColor);
		GdiObjSelector penSelector(dc, pen);

		::SetBkMode(dc, TRANSPARENT);
		::SetTextColor(dc, g_guageTextColor);

		for (int i = g_maxRange; i > g_minRange; i--)
		{
			if (i % freq) continue;

			double di = i - g_minRange;

			RECT rcText = rc;
			rcText.top = (int)(rc.top + h * (1.0 - di / range));
			rcText.bottom = rcText.top + size.cy;

			::MoveToEx(dc, rc.left, rcText.bottom, 0);
			::LineTo(dc, rc.left + ::MulDiv(w, 1, 4), rcText.bottom);

			::MoveToEx(dc, rc.right, rcText.bottom, 0);
			::LineTo(dc, rc.left + ::MulDiv(w, 3, 4), rcText.bottom);

			WCHAR text[MAX_PATH] = {};
			::StringCbPrintfW(text, sizeof(text), L"%+d", i);

			if (g_useTheme)
			{
				::DrawThemeText(g_theme, dc, WP_DIALOG, ETS_NORMAL,
					text, ::lstrlenW(text), DT_LEFT | DT_NOCLIP, 0, &rcText);
				::DrawThemeText(g_theme, dc, WP_DIALOG, ETS_NORMAL,
					text, ::lstrlenW(text), DT_RIGHT | DT_NOCLIP, 0, &rcText);
			}
			else
			{
				::DrawTextW(dc, text, ::lstrlenW(text), &rcText, DT_LEFT | DT_NOCLIP);
				::DrawTextW(dc, text, ::lstrlenW(text), &rcText, DT_RIGHT | DT_NOCLIP);
			}
		}
	}

	{
		// セパレータを描画する。

		GdiObj<HPEN> pen = ::CreatePen(PS_SOLID, 1, g_separatorColor);
		GdiObjSelector penSelector(dc, pen);

		for (int i = 2; i <= 2; i++)
		{
			::MoveToEx(dc, rc.left + ::MulDiv(w, i, 4), rc.top, 0);
			::LineTo(dc, rc.left + ::MulDiv(w, i, 4), rc.bottom);
		}
	}
}

//--------------------------------------------------------------------

void onContextMenu(HWND hwnd)
{
	POINT cursorPos; ::GetCursorPos(&cursorPos);

	HMENU menu = ::CreatePopupMenu();

	::AppendMenu(menu, MF_STRING, CommandID::CONFIG, _T("設定"));

	int id = ::TrackPopupMenu(menu, TPM_NONOTIFY | TPM_RETURNCMD, cursorPos.x, cursorPos.y, 0, hwnd, 0);

	switch (id)
	{
	case CommandID::CONFIG:
		{
			onConfigDialog(hwnd);

			break;
		}
	}

	::DestroyMenu(menu);
}

//--------------------------------------------------------------------

void onConfigDialog(HWND hwnd)
{
	ConfigDialog dialog(hwnd);

	::SetDlgItemInt(dialog, IDC_FILL_COLOR, g_fillColor, FALSE);
	::SetDlgItemInt(dialog, IDC_RMS_COLOR_0, g_rmsColor0, FALSE);
	::SetDlgItemInt(dialog, IDC_RMS_COLOR_1, g_rmsColor1, FALSE);
	::SetDlgItemInt(dialog, IDC_PEAK_COLOR, g_peakColor, FALSE);
	::SetDlgItemInt(dialog, IDC_PEAK_WIDTH, g_peakWidth, FALSE);
	::SetDlgItemInt(dialog, IDC_GUAGE_COLOR, g_guageColor, FALSE);
	::SetDlgItemInt(dialog, IDC_GUAGE_WIDTH, g_guageWidth, FALSE);
	::SetDlgItemInt(dialog, IDC_GUAGE_TEXT_COLOR, g_guageTextColor, FALSE);
	::SetDlgItemInt(dialog, IDC_SEPARATOR_COLOR, g_separatorColor, FALSE);
	::SetDlgItemInt(dialog, IDC_MIN_RANGE, g_minRange, TRUE);
	::SetDlgItemInt(dialog, IDC_MAX_RANGE, g_maxRange, TRUE);
	HWND hwndUseTheme = ::GetDlgItem(dialog, IDC_USE_THEME);
	Button_SetCheck(hwndUseTheme, g_useTheme);

	int retValue = dialog.doModal();

	if (IDOK != retValue)
		return;

	g_fillColor = ::GetDlgItemInt(dialog, IDC_FILL_COLOR, 0, FALSE);
	g_rmsColor0 = ::GetDlgItemInt(dialog, IDC_RMS_COLOR_0, 0, FALSE);
	g_rmsColor1 = ::GetDlgItemInt(dialog, IDC_RMS_COLOR_1, 0, FALSE);
	g_peakColor = ::GetDlgItemInt(dialog, IDC_PEAK_COLOR, 0, FALSE);
	g_peakWidth = ::GetDlgItemInt(dialog, IDC_PEAK_WIDTH, 0, FALSE);
	g_guageColor = ::GetDlgItemInt(dialog, IDC_GUAGE_COLOR, 0, FALSE);
	g_guageWidth = ::GetDlgItemInt(dialog, IDC_GUAGE_WIDTH, 0, FALSE);
	g_guageTextColor = ::GetDlgItemInt(dialog, IDC_GUAGE_TEXT_COLOR, 0, FALSE);
	g_separatorColor = ::GetDlgItemInt(dialog, IDC_SEPARATOR_COLOR, 0, FALSE);
	g_minRange = ::GetDlgItemInt(dialog, IDC_MIN_RANGE, 0, TRUE);
	g_maxRange = ::GetDlgItemInt(dialog, IDC_MAX_RANGE, 0, TRUE);
	g_useTheme = Button_GetCheck(hwndUseTheme);

	::InvalidateRect(hwnd, 0, FALSE);
}

//--------------------------------------------------------------------

void loadConfig()
{
	WCHAR fileName[MAX_PATH] = {};
	::GetModuleFileNameW(g_fp->dll_hinst,  fileName, MAX_PATH);
	::PathRenameExtensionW(fileName, L".ini");

	getPrivateProfileColor(fileName, L"Config", L"fillColor", g_fillColor);
	getPrivateProfileColor(fileName, L"Config", L"rmsColor0", g_rmsColor0);
	getPrivateProfileColor(fileName, L"Config", L"rmsColor1", g_rmsColor1);
	getPrivateProfileColor(fileName, L"Config", L"peakColor", g_peakColor);
	getPrivateProfileInt(fileName, L"Config", L"peakWidth", g_peakWidth);
	getPrivateProfileColor(fileName, L"Config", L"guageColor", g_guageColor);
	getPrivateProfileInt(fileName, L"Config", L"guageWidth", g_guageWidth);
	getPrivateProfileColor(fileName, L"Config", L"guageTextColor", g_guageTextColor);
	getPrivateProfileColor(fileName, L"Config", L"separatorColor", g_separatorColor);
	getPrivateProfileInt(fileName, L"Config", L"minRange", g_minRange);
	getPrivateProfileInt(fileName, L"Config", L"maxRange", g_maxRange);
	getPrivateProfileBool(fileName, L"Config", L"useTheme", g_useTheme);
}

void saveConfig()
{
	WCHAR fileName[MAX_PATH] = {};
	::GetModuleFileNameW(g_fp->dll_hinst,  fileName, MAX_PATH);
	::PathRenameExtensionW(fileName, L".ini");

	setPrivateProfileColor(fileName, L"Config", L"fillColor", g_fillColor);
	setPrivateProfileColor(fileName, L"Config", L"rmsColor0", g_rmsColor0);
	setPrivateProfileColor(fileName, L"Config", L"rmsColor1", g_rmsColor1);
	setPrivateProfileColor(fileName, L"Config", L"peakColor", g_peakColor);
	setPrivateProfileInt(fileName, L"Config", L"peakWidth", g_peakWidth);
	setPrivateProfileColor(fileName, L"Config", L"guageColor", g_guageColor);
	setPrivateProfileInt(fileName, L"Config", L"guageWidth", g_guageWidth);
	setPrivateProfileColor(fileName, L"Config", L"guageTextColor", g_guageTextColor);
	setPrivateProfileColor(fileName, L"Config", L"separatorColor", g_separatorColor);
	setPrivateProfileInt(fileName, L"Config", L"minRange", g_minRange);
	setPrivateProfileInt(fileName, L"Config", L"maxRange", g_maxRange);
	setPrivateProfileBool(fileName, L"Config", L"useTheme", g_useTheme);
}

//--------------------------------------------------------------------
