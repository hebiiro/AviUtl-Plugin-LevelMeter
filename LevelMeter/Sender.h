#pragma once

#include "Common.h"

//--------------------------------------------------------------------

struct Sender
{
	// 入力データ。
	std::unique_ptr<short[]> audiop;
	int32_t audio_ch;
	int32_t audio_n;

	// 出力データ。
	LevelMeter levelMeter;

	// メインスレッド側の処理。
	Sender(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip, DWORD time);

	// サブスレッド側の処理。
	static double normalize(short pcm);
	void calc(int ch);
	void calc();
};

//--------------------------------------------------------------------
