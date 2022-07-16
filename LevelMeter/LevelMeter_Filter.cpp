#include "pch.h"
#include "LevelMeter.h"

//--------------------------------------------------------------------

BOOL func_init(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("func_init()\n"));

	g_fp = fp;

	loadConfig();

	return TRUE;
}

BOOL func_exit(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("func_exit()\n"));

	saveConfig();

	return TRUE;
}

BOOL func_proc(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip)
{
	MY_TRACE(_T("func_proc()\n"));

	if (fp->exfunc->is_saving(fpip->editp))
		return FALSE; // 保存のときは何もしない。

	// ファイル情報を取得する。
	AviUtl::FileInfo fi = {};
	fp->exfunc->get_file_info(fpip->editp, &fi);

	// 音声バッファのサイズを取得する。
	int c = fp->exfunc->get_audio_filtered(fpip->editp, fpip->frame, 0);

	// 音声バッファを取得する。
	std::vector<short> buffer(c * fi.audio_ch);
	fp->exfunc->get_audio_filtered(fpip->editp, fpip->frame, buffer.data());

	// 音声データを構造体にまとめる。
	AudioData data = {};
	data.frame_n = fi.frame_n;
	data.audio_rate = fi.audio_rate;
	data.audio_ch = fi.audio_ch;
	data.audio_n = c;
	data.audiop = buffer.data();

	// メイン処理を行う。
	return onProc(fp, fpip, &data);
}

BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
//	MY_TRACE(_T("func_WndProc(0x%08X, 0x%08X, 0x%08X)\n"), message, wParam, lParam);

	switch (message)
	{
	case AviUtl::detail::FilterPluginWindowMessage::Init:
		{
			MY_TRACE(_T("func_WndProc(Init, 0x%08X, 0x%08X)\n"), wParam, lParam);

			g_theme = ::OpenThemeData(hwnd, VSCLASS_WINDOW);
			MY_TRACE_HEX(g_theme);

			break;
		}
	case AviUtl::detail::FilterPluginWindowMessage::Exit:
		{
			MY_TRACE(_T("func_WndProc(Exit, 0x%08X, 0x%08X)\n"), wParam, lParam);

			::CloseThemeData(g_theme), g_theme = 0;

			break;
		}
	case WM_SIZE:
		{
			MY_TRACE(_T("func_WndProc(WM_SIZE, 0x%08X, 0x%08X)\n"), wParam, lParam);

			break;
		}
	case WM_TIMER:
		{
			if (wParam == TimerID::REDRAW)
			{
				::KillTimer(hwnd, wParam);

				onPaint(hwnd, editp, fp);
			}

			break;
		}
	case WM_PAINT:
		{
			MY_TRACE(_T("func_WndProc(WM_PAINT, 0x%08X, 0x%08X)\n"), wParam, lParam);

			onPaint(hwnd, editp, fp);

			break;
		}
	case WM_LBUTTONDOWN:
		{
			MY_TRACE(_T("func_WndProc(WM_LBUTTONDOWN, 0x%08X, 0x%08X)\n"), wParam, lParam);

			::SetCapture(hwnd);
			g_dragOriginPoint = LP2PT(lParam);

			RECT rc; ::GetClientRect(hwnd, &rc);
			int cy = (rc.top + rc.bottom) / 2;

			if (g_dragOriginPoint.y < cy)
			{
				g_dragMode = DragMode::maxRange;
				g_dragOriginRange = g_maxRange;
			}
			else
			{
				g_dragMode = DragMode::minRange;
				g_dragOriginRange = g_minRange;
			}

			break;
		}
	case WM_MOUSEMOVE:
		{
//			MY_TRACE(_T("func_WndProc(WM_MOUSEMOVE, 0x%08X, 0x%08X)\n"), wParam, lParam);

			if (::GetCapture() == hwnd)
			{
				POINT point = LP2PT(lParam);
				int offset = point.y - g_dragOriginPoint.y;

				switch (g_dragMode)
				{
				case DragMode::maxRange: g_maxRange = g_dragOriginRange + offset / 5; break;
				case DragMode::minRange: g_minRange = g_dragOriginRange + offset / 5; break;
				}

				::InvalidateRect(hwnd, 0, FALSE);
			}

			break;
		}
	case WM_LBUTTONUP:
		{
			MY_TRACE(_T("func_WndProc(WM_LBUTTONUP, 0x%08X, 0x%08X)\n"), wParam, lParam);

			::ReleaseCapture();

			break;
		}
	case WM_CONTEXTMENU:
		{
			MY_TRACE(_T("func_WndProc(WM_CONTEXTMENU, 0x%08X, 0x%08X)\n"), wParam, lParam);

			onContextMenu(hwnd);

			break;
		}
	}

	return FALSE;
}

EXTERN_C AviUtl::FilterPluginDLL* CALLBACK GetFilterTable()
{
	LPCSTR name = "レベルメーター";
	LPCSTR information = "レベルメーター 1.3.0 by 蛇色";

	static AviUtl::FilterPluginDLL filter =
	{
		.flag =
			AviUtl::detail::FilterPluginFlag::AlwaysActive |
//			AviUtl::detail::FilterPluginFlag::DispFilter |
			AviUtl::detail::FilterPluginFlag::WindowThickFrame |
			AviUtl::detail::FilterPluginFlag::WindowSize |
			AviUtl::detail::FilterPluginFlag::ExInformation,
		.x = 200,
		.y = 400,
		.name = name,
		.func_proc = func_proc,
		.func_init = func_init,
		.func_exit = func_exit,
		.func_WndProc = func_WndProc,
		.information = information,
	};

	return &filter;
}

//--------------------------------------------------------------------
