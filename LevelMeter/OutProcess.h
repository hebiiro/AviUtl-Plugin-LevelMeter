#pragma once

#include "Common.h"

//--------------------------------------------------------------------

struct OutProcess
{
	PROCESS_INFORMATION m_pi = {};
	HWND m_mainWindow = 0;

	BOOL init(AviUtl::FilterPlugin* fp);
	BOOL exit(AviUtl::FilterPlugin* fp);
	BOOL resize(AviUtl::FilterPlugin* fp);
	BOOL config(AviUtl::FilterPlugin* fp);
};

extern OutProcess g_outProcess;

//--------------------------------------------------------------------
