#pragma once
#include "Design.h"

//
// ���̃N���X�͑��N���X���狤�ʂ��Ďg�p�����ϐ���ێ����܂��B
//
inline struct Hive
{
	struct HitTest
	{
		inline static const int none = 0;
		inline static const int minRange = 1;
		inline static const int maxRange = 2;
	};

	HINSTANCE instance = 0;
	HWND director = 0;
	Mutex mutex;
	SimpleFileMapping fileMapping;

	double level[2] = {};
	double peak[2] = {};

	int minRange = -33;
	int maxRange = 14;

	struct Drag {
		int ht = HitTest::none;
		POINT origPoint = {};
		int origValue = 0;
	} drag;

	void getConfigFileName(LPWSTR fileName, DWORD size) const;
	void init(HWND director);
	void exit();
	void load();
	void save();
	BOOL redraw();
	BOOL config(HWND hwnd);
} hive;
