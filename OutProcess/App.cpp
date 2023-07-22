#include "pch.h"
#include "App.h"
#include "Hive.h"
#include "View.h"
#include "Common/Tracer2.h"

App::App()
{
	_tsetlocale(LC_CTYPE, _T(""));
	trace_init(0, 0, TRUE);
	::CoInitialize(0);

	MY_TRACE(_T("App::App()\n"));
}

App::~App()
{
	MY_TRACE(_T("App::~App()\n"));

	trace_term();
	::CoUninitialize();
}

BOOL App::init(HWND director)
{
	MY_TRACE(_T("App::init(0x%08X)\n"), director);

	hive.init(director);
	hive.load();

	main_process_checker.init();
	config_file_checker.init();

	return TRUE;
}

BOOL App::exit()
{
	MY_TRACE(_T("App::exit()\n"));

	config_file_checker.exit();
	main_process_checker.exit();

	hive.save();
	hive.exit();

	return TRUE;
}

BOOL App::messageLoop()
{
	MY_TRACE(_T("App::messageLoop()\n"));

	MSG msg = {};
	while (::GetMessage(&msg, 0, 0, 0) > 0)
	{
		// WM_AVIUTL_FILTER_INIT (スレッドメッセージ) をハンドルします。
		if (msg.hwnd == 0 && msg.message == WM_AVIUTL_FILTER_INIT)
		{
			HWND pluginWindow = (HWND)msg.wParam;
			int painterType = (int)msg.lParam;

			// レベルメーターを描画するウィンドウを作成します。
			auto painter = view.createPainter(pluginWindow, painterType);
			if (painter)
			{
				// インプロセスウィンドウにアウトプロセスウィンドウのハンドルを渡します。
				::PostMessage(pluginWindow, WM_AVIUTL_FILTER_INIT, (WPARAM)(HWND)*painter, 0);
			}

			continue;
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return TRUE;
}

int App::WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	MY_TRACE(_T("App::WinMain()\n"));

	if (!init((HWND)_tcstoul(::GetCommandLine(), 0, 0))) return 0;
	if (!messageLoop()) return 0;
	if (!exit()) return 0;

	return 0;
}

BOOL MainProcessChecker::init()
{
	MY_TRACE(_T("MainProcessChecker::init()\n"));

	// メインプロセス(ディレクターウィンドウ)を監視するタイマーを作成します。
	// カレントプロセス内のウィンドウが破壊されたあともチェックしなければならないので
	// WM_TIMER は使用できません。
	// ディレクターウィンドウはハイブに格納されています。
	::SetTimer(0, 0, 1000, timerProc);

	return TRUE;
}

BOOL MainProcessChecker::exit()
{
	MY_TRACE(_T("MainProcessChecker::exit()\n"));

	return TRUE;
}

void CALLBACK MainProcessChecker::timerProc(HWND hwnd, UINT message, UINT_PTR timerId, DWORD time)
{
	if (!::IsWindow(hive.director))
	{
		MY_TRACE(_T("メインプロセス(ディレクターウィンドウ)が無効になりました\n"));

		::PostQuitMessage(0);
	}
}

BOOL ConfigFileChecker::init()
{
	MY_TRACE(_T("ConfigFileChecker::init()\n"));

	// ハイブからコンフィグファイルのファイル名を取得します。
	WCHAR fileName[MAX_PATH] = {};
	hive.getConfigFileName(fileName, std::size(fileName));
	MY_TRACE_WSTR(fileName);

	// ファイルアップデートチェッカー(スーパークラス)を初期化します。
	__super::init(fileName);

	// コンフィグファイルを監視するタイマーを作成します。
	::SetTimer(0, 0, 1000, timerProc);

	return TRUE;
}

BOOL ConfigFileChecker::exit()
{
	MY_TRACE(_T("ConfigFileChecker::exit()\n"));

	return TRUE;
}

void CALLBACK ConfigFileChecker::timerProc(HWND hwnd, UINT message, UINT_PTR timerId, DWORD time)
{
	if (config_file_checker.isFileUpdated())
	{
		MY_TRACE(_T("コンフィグファイルが更新されました\n"));

		hive.load(); // ハイブにコンフィグファイルを再ロードさせます。(ドキュメントの再読み込み)
		view.updateDesign(); // ビューのデザインを更新します。(ビューの再描画)
	}
}
