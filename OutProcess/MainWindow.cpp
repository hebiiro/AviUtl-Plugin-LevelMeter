#include "pch.h"
#include "MainWindow.h"

#define NANOVG_GL2_IMPLEMENTATION
#include "NanoVG/nanovg_gl.h"

//--------------------------------------------------------------------

MainWindow g_mainWindow;

//--------------------------------------------------------------------

BOOL MainWindow::setupPixelFormat(HDC dc)
{
	MY_TRACE(_T("MainWindow::setupPixelFormat(0x%08X)\n"), dc);

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

BOOL MainWindow::initOpenGL()
{
	MY_TRACE(_T("MainWindow::initOpenGL()\n"));

	// OpenGL を初期化する。

	ClientDC dc(m_hwnd);

	setupPixelFormat(dc);

	m_rc = wglCreateContext(dc);
	MY_TRACE_HEX(m_rc);

	if (!m_rc)
	{
		MY_TRACE(_T("wglCreateContext() が失敗しました\n"));

		return -1;
	}

	MakeCurrent makeCurrent(dc, m_rc);

	// GLEW を初期化する。

	LPCSTR text = 0;
	GLenum err = glewInit();
	if (err == GLEW_OK)
	{
		text = (LPCSTR)glewGetString(GLEW_VERSION);
	}
	else
	{
		text = (LPCSTR)glewGetErrorString(err);
	}

	// NanoVG を初期化する。

	m_vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

	updateDesign();

	return TRUE;
}

BOOL MainWindow::termOpenGL()
{
	MY_TRACE(_T("MainWindow::termOpenGL()\n"));

	nvgDeleteGL2(m_vg), m_vg = 0;

	wglDeleteContext(m_rc), m_rc = 0;

	return TRUE;
}

//--------------------------------------------------------------------

LRESULT MainWindow::onCreate(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("MainWindow::onCreate()\n"));

	m_hwnd = hwnd;

	load();

	if (!initOpenGL()) return -1;

	{
		HWND hwnd = g_parent;

		m_mutex.reset(new Mutex(0, FALSE, FormatText(_T("LevelMeter.Mutex.%08X"), hwnd)));
		m_fileMapping.reset(new SimpleFileMapping(sizeof(LevelMeter), FormatText(_T("LevelMeter.FileMapping.%08X"), hwnd)));
	}

	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT MainWindow::onDestroy(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("MainWindow::onDestroy()\n"));

	m_mutex.reset();
	m_fileMapping.reset();

	termOpenGL();

	::PostQuitMessage(0);

	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT MainWindow::onLButtonDown(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("MainWindow::onLButtonDown(WM_LBUTTONDOWN, 0x%08X, 0x%08X)\n"), wParam, lParam);

	::SetCapture(hwnd);
	m_dragOriginPoint = LP2PT(lParam);

	RECT rc; ::GetClientRect(hwnd, &rc);
	int cy = (rc.top + rc.bottom) / 2;

	if (m_dragOriginPoint.y < cy)
	{
		m_dragMode = DragMode::maxRange;
		m_dragOriginRange = m_maxRange;
	}
	else
	{
		m_dragMode = DragMode::minRange;
		m_dragOriginRange = m_minRange;
	}

	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT MainWindow::onMouseMove(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("MainWindow::onMouseMove(WM_LBUTTONDOWN, 0x%08X, 0x%08X)\n"), wParam, lParam);

	if (::GetCapture() == hwnd)
	{
		POINT point = LP2PT(lParam);
		int offset = point.y - m_dragOriginPoint.y;

		switch (m_dragMode)
		{
		case DragMode::maxRange: m_maxRange = m_dragOriginRange + offset / 5; break;
		case DragMode::minRange: m_minRange = m_dragOriginRange + offset / 5; break;
		}

		::InvalidateRect(hwnd, 0, FALSE);
	}

	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT MainWindow::onLButtonUp(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("MainWindow::onLButtonUp(WM_LBUTTONDOWN, 0x%08X, 0x%08X)\n"), wParam, lParam);

	::ReleaseCapture();

	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT MainWindow::onAviUtlFilterResize(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("onAviUtlFilterResize(0x%08X, 0x%08X)\n"), wParam, lParam);

	RECT rc; ::GetClientRect(g_parent, &rc);
	::SetWindowPos(hwnd, 0, rc.left, rc.top, getWidth(rc), getHeight(rc), SWP_NOZORDER);

	return 0;
}

