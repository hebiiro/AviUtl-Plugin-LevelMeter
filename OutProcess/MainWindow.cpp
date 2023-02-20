#include "pch.h"
#include "MainWindow.h"
#include "ConfigDialog.h"

#define NANOVG_GL2_IMPLEMENTATION
#include "NanoVG/nanovg_gl.h"

//--------------------------------------------------------------------

MainWindow g_mainWindow;

//--------------------------------------------------------------------

void MainWindow::load()
{
	MY_TRACE(_T("MainWindow::load()\n"));

	WCHAR fileName[MAX_PATH] = {};
	::GetModuleFileNameW(g_instance,  fileName, MAX_PATH);
	::PathRenameExtensionW(fileName, L".ini");
	MY_TRACE_WSTR(fileName);

	getPrivateProfileColor(fileName, L"Config", L"fillColor", g_design.fillColor);
	getPrivateProfileColor(fileName, L"Config", L"rmsColor0", g_design.rmsColor0);
	getPrivateProfileColor(fileName, L"Config", L"rmsColor1", g_design.rmsColor1);
	getPrivateProfileColor(fileName, L"Config", L"peakColor", g_design.peakColor);
	getPrivateProfileInt(fileName, L"Config", L"peakWidth", g_design.peakWidth);
	getPrivateProfileColor(fileName, L"Config", L"scaleColor", g_design.scaleColor);
	getPrivateProfileInt(fileName, L"Config", L"scaleWidth", g_design.scaleWidth);
	getPrivateProfileColor(fileName, L"Config", L"scaleTextColor", g_design.scaleTextColor);
	getPrivateProfileColor(fileName, L"Config", L"separatorColor", g_design.separatorColor);
	getPrivateProfileColor(fileName, L"Config", L"zebraColor", g_design.zebraColor);
	getPrivateProfileInt(fileName, L"Config", L"zebraWidth", g_design.zebraWidth);
	getPrivateProfileInt(fileName, L"Config", L"zebraStep", g_design.zebraStep);
	getPrivateProfileInt(fileName, L"Config", L"minRange", m_minRange);
	getPrivateProfileInt(fileName, L"Config", L"maxRange", m_maxRange);
}

void MainWindow::save()
{
	MY_TRACE(_T("MainWindow::save()\n"));

	WCHAR fileName[MAX_PATH] = {};
	::GetModuleFileNameW(g_instance,  fileName, MAX_PATH);
	::PathRenameExtensionW(fileName, L".ini");
	MY_TRACE_WSTR(fileName);

	setPrivateProfileColor(fileName, L"Config", L"fillColor", g_design.fillColor);
	setPrivateProfileColor(fileName, L"Config", L"rmsColor0", g_design.rmsColor0);
	setPrivateProfileColor(fileName, L"Config", L"rmsColor1", g_design.rmsColor1);
	setPrivateProfileColor(fileName, L"Config", L"peakColor", g_design.peakColor);
	setPrivateProfileInt(fileName, L"Config", L"peakWidth", g_design.peakWidth);
	setPrivateProfileColor(fileName, L"Config", L"scaleColor", g_design.scaleColor);
	setPrivateProfileInt(fileName, L"Config", L"scaleWidth", g_design.scaleWidth);
	setPrivateProfileColor(fileName, L"Config", L"scaleTextColor", g_design.scaleTextColor);
	setPrivateProfileColor(fileName, L"Config", L"separatorColor", g_design.separatorColor);
	setPrivateProfileColor(fileName, L"Config", L"zebraColor", g_design.zebraColor);
	setPrivateProfileInt(fileName, L"Config", L"zebraWidth", g_design.zebraWidth);
	setPrivateProfileInt(fileName, L"Config", L"zebraStep", g_design.zebraStep);
	setPrivateProfileInt(fileName, L"Config", L"minRange", m_minRange);
	setPrivateProfileInt(fileName, L"Config", L"maxRange", m_maxRange);
}

