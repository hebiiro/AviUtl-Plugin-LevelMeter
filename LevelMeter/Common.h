#pragma once

// OutProcess => InProcess
static const UINT WM_AVIUTL_FILTER_INIT = ::RegisterWindowMessage(_T("WM_AVIUTL_FILTER_INIT"));

// InProcess => OutProcess
static const UINT WM_AVIUTL_FILTER_EXIT = ::RegisterWindowMessage(_T("WM_AVIUTL_FILTER_EXIT"));
static const UINT WM_AVIUTL_FILTER_RESIZE = ::RegisterWindowMessage(_T("WM_AVIUTL_FILTER_RESIZE"));
static const UINT WM_AVIUTL_FILTER_CONFIG = ::RegisterWindowMessage(_T("WM_AVIUTL_FILTER_CONFIG"));
static const UINT WM_AVIUTL_FILTER_LEVEL_METER = ::RegisterWindowMessage(_T("WM_AVIUTL_FILTER_LEVEL_METER"));

struct LevelMeter
{
	DWORD time;
	int frame;
	double level[2];
	double peak[2];
};
