#pragma once

//--------------------------------------------------------------------

struct StringU8
{
	char m_buffer[MAX_PATH] = {};

	StringU8()
	{
	}

	StringU8(LPCWSTR x)
	{
		operator=(x);
	}

	StringU8& operator=(LPCWSTR x)
	{
		::WideCharToMultiByte(CP_UTF8, 0, x, -1, m_buffer, MAX_PATH, 0, 0);
		return *this;
	}

	operator LPCSTR() const
	{
		return m_buffer;
	}
};

//--------------------------------------------------------------------

struct Design
{
	WCHAR fontDefault[MAX_PATH];
	WCHAR fontDefault2[MAX_PATH];
	COLORREF fillColor;
	COLORREF rmsColor0;
	COLORREF rmsColor1;
	COLORREF peakColor;
	int peakWidth;
	COLORREF scaleColor;
	int scaleWidth;
	int scaleTextHeight;
	COLORREF scaleTextColor;
	COLORREF separatorColor;
	int zebraStep;
	int zebraWidth;
	COLORREF zebraColor;

	Design();
	~Design();
};

extern Design g_design;

//--------------------------------------------------------------------