LRESULT MainWindow::onAviUtlFilterExit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("onAviUtlFilterExit(0x%08X, 0x%08X)\n"), wParam, lParam);

	::PostQuitMessage(0);

	return 0;
}

LRESULT MainWindow::onAviUtlFilterConfig(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("onAviUtlFilterConfig(0x%08X, 0x%08X)\n"), wParam, lParam);

	if (config(hwnd))
		::InvalidateRect(hwnd, 0, FALSE);

	return 0;
}

LRESULT MainWindow::onAviUtlFilterLevelMeter(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("onAviUtlFilterLevelMeter(0x%08X, 0x%08X)\n"), wParam, lParam);

	LevelMeter levelMeter = {};

	if (m_mutex && m_fileMapping)
	{
//		Synchronizer sync(*m_mutex);
		LevelMeter* shared = (LevelMeter*)m_fileMapping->getBuffer();

		memcpy(&levelMeter, shared, sizeof(*shared));
	}

	for (int i = 0; i < _countof(m_level); i++)
	{
		m_level[i] = levelMeter.level[i];
		m_peak[i] = levelMeter.peak[i];
	}

	if (levelMeter.time != 0)
	{
		DWORD time = ::timeGetTime();

		if (levelMeter.time > time)
		{
			DWORD sleepTime = levelMeter.time - time;
			MY_TRACE(_T("%d, %.3f, %.3f, %.3f秒間スリープします\n"), levelMeter.frame, time / 1000.0, levelMeter.time / 1000.0, sleepTime / 1000.0);
			::Sleep(sleepTime / 2);
		}
		else if (time - levelMeter.time > 5) // 許容範囲以上に遅れている場合は描画を行わない。
		{
			MY_TRACE(_T("%d, %.3f, %.3f, 遅れているので無視します\n"), levelMeter.frame, time / 1000.0, levelMeter.time / 1000.0);
			return 0;
		}
	}
	else
	{
		MY_TRACE(_T("%d\n"), levelMeter.frame);
	}

	doPaint();

	return 0;
}

LRESULT CALLBACK MainWindow::wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE: return g_mainWindow.onCreate(hwnd, message, wParam, lParam);
	case WM_DESTROY: return g_mainWindow.onDestroy(hwnd, message, wParam, lParam);
	case WM_PAINT: return g_mainWindow.onPaint(hwnd, message, wParam, lParam);
	case WM_LBUTTONDOWN: return g_mainWindow.onLButtonDown(hwnd, message, wParam, lParam);
	case WM_MOUSEMOVE: return g_mainWindow.onMouseMove(hwnd, message, wParam, lParam);
	case WM_LBUTTONUP: return g_mainWindow.onLButtonUp(hwnd, message, wParam, lParam);
	}

	if (message == WM_AVIUTL_FILTER_RESIZE) return g_mainWindow.onAviUtlFilterResize(hwnd, message, wParam, lParam);
	else if (message == WM_AVIUTL_FILTER_EXIT) return g_mainWindow.onAviUtlFilterExit(hwnd, message, wParam, lParam);
	else if (message == WM_AVIUTL_FILTER_CONFIG) return g_mainWindow.onAviUtlFilterConfig(hwnd, message, wParam, lParam);
	else if (message == WM_AVIUTL_FILTER_LEVEL_METER) return g_mainWindow.onAviUtlFilterLevelMeter(hwnd, message, wParam, lParam);

	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

//--------------------------------------------------------------------
