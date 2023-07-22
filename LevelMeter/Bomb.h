#pragma once

//
// ���̃N���X�͎󂯎���������M�����特�ʂ��Z�o���܂��B
//
struct Bomb
{
	//
	// ���ʂ̎Z�o�Ɏg�p���鉹���M���ł��B
	//
	std::unique_ptr<short[]> audiop;

	//
	// �����M���̐��ł��B
	//
	int32_t audio_n;

	//
	// �����M���̃`�����l�����ł��B
	//
	int32_t audio_ch;

	//
	// �Z�o���ꂽ���ʂł��B
	//
	Volume volume;

	//
	// �R���X�g���N�^�ł��B
	// fp �� fpip ����K�v�ȃf�[�^���擾���āA���̃I�u�W�F�N�g�ŕێ����܂��B
	// ���C���X���b�h������Ă΂�܂��B
	//
	Bomb(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip, DWORD time)
		: audiop(new short[fpip->audio_n * fpip->audio_ch])
		, audio_n(fpip->audio_n)
		, audio_ch(fpip->audio_ch)
		, volume({ time, fpip->frame })
	{
		memcpy(audiop.get(), fpip->audiop, sizeof(short) * fpip->audio_n * fpip->audio_ch);
	}

	//
	// ���ʃ��x���ƃs�[�N���Z�o���܂��B
	// �T�u�X���b�h������Ă΂�܂��B
	//
	void calc()
	{
		for (int ch = 0; ch < audio_ch; ch++)
			calc(ch);
	}

	//
	// �w�肳�ꂽ�`�����l���̉��ʃ��x���ƃs�[�N���Z�o���܂��B
	//
	void calc(int ch)
	{
		float level = 0.0f;
		float peak = 0.0f;

		for (int i = 0; i < audio_n * audio_ch; i += audio_ch)
		{
			float n = normalize(audiop[i + ch]);

			level += n * n;
			peak = std::max(peak, std::fabs(n));
		}

		level = std::sqrt(level / audio_n);

		volume.level[ch] = 20 * std::log10(level);
		volume.peak[ch] = 20 * std::log10(peak);
	}

	//
	// �󂯎���������M���� -1.0 �` 1.0 �ɐ��K�����ĕԂ��܂��B
	//
	inline static float normalize(short pcm)
	{
		return pcm / 32768.0f;
	}
};
