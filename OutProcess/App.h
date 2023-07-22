#pragma once

struct App
{
	App();
	~App();
	BOOL init(HWND director);
	BOOL exit();
	BOOL messageLoop();
	int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow);
};

//
// このクラスはメインプロセスを監視します。
// メインプロセスが無効になった場合はカレントプロセスを終了します。
// これにより、メインプロセスが強制終了したとしても、
// サブプロセス(カレントプロセス)の残骸が残らないようになります。
//
inline struct MainProcessChecker
{
	BOOL init();
	BOOL exit();
	static void CALLBACK timerProc(HWND hwnd, UINT message, UINT_PTR timerId, DWORD time);
} main_process_checker;

//
// このクラスはコンフィグファイルの更新を監視します。
// これにより、ユーザーがコンフィグファイルを直接編集した場合でも、
// 変更が即時に反映されるようになります。
//
inline struct ConfigFileChecker : FileUpdateChecker
{
	BOOL init();
	BOOL exit();
	static void CALLBACK timerProc(HWND hwnd, UINT message, UINT_PTR timerId, DWORD time);
} config_file_checker;
