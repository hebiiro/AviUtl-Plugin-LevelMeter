#pragma once

#include "Common.h"

//--------------------------------------------------------------------

struct Sender
{
	// ���̓f�[�^�B
	std::unique_ptr<short[]> audiop;
	int32_t audio_ch;
	int32_t audio_n;

	// �o�̓f�[�^�B
	LevelMeter levelMeter;

	// ���C���X���b�h���̏����B
	Sender(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip, DWORD time);

	// �T�u�X���b�h���̏����B
	static double normalize(short pcm);
	void calc(int ch);
	void calc();
};

//--------------------------------------------------------------------
