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
// ���̃N���X�̓��C���v���Z�X���Ď����܂��B
// ���C���v���Z�X�������ɂȂ����ꍇ�̓J�����g�v���Z�X���I�����܂��B
// ����ɂ��A���C���v���Z�X�������I�������Ƃ��Ă��A
// �T�u�v���Z�X(�J�����g�v���Z�X)�̎c�[���c��Ȃ��悤�ɂȂ�܂��B
//
inline struct MainProcessChecker
{
	BOOL init();
	BOOL exit();
	static void CALLBACK timerProc(HWND hwnd, UINT message, UINT_PTR timerId, DWORD time);
} main_process_checker;

//
// ���̃N���X�̓R���t�B�O�t�@�C���̍X�V���Ď����܂��B
// ����ɂ��A���[�U�[���R���t�B�O�t�@�C���𒼐ڕҏW�����ꍇ�ł��A
// �ύX�������ɔ��f�����悤�ɂȂ�܂��B
//
inline struct ConfigFileChecker : FileUpdateChecker
{
	BOOL init();
	BOOL exit();
	static void CALLBACK timerProc(HWND hwnd, UINT message, UINT_PTR timerId, DWORD time);
} config_file_checker;
