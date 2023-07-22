#pragma once

#include "Hive.h"

inline struct OutProcess
{
	PROCESS_INFORMATION pi = {};

	BOOL init()
	{
		MY_TRACE(_T("OutProcess::init()\n"));

		TCHAR path[MAX_PATH] = {};
		::GetModuleFileName(hive.fp->dll_hinst, path, MAX_PATH);
		::PathRemoveExtension(path);
		::PathAppend(path, _T("LevelMeter.exe"));
		MY_TRACE_TSTR(path);

		TCHAR args[MAX_PATH] = {};
		::StringCbPrintf(args, sizeof(args), _T("0x%08p"), hive.fp->hwnd);
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
			&pi))         // Pointer to PROCESS_INFORMATION structur
		{
			MY_TRACE(_T("::CreateProcess() failed.\n"));

			return FALSE;
		}

		::WaitForInputIdle(pi.hProcess, INFINITE);

		return TRUE;
	}

	BOOL exit()
	{
		MY_TRACE(_T("OutProcess::exit()\n"));

		return TRUE;
	}
} outProcess;