BOOL MainWindow::config(HWND hwnd)
{
	MY_TRACE(_T("MainWindow::config()\n"));

	ConfigDialog dialog(hwnd);

	::SetDlgItemText(dialog, IDC_FONT_DEFAULT, g_design.fontDefault);
	::SetDlgItemText(dialog, IDC_FONT_DEFAULT_2, g_design.fontDefault2);
	::SetDlgItemInt(dialog, IDC_FILL_COLOR, g_design.fillColor, FALSE);
	::SetDlgItemInt(dialog, IDC_RMS_COLOR_0, g_design.rmsColor0, FALSE);
	::SetDlgItemInt(dialog, IDC_RMS_COLOR_1, g_design.rmsColor1, FALSE);
	::SetDlgItemInt(dialog, IDC_PEAK_COLOR, g_design.peakColor, FALSE);
	::SetDlgItemInt(dialog, IDC_PEAK_WIDTH, g_design.peakWidth, FALSE);
	::SetDlgItemInt(dialog, IDC_SCALE_COLOR, g_design.scaleColor, FALSE);
	::SetDlgItemInt(dialog, IDC_SCALE_WIDTH, g_design.scaleWidth, FALSE);
	::SetDlgItemInt(dialog, IDC_SCALE_TEXT_COLOR, g_design.scaleTextColor, FALSE);
	::SetDlgItemInt(dialog, IDC_SEPARATOR_COLOR, g_design.separatorColor, FALSE);
	::SetDlgItemInt(dialog, IDC_ZEBRA_COLOR, g_design.zebraColor, FALSE);
	::SetDlgItemInt(dialog, IDC_ZEBRA_WIDTH, g_design.zebraWidth, FALSE);
	::SetDlgItemInt(dialog, IDC_ZEBRA_STEP, g_design.zebraStep, FALSE);
	::SetDlgItemInt(dialog, IDC_MIN_RANGE, m_minRange, TRUE);
	::SetDlgItemInt(dialog, IDC_MAX_RANGE, m_maxRange, TRUE);

	int retValue = dialog.doModal();

	if (IDOK != retValue)
		return FALSE;

	::GetDlgItemText(dialog, IDC_FONT_DEFAULT, g_design.fontDefault, MAX_PATH);
	::GetDlgItemText(dialog, IDC_FONT_DEFAULT_2, g_design.fontDefault2, MAX_PATH);
	g_design.fillColor = ::GetDlgItemInt(dialog, IDC_FILL_COLOR, 0, FALSE);
	g_design.rmsColor0 = ::GetDlgItemInt(dialog, IDC_RMS_COLOR_0, 0, FALSE);
	g_design.rmsColor1 = ::GetDlgItemInt(dialog, IDC_RMS_COLOR_1, 0, FALSE);
	g_design.peakColor = ::GetDlgItemInt(dialog, IDC_PEAK_COLOR, 0, FALSE);
	g_design.peakWidth = ::GetDlgItemInt(dialog, IDC_PEAK_WIDTH, 0, FALSE);
	g_design.scaleColor = ::GetDlgItemInt(dialog, IDC_SCALE_COLOR, 0, FALSE);
	g_design.scaleWidth = ::GetDlgItemInt(dialog, IDC_SCALE_WIDTH, 0, FALSE);
	g_design.scaleTextColor = ::GetDlgItemInt(dialog, IDC_SCALE_TEXT_COLOR, 0, FALSE);
	g_design.separatorColor = ::GetDlgItemInt(dialog, IDC_SEPARATOR_COLOR, 0, FALSE);
	g_design.zebraColor = ::GetDlgItemInt(dialog, IDC_ZEBRA_COLOR, 0, FALSE);
	g_design.zebraWidth = ::GetDlgItemInt(dialog, IDC_ZEBRA_WIDTH, 0, FALSE);
	g_design.zebraStep = ::GetDlgItemInt(dialog, IDC_ZEBRA_STEP, 0, FALSE);
	m_minRange = ::GetDlgItemInt(dialog, IDC_MIN_RANGE, 0, TRUE);
	m_maxRange = ::GetDlgItemInt(dialog, IDC_MAX_RANGE, 0, TRUE);

	ClientDC dc(m_hwnd);
	MakeCurrent makeCurrent(dc, m_rc);

	m_fontDefault = nvgCreateFont(m_vg, u8"default", StringU8(g_design.fontDefault));
	m_fontDefault2 = nvgCreateFont(m_vg, u8"default2", StringU8(g_design.fontDefault2));
	nvgAddFallbackFontId(m_vg, m_fontDefault, m_fontDefault2);

	return TRUE;
}

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

	m_fontDefault = nvgCreateFont(m_vg, u8"default", StringU8(g_design.fontDefault));
	m_fontDefault2 = nvgCreateFont(m_vg, u8"default2", StringU8(g_design.fontDefault2));
	nvgAddFallbackFontId(m_vg, m_fontDefault, m_fontDefault2);

	return TRUE;
}

