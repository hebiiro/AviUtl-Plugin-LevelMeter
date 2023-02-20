#include "pch.h"
#include "SenderThread.h"
#include "OutProcess.h"

//--------------------------------------------------------------------

SenderThread g_senderThread;

//--------------------------------------------------------------------

BOOL SenderThread::init(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("SenderThread::init()\n"));

	m_hwnd = fp->hwnd;
	m_handle = ::CreateThread(0, 0, threadProc, this, 0, &m_tid);
	return !!m_handle;
}

BOOL SenderThread::exit(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("SenderThread::exit()\n"));

	return ::PostThreadMessage(m_tid, WM_QUIT, 0, 0);
}

BOOL SenderThread::send(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip)
{
	MY_TRACE(_T("SenderThread::send()\n"));

	if (fpip->audio_ch != 1 && fpip->audio_ch != 2)
		return FALSE;

	if (fpip->audio_n == 0)
		return FALSE;

	DWORD time = 0;

	if (isPlaying(fp, fpip))
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

			int offset = fpip->frame - m_startFrame;
			offset *= 1000;
			if (fpip->editp->video_rate != 0) offset *= fpip->editp->video_rate;
			if (fpip->editp->video_scale != 0) offset /= fpip->editp->video_scale;

			time = m_startTime + offset;
		}
	}
	else
	{
		MY_TRACE(_T("再生中ではありません\n"));

		m_isPlaying = FALSE;
	}

	return ::PostThreadMessage(m_tid, WM_SEND, (WPARAM)new Sender(fp, fpip, time), 0);
}

BOOL SenderThread::isPlaying(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip)
{
	return fp->exfunc->get_frame(fpip->editp) != fpip->frame;
}

//--------------------------------------------------------------------

DWORD SenderThread::threadProc()
{
	SubThread thread(m_hwnd);

	MSG msg = {};
	while (::GetMessage(&msg, 0, 0, 0))
	{
		if (!msg.hwnd)
		{
			switch (msg.message)
			{
			case WM_SEND:
				{
					thread.onSend((Sender*)msg.wParam);
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

DWORD CALLBACK SenderThread::threadProc(LPVOID param)
{
	SenderThread* senderThread = (SenderThread*)param;
	return senderThread->threadProc();
}

//--------------------------------------------------------------------

SenderThread::SubThread::SubThread(HWND hwnd)
{
	m_mutex.reset(new Mutex(0, FALSE, FormatText(_T("LevelMeter.Mutex.%08X"), hwnd)));
	m_fileMapping.reset(new SimpleFileMapping(sizeof(LevelMeter), FormatText(_T("LevelMeter.FileMapping.%08X"), hwnd)));
}

SenderThread::SubThread::~SubThread()
{
}

void SenderThread::SubThread::onSend(Sender* sender)
{
	sender->calc();

	if (m_mutex && m_fileMapping)
	{
//		Synchronizer sync(*m_mutex);
		LevelMeter* shared = (LevelMeter*)m_fileMapping->getBuffer();

		memcpy(shared, &sender->levelMeter, sizeof(*shared));
	}

	::SendMessage(g_outProcess.m_mainWindow, WM_AVIUTL_FILTER_LEVEL_METER, 0, 0);

	delete sender;
}

//--------------------------------------------------------------------
