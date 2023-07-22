#pragma once
#include "Hive.h"
#include "OutProcess.h"
#include "Bomber.h"
#include "View.h"

inline struct App
{
	BOOL func_init(AviUtl::FilterPlugin* fp)
	{
		MY_TRACE(_T("func_init()\n"));

		hive.init(fp);

		return TRUE;
	}

	BOOL func_exit(AviUtl::FilterPlugin* fp)
	{
		MY_TRACE(_T("func_exit()\n"));

		hive.exit();

		return TRUE;
	}

	BOOL func_proc(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip)
	{
		MY_TRACE(_T("frame = %d, audio_n = %d\n"), fpip->frame, fpip->audio_n);

		switch (hive.enableMode)
		{
		case Hive::EnableMode::OFF:
			{
				return FALSE; // 無効化されているときは何もしない。
			}
		case Hive::EnableMode::ON_WITHOUT_PLAYING:
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

		return bomber.send(fp, fpip);
	}

	BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
	{
//		MY_TRACE(_T("func_WndProc(0x%08X, 0x%08X, 0x%08X)\n"), message, wParam, lParam);

		switch (message)
		{
		case AviUtl::FilterPlugin::WindowMessage::Init:
			{
				MY_TRACE(_T("func_WndProc(Init, 0x%08X, 0x%08X)\n"), wParam, lParam);

				modifyStyle(hwnd, 0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

				hive.load();
				bomber.init(fp);
				outProcess.init();
				view.init();
				view.load();

				break;
			}
		case AviUtl::FilterPlugin::WindowMessage::Exit:
			{
				MY_TRACE(_T("func_WndProc(Exit, 0x%08X, 0x%08X)\n"), wParam, lParam);

				hive.save();
				view.save();
				view.exit();
				outProcess.exit();
				bomber.exit(fp);

				break;
			}
		}

		return FALSE;
	}
} app;
