﻿#include "pch.h"
#include "MainWindow.h"
#include "Common/Tracer2.h"

//--------------------------------------------------------------------

HINSTANCE g_instance = 0;
HWND g_parent = 0;

//--------------------------------------------------------------------

ATOM registerClass()
{
	WNDCLASSEXW wc = { sizeof(wc) };
	wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc    = MainWindow::wndProc;
	wc.hInstance      = g_instance;
	wc.hCursor        = ::LoadCursor(0, IDC_ARROW);
	wc.lpszClassName  = L"LevelMeter.OutProcess";

	return ::RegisterClassExW(&wc);
}

BOOL initInstance()
{
	if (!registerClass())
		return FALSE;

	::CreateWindowExW(
		WS_EX_NOPARENTNOTIFY,
		L"LevelMeter.OutProcess",
		L"LevelMeter.OutProcess",
		WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, 0, 0,
		g_parent, 0, g_instance, 0);

	if (!g_mainWindow.m_hwnd)
		return FALSE;

	return TRUE;
}

void CALLBACK timerProc(HWND hwnd, UINT message, UINT_PTR timerId, DWORD time)
{
	if (!::IsWindow(g_parent))
	{
		MY_TRACE(_T("親ウィンドウが無効になりました\n"));

		::PostQuitMessage(0);
	}
}

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	struct Initializer
	{
		Initializer()
		{
			_tsetlocale(LC_ALL, _T(""));
			trace_init(0, 0, TRUE);
			::CoInitialize(0);
		}

		~Initializer()
		{
			trace_term();
			::CoUninitialize();
		}

	} initializer;

	MY_TRACE(_T("WinMain()\n"));

	g_parent = (HWND)_tcstoul(::GetCommandLine(), 0, 0);
	MY_TRACE_HEX(g_parent);

	if (!initInstance())
		return 0;

	MY_TRACE_HEX(g_mainWindow.m_hwnd);

	// InProcess にウィンドウハンドルを渡す。
	::PostMessage(g_parent, WM_AVIUTL_FILTER_INIT, (WPARAM)g_mainWindow.m_hwnd, 0);

	// InProcess を監視するタイマーを作成する。
	// メインウィンドウは削除されるかもしれないので WM_TIMER は使用できない。
	::SetTimer(0, 0, 1000, timerProc);

	MSG msg = {};
	while (::GetMessage(&msg, 0, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	g_mainWindow.save();

	MY_TRACE(_T("プロセスが正常終了しました\n"));

	return 0;
}

//--------------------------------------------------------------------
