#include "pch.h"
#include "Sender.h"

//--------------------------------------------------------------------

// fp と fpip から必要なデータを取得して、このオブジェクトで保持する。
Sender::Sender(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip, DWORD time)
	: audiop(new short[fpip->audio_n * fpip->audio_ch])
	, audio_n(fpip->audio_n)
	, audio_ch(fpip->audio_ch)
	, levelMeter({ time, fpip->frame })
{
	memcpy(audiop.get(), fpip->audiop, sizeof(short) * fpip->audio_n * fpip->audio_ch);
}

//--------------------------------------------------------------------

inline double Sender::normalize(short pcm)
{
	return pcm / 32768.0; // -1.0 ～ 1.0に正規化
}

void Sender::calc(int ch)
{
	double level = 0.0;
	double peak = 0.0;

	for (int i = 0; i < audio_n * 2; i += 2)
	{
		double n = normalize(audiop[i + ch]);

		level += n * n;
		peak = std::max(peak, fabs(n));
	}

	level = sqrt(level / audio_n);

	levelMeter.level[ch] = 20 * log10(level);
	levelMeter.peak[ch] = 20 * log10(peak);
}

void Sender::calc()
{
	for (int ch = 0; ch < audio_ch; ch++)
		calc(ch);
}

//--------------------------------------------------------------------
