#pragma once
#include "Bomb.h"

inline struct Bomber
{
	static const UINT WM_SEND = WM_APP + 1;

	HWND director = 0;
	DWORD m_tid = 0;
	Handle m_thread;

	BOOL m_isPlaying = FALSE;
	DWORD m_startTime = 0;
	int m_startFrame = 0;

	// メインスレッド側の処理。

	BOOL init(AviUtl::FilterPlugin* fp)
	{
		MY_TRACE(_T("Bomber::init()\n"));

		director = fp->hwnd;
		m_thread = ::CreateThread(0, 0, threadProc, this, 0, &m_tid);
		return !!m_thread;
	}

	BOOL exit(AviUtl::FilterPlugin* fp)
	{
		MY_TRACE(_T("Bomber::exit()\n"));

//		::PostThreadMessage(m_tid, WM_QUIT, 0, 0);
		::TerminateThread(m_thread, 0);
		return TRUE;
	}

	BOOL send(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip)
	{
		MY_TRACE(_T("Bomber::send()\n"));

		if (fpip->audio_ch != 1 && fpip->audio_ch != 2)
			return FALSE;

		if (fpip->audio_n == 0)
			return FALSE;

		DWORD time = 0;

		if (isSameFrame(fp, fpip))
		{
			MY_TRACE(_T("再生中ではありません\n"));

			m_isPlaying = FALSE;
		}
		else
		{
			if (!((DWORD)fpip->editp->aviutl_window_info.flag & 0x00040000))
			{
				// 再生中なのにフレームが噛み合わないときは
				// 他のプラグインから呼ばれている可能性があるので何もしない。
				return FALSE;
			}

			if (!m_isPlaying || m_startFrame >= fpip->frame)
			{
				MY_TRACE(_T("再生が開始されました\n"));

				m_isPlaying = TRUE;
				m_startTime = ::timeGetTime();
				m_startFrame = fpip->frame;

				time = m_startTime;
			}
			else
			{
				MY_TRACE(_T("再生中です\n"));

				MY_TRACE_INT(fpip->frame);
				MY_TRACE_INT(m_startFrame);

				int offset = frame2ms(fp, fpip, fpip->frame - m_startFrame);
				MY_TRACE_INT(offset);

				time = m_startTime + offset;
				MY_TRACE_INT(time);
			}
		}

		return ::PostThreadMessage(m_tid, WM_SEND, (WPARAM)new Bomb(fp, fpip, time), 0);
	}

	static BOOL isSameFrame(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip)
	{
		return fp->exfunc->get_frame(fpip->editp) == fpip->frame;
	}

	static int frame2ms(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip, int frame)
	{
		AviUtl::FileInfo fi = {};
		fp->exfunc->get_file_info(fpip->editp, &fi);

		if (fi.video_rate != 0 && fi.video_scale != 0)
			return (int)(frame * 1000.0f * fi.video_scale / fi.video_rate);
		else
			return 0;
	}

	// サブスレッド側の処理。

	DWORD onThreadProc()
	{
		SubThread thread(director);

		return thread.messageLoop();
	}

	static DWORD CALLBACK threadProc(LPVOID param)
	{
		Bomber* bomber = (Bomber*)param;

		return bomber->onThreadProc();
	}

	struct SubThread
	{
		HWND director;
		Mutex mutex;
		SimpleFileMapping fileMapping;

		SubThread(HWND director)
			: director(director)
			, mutex(0, FALSE, FormatText(_T("LevelMeter.Mutex.%08X"), director))
			, fileMapping(sizeof(Volume), FormatText(_T("LevelMeter.FileMapping.%08X"), director))
		{
			MY_TRACE(_T("SubThread::SubThread()\n"));
		}

		~SubThread()
		{
			MY_TRACE(_T("SubThread::~SubThread()\n"));
		}

		DWORD messageLoop()
		{
			MSG msg = {};
			while (::GetMessage(&msg, 0, 0, 0))
			{
				if (!msg.hwnd)
				{
					switch (msg.message)
					{
					case WM_SEND:
						{
							onSend((Bomb*)msg.wParam);

							break;
						}
					case WM_QUIT:
						{
							return 0;
						}
					}
				}

				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			return 0;
		}

		void onSend(Bomb* bomb)
		{
//			MY_TRACE(_T("SubThread::onSend()\n"));

			bomb->calc();

			{
//				Synchronizer sync(mutex);
				Volume* shared = (Volume*)fileMapping.getBuffer();

				*shared = bomb->volume;
			}

			delete bomb;

			auto window = static_cast<Tools::Window2*>(Tools::Window::getPointer(director));
			if (window)
			{
				auto windowHolderExtension = window->getExtension<Tools::AviUtl::WindowHolderExtension>();
				if (windowHolderExtension)
				{
					HWND target = windowHolderExtension->getTarget();
					if (target)
						::SendMessage(target, WM_AVIUTL_FILTER_REDRAW, 0, 0);
				}
			}
		}
	};
} bomber;
