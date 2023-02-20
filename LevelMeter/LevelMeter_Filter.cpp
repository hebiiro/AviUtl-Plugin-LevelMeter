#include "pch.h"
#include "LevelMeter.h"
#include "OutProcess.h"
#include "SenderThread.h"

//--------------------------------------------------------------------

#define PLUGIN_VERSION "2.0.0"

//--------------------------------------------------------------------

BOOL func_init(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("func_init()\n"));

	g_fp = fp;

	return TRUE;
}

BOOL func_exit(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("func_exit()\n"));

	return TRUE;
}

BOOL func_proc(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip)
{
	MY_TRACE(_T("frame = %d, audio_n = %d\n"), fpip->frame, fpip->audio_n);

	switch (g_enableMode)
	{
	case EnableMode::OFF:
		{
			return FALSE; // 無効化されているときは何もしない。
		}
	case EnableMode::ON_WITHOUT_PLAYING:
		{
			if ((DWORD)fpip->editp->aviutl_window_info.flag & 0x00040000)
				return FALSE; // 再生中のときは何もしない。

			break;
		}
	}

	if (fp->exfunc->is_saving(fpip->editp))
		return FALSE; // 保存のときは何もしない。

	if (!fp->exfunc->is_filter_window_disp(fp))
		return FALSE; // ウィンドウが非表示のときは何もしない。

	return g_senderThread.send(fp, fpip);
}

BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
//	MY_TRACE(_T("func_WndProc(0x%08X, 0x%08X, 0x%08X)\n"), message, wParam, lParam);

	switch (message)
	{
	case AviUtl::FilterPlugin::WindowMessage::Init:
		{
			MY_TRACE(_T("func_WndProc(Init, 0x%08X, 0x%08X)\n"), wParam, lParam);

			modifyStyle(hwnd, 0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

			loadConfig();
			g_senderThread.init(fp);
			g_outProcess.init(fp);

			break;
		}
	case AviUtl::FilterPlugin::WindowMessage::Exit:
		{
			MY_TRACE(_T("func_WndProc(Exit, 0x%08X, 0x%08X)\n"), wParam, lParam);

			saveConfig();
			g_outProcess.exit(fp);
			g_senderThread.exit(fp);

			break;
		}
	case WM_SIZE:
		{
			MY_TRACE(_T("func_WndProc(WM_SIZE, 0x%08X, 0x%08X)\n"), wParam, lParam);

			g_outProcess.resize(fp);

			break;
		}
	case WM_CONTEXTMENU:
		{
			MY_TRACE(_T("func_WndProc(WM_CONTEXTMENU, 0x%08X, 0x%08X)\n"), wParam, lParam);

			onContextMenu(hwnd);

			break;
		}
	}

	if (message == WM_AVIUTL_FILTER_INIT)
	{
		MY_TRACE(_T("func_WndProc(WM_AVIUTL_FILTER_INIT, 0x%08X, 0x%08X)\n"), wParam, lParam);

		g_outProcess.m_mainWindow = (HWND)wParam;
		g_outProcess.resize(fp);
	}

	return FALSE;
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

//--------------------------------------------------------------------
