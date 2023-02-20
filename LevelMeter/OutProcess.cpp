#include "pch.h"
#include "OutProcess.h"

//--------------------------------------------------------------------

OutProcess g_outProcess;

//--------------------------------------------------------------------

BOOL OutProcess::init(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("OutProcess::init()\n"));

	TCHAR path[MAX_PATH] = {};
	::GetModuleFileName(fp->dll_hinst, path, MAX_PATH);
	::PathRemoveExtension(path);
	::PathAppend(path, _T("LevelMeter.exe"));
	MY_TRACE_TSTR(path);

	TCHAR args[MAX_PATH] = {};
	::StringCbPrintf(args, sizeof(args), _T("0x%08p"), fp->hwnd);
	MY_TRACE_TSTR(args);

	STARTUPINFO si = { sizeof(si) };
	if (!::CreateProcess(
		path,           // No module name (use command line)
		args,           // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&m_pi))         // Pointer to PROCESS_INFORMATION structur
	{
		MY_TRACE(_T("::CreateProcess() failed.\n"));

		return FALSE;
	}

	return TRUE;
}

BOOL OutProcess::exit(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("OutProcess::exit()\n"));

	return ::PostMessage(m_mainWindow, WM_AVIUTL_FILTER_EXIT, 0, 0);
}

BOOL OutProcess::resize(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("OutProcess::resize()\n"));

	return ::PostMessage(m_mainWindow, WM_AVIUTL_FILTER_RESIZE, 0, 0);
}

BOOL OutProcess::config(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("OutProcess::config()\n"));

	return ::PostMessage(m_mainWindow, WM_AVIUTL_FILTER_CONFIG, 0, 0);
}

//--------------------------------------------------------------------
