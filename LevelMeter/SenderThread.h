#pragma once

#include "Common.h"
#include "Sender.h"

//--------------------------------------------------------------------

struct SenderThread
{
	static const UINT WM_SEND = WM_APP + 1;

	HWND m_hwnd = 0;
	DWORD m_tid = 0;
	Handle m_handle;

	BOOL m_isPlaying = FALSE;
	DWORD m_startTime = 0;
	int m_startFrame = 0;

	// メインスレッド側の処理。
	BOOL init(AviUtl::FilterPlugin* fp);
	BOOL exit(AviUtl::FilterPlugin* fp);
	BOOL send(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip);
	static BOOL isPlaying(AviUtl::FilterPlugin* fp, AviUtl::FilterProcInfo* fpip);

	// サブスレッド側の処理。
	DWORD threadProc();
	static DWORD CALLBACK threadProc(LPVOID param);

	struct SubThread
	{
		std::unique_ptr<Mutex> m_mutex;
		std::unique_ptr<SimpleFileMapping> m_fileMapping;

		SubThread(HWND hwnd);
		~SubThread();
		void onSend(Sender* sender);
	};
};

extern SenderThread g_senderThread;

//--------------------------------------------------------------------