BOOL MainWindow::termOpenGL()
{
	MY_TRACE(_T("MainWindow::termOpenGL()\n"));

	nvgDeleteGL2(m_vg), m_vg = 0;

	wglDeleteContext(m_rc), m_rc = 0;

	return TRUE;
}

void MainWindow::doPaint()
{
	ClientDC dc(m_hwnd);
	RECT rc; ::GetClientRect(m_hwnd, &rc);

	doPaint(dc, rc);
}

void MainWindow::doPaint(HDC dc, const RECT& rc)
{
	MakeCurrent makeCurrent(dc, m_rc);
	int w = getWidth(rc);
	int h = getHeight(rc);

	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(m_vg, (float)w, (float)h, 1.0f);
	nvgSave(m_vg);

	doPaint(rc);

	nvgRestore(m_vg);
	nvgEndFrame(m_vg);

	glFlush();
	::SwapBuffers(dc);
}

void MainWindow::doPaint(const RECT& rc)
{
	int w = getWidth(rc);
	int h = getHeight(rc);

	if (w <= 0 || h <= 0) return;

	{
		// 背景を描画する。

		nvgBeginPath(m_vg);
		nvgRect(m_vg, 0.0f, 0.0f, (float)w, (float)h);
		nvgFillColor(m_vg, (MyColor)g_design.fillColor);
		nvgFill(m_vg);
	}

	{
		// RMS とピークを描画する。

		RECT rects[2] = { rc, rc };
		rects[0].left = rc.left + ::MulDiv(w, 1, 4);
		rects[0].right = rc.left + ::MulDiv(w, 2, 4);
		rects[1].left = rc.left + ::MulDiv(w, 2, 4);
		rects[1].right = rc.left + ::MulDiv(w, 3, 4);

		for (int i = 0; i < 2; i++)
		{
			int top = rects[i].top;
			int bottom = rects[i].bottom;
			int h = rects[i].bottom - rects[i].top;

			if (m_level[i] > m_minRange)
			{
				double level = (m_level[i] - m_minRange) / (m_maxRange - m_minRange);

				rects[i].top = bottom - (int)(h * level);

				float x = (float)(rects[i].left);
				float y = (float)(rects[i].top);
				float w = (float)(getWidth(rects[i]));
				float h = (float)(getHeight(rects[i]));

				NVGpaint paint = nvgLinearGradient(m_vg,
					x, y, x, y + h,
					(MyColor)g_design.rmsColor0,
					(MyColor)g_design.rmsColor1);
				nvgBeginPath(m_vg);
				nvgRect(m_vg, x, y, w, h);
				nvgFillPaint(m_vg, paint);
				nvgFill(m_vg);
			}

			if (m_peak[i] > m_minRange)
			{
				double peak = (m_peak[i] - m_minRange) / (m_maxRange - m_minRange);

				int peakY = bottom - (int)(h * peak);

				{
					float mx = (float)(rects[i].left);
					float my = (float)(peakY);
					float lx = (float)(rects[i].right);
					float ly = (float)(peakY);

					nvgBeginPath(m_vg);
					nvgMoveTo(m_vg, mx, my);
					nvgLineTo(m_vg, lx, ly);
					nvgStrokeWidth(m_vg, (float)g_design.peakWidth);
					nvgStrokeColor(m_vg, (MyColor)g_design.peakColor);
					nvgStroke(m_vg);
				}

				if (g_design.zebraStep > 0)
				{
					for (int y = 0; y < (bottom - peakY) - g_design.zebraStep / 2; y += g_design.zebraStep)
					{
						float mx = (float)(rects[i].left);
						float my = (float)(bottom - y);
						float lx = (float)(rects[i].right);
						float ly = (float)(bottom - y);

						nvgBeginPath(m_vg);
						nvgMoveTo(m_vg, mx, my);
						nvgLineTo(m_vg, lx, ly);
						nvgStrokeWidth(m_vg, (float)g_design.zebraWidth);
						nvgStrokeColor(m_vg, (MyColor)g_design.zebraColor);
						nvgStroke(m_vg);
					}
				}
			}
		}
	}

	{
		// ゲージを描画する。

		nvgStrokeWidth(m_vg, (float)g_design.scaleWidth);
		nvgStrokeColor(m_vg, (MyColor)g_design.scaleColor);
		nvgFontBlur(m_vg, 0.0f);
		nvgFontSize(m_vg, (float)g_design.scaleTextHeight);
		nvgFontFaceId(m_vg, m_fontDefault);
		nvgTextAlign(m_vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
		nvgFillColor(m_vg, (MyColor)g_design.scaleTextColor);

		int textPadding = 1;
		int textHeight = g_design.scaleTextHeight + textPadding * 2;

		int range = m_maxRange - m_minRange;
		int freq = range * textHeight / h + 1;

		for (int i = m_maxRange; i > m_minRange; i--)
		{
			if (i % freq) continue;

			double di = (double)i - m_minRange;

			RECT rcText = rc;
			rcText.top = (int)(rc.top + h * (1.0 - di / range));
			rcText.bottom = rcText.top + textHeight;

			{
				float mx = (float)(rc.left);
				float my = (float)(rcText.bottom);
				float lx = (float)(rc.left + ::MulDiv(w, 1, 4));
				float ly = (float)(rcText.bottom);

				nvgBeginPath(m_vg);
				nvgMoveTo(m_vg, mx, my);
				nvgLineTo(m_vg, lx, ly);
				nvgStroke(m_vg);
			}

			{
				float mx = (float)(rc.right);
				float my = (float)(rcText.bottom);
				float lx = (float)(rc.left + ::MulDiv(w, 3, 4));
				float ly = (float)(rcText.bottom);

				nvgBeginPath(m_vg);
				nvgMoveTo(m_vg, mx, my);
				nvgLineTo(m_vg, lx, ly);
				nvgStroke(m_vg);
			}

			char text[MAX_PATH] = {};
			::StringCbPrintfA(text, sizeof(text), "%+d", i);

			nvgTextAlign(m_vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
			nvgText(m_vg, (float)rcText.left, (float)(rcText.top + textPadding), text, 0);

			nvgTextAlign(m_vg, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP);
			nvgText(m_vg, (float)rcText.right, (float)(rcText.top + textPadding), text, 0);
		}
	}

	{
		// セパレータを描画する。

		for (int i = 2; i <= 2; i++)
		{
			float mx = (float)(rc.left + ::MulDiv(w, i, 4));
			float my = (float)(rc.top);
			float lx = (float)(rc.left + ::MulDiv(w, i, 4));
			float ly = (float)(rc.bottom);

			nvgBeginPath(m_vg);
			nvgMoveTo(m_vg, mx, my);
			nvgLineTo(m_vg, lx, ly);
			nvgStrokeWidth(m_vg, 1.0f);
			nvgStrokeColor(m_vg, (MyColor)g_design.separatorColor);
			nvgStroke(m_vg);
		}
	}
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

LRESULT MainWindow::onPaint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PaintDC dc(hwnd);
	RECT rc = dc.m_ps.rcPaint;
	doPaint(dc, rc);

	return 0;
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
//	::InvalidateRect(hwnd, 0, FALSE);
//	::UpdateWindow(hwnd);

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
