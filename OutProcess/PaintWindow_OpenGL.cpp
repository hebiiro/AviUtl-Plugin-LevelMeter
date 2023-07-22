#include "pch.h"
#include "PaintWindow.h"

#define NANOVG_GL3_IMPLEMENTATION
#include "NanoVG/nanovg_gl.h"

using namespace Tools;

BOOL PaintWindow::setupPixelFormat(HDC dc)
{
	MY_TRACE(_T("PaintWindow::setupPixelFormat(0x%08X)\n"), dc);

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(pfd),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		1,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pixelFormat = ::ChoosePixelFormat(dc, &pfd);
	MY_TRACE_INT(pixelFormat);

	if (!pixelFormat)
	{
		MY_TRACE(_T("::ChoosePixelFormat() が失敗しました\n"));

		return FALSE;
	}

	if (!::SetPixelFormat(dc, pixelFormat, &pfd))
	{
		MY_TRACE(_T("::SetPixelFormat() が失敗しました\n"));

		return FALSE;
	}

	return TRUE;
}

BOOL PaintWindow::initOpenGL()
{
	MY_TRACE(_T("PaintWindow::initOpenGL()\n"));

	// OpenGL を初期化する。

	ClientDC dc(*this);

	setupPixelFormat(dc);

	glrc = wglCreateContext(dc);
	MY_TRACE_HEX(glrc);

	if (!glrc)
	{
		MY_TRACE(_T("wglCreateContext() が失敗しました\n"));

		return FALSE;
	}

	MakeCurrent makeCurrent(dc, glrc);

	// GLEW を初期化する。

	LPCSTR text = 0;
	GLenum err = glewInit();
	MY_TRACE_HEX(err);
	if (err == GLEW_OK)
	{
		text = (LPCSTR)glewGetString(GLEW_VERSION);
	}
	else
	{
		text = (LPCSTR)glewGetErrorString(err);
	}
	MY_TRACE_STR(text);

	// NanoVG を初期化する。

	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	MY_TRACE_HEX(vg);

	updateDesign();

	return TRUE;
}

BOOL PaintWindow::termOpenGL()
{
	MY_TRACE(_T("PaintWindow::termOpenGL()\n"));

	nvgDeleteGL3(vg), vg = 0;

	wglDeleteContext(glrc), glrc = 0;

	return TRUE;
}

void PaintWindow::updateDesign()
{
	MY_TRACE(_T("PaintWindow::updateDesign()\n"));

	ClientDC dc(*this);
	MakeCurrent makeCurrent(dc, glrc);

	updateDesignInternal();

	::InvalidateRect(*this, 0, FALSE);
}

void PaintWindow::updateDesignInternal()
{
	MY_TRACE(_T("PaintWindow::updateDesignInternal()\n"));

	if (wcslen(design.fontDefault) && wcslen(design.fontDefault2))
	{
		// NanoVG を使用してフォントを読み込みます。

		if (wcslen(design.fontDefault) != 0)
		{
			MY_TRACE_STR((LPCSTR)(design.fontDefault));
			fontDefault = nvgCreateFont(vg, "default", (LPCSTR)(design.fontDefault));
			MY_TRACE_INT(fontDefault);

			if (wcslen(design.fontDefault2) != 0)
			{
				MY_TRACE_STR((LPCSTR)(design.fontDefault2));
				fontDefault2 = nvgCreateFont(vg, "default2", (LPCSTR)(design.fontDefault2));
				MY_TRACE_INT(fontDefault2);

				nvgAddFallbackFontId(vg, fontDefault, fontDefault2);
			}
		}
	}

	{
		// NanoVG を使用して画像を読み込みます。

		if (image)
			nvgDeleteImage(vg, image);

		const Design::Image* image = &design.image;

		switch (painterType)
		{
		case PainterType::Left: image = &design.left.image; break;
		case PainterType::Right: image = &design.right.image; break;
		}

		MY_TRACE_WSTR((BSTR)image->fileName);
		MY_TRACE_STR((LPCSTR)(image->fileName));

		this->image = nvgCreateImage(vg, (LPCSTR)(image->fileName), 0);
	}
}
