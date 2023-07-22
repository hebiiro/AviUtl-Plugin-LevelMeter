#pragma once

//
// このメッセージはインプロセスウィンドウの初期化が完了したとき、アウトプロセスに送られます。
// その後、アウトプロセスウィンドウの初期化が完了したとき、インプロセスウィンドウに送り返されます。
// WPARAM == HWND, LPARAM == PainterType です。
//
static const UINT WM_AVIUTL_FILTER_INIT = ::RegisterWindowMessage(_T("WM_AVIUTL_FILTER_INIT"));

//
// このメッセージはアウトプロセスウィンドウの再描画が必要なとき、
// インプロセスからアウトプロセスウィンドウへ送られます。
//
static const UINT WM_AVIUTL_FILTER_REDRAW = ::RegisterWindowMessage(_T("WM_AVIUTL_FILTER_REDRAW"));

//
// このメッセージはアウトプロセスウィンドウのコンフィグダイアログを表示する必要があるとき、
// インプロセスからアウトプロセスウィンドウへ送られます。
//
static const UINT WM_AVIUTL_FILTER_CONFIG = ::RegisterWindowMessage(_T("WM_AVIUTL_FILTER_CONFIG"));

//
// この構造体はアウトプロセスで WM_AVIUTL_FILTER_REDRAW をハンドルしたとき、共有メモリから読み込まれます。
//
struct Volume
{
	DWORD time;
	int frame;
	double level[2];
	double peak[2];
};

//
// この構造体は疑似列挙型です。
// WM_AVIUTL_FILTER_INIT で使用されます。
//
struct PainterType
{
	static const int Both = 0;
	static const int Left = 1;
	static const int Right = 2;
};
