#pragma once

//--------------------------------------------------------------------

struct XYWHRect
{
	int x, y, w, h;

	XYWHRect()
	{
		x = y = w = h = 0;
	}

	XYWHRect(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	XYWHRect(const RECT& rc)
	{
		operator=(rc);
	}

	XYWHRect& operator=(const RECT& rc)
	{
		this->x = rc.left;
		this->y = rc.top;
		this->w = getWidth(rc);
		this->h = getHeight(rc);

		return *this;
	}
};

struct MyColor : public NVGcolor
{
	MyColor()
	{
	}

	MyColor(DWORD color)
	{
		this->r = GetRValue(color) / 255.0f;
		this->g = GetGValue(color) / 255.0f;
		this->b = GetBValue(color) / 255.0f;
		this->a = GetAValue(color) / 255.0f;
	}

	operator DWORD()
	{
		BYTE r = (BYTE)roundf(this->r * 255.0f);
		BYTE g = (BYTE)roundf(this->g * 255.0f);
		BYTE b = (BYTE)roundf(this->b * 255.0f);
		BYTE a = (BYTE)roundf(this->a * 255.0f);

		return getDWORD(r, g, b, a);
	}

	MyColor& operator=(const NVGcolor& color)
	{
		((NVGcolor&)*this) = color;

		return *this;
	}

	static BYTE GetAValue(DWORD color)
	{
		return LOBYTE(color >> 24);
	}

	static COLORREF getCOLORREF(DWORD rgba)
	{
		return (rgba & 0x00FFFFFF);
	}

	static DWORD getDWORD(BYTE r, BYTE g, BYTE b, BYTE a)
	{
		return r | ((WORD)g << 8) | ((DWORD)b << 16) | ((DWORD)a << 24);
	}

	static DWORD getDWORD(COLORREF color, DWORD rgba)
	{
		return (color & 0x00FFFFFF) | (rgba & 0xFF000000);
	}
};

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
	_bstr_t fontDefault;
	_bstr_t fontDefault2;
	_bstr_t imageFileName;
	POINT imageOffset;
	float imageScale;
	float imageAngle;
	float imageAlpha;
	MyColor fillColor;
	MyColor rmsColor0;
	MyColor rmsColor1;
	MyColor peakColor;
	int peakWidth;
	MyColor scaleColor;
	int scaleWidth;
	int scaleTextHeight;
	MyColor scaleTextColor;
	MyColor separatorColor;
	int zebraStep;
	int zebraWidth;
	MyColor zebraColor;
	float shadowBlur;
	POINT shadowOffset;
	MyColor shadowColor;

	Design();
	~Design();
};

extern Design g_design;

//--------------------------------------------------------------------
