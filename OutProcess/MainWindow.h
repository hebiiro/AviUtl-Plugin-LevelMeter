#pragma once

#include "OutProcess.h"
#include "Design.h"

//--------------------------------------------------------------------

struct MakeCurrent
{
	HDC m_prevDC = 0;
	HGLRC m_prevRC = 0;

	MakeCurrent(HDC dc, HGLRC rc)
	{
		m_prevDC = wglGetCurrentDC();
		m_prevRC = wglGetCurrentContext();

		wglMakeCurrent(dc, rc);
	}

	~MakeCurrent()
	{
		wglMakeCurrent(m_prevDC, m_prevRC);
	}
};

//--------------------------------------------------------------------

struct MainWindow
{
	struct DragMode
	{
		static const int minRange = 0;
		static const int maxRange = 1;
	};

	HWND m_hwnd = 0;
	HGLRC m_rc = 0;
	NVGcontext* m_vg = 0;
	int m_fontDefault = 0;
	int m_fontDefault2 = 0;
	int m_image = 0;

	double m_level[2] = {};
	double m_peak[2] = {};

	int m_minRange = -33;
	int m_maxRange = 14;

	int m_dragMode = DragMode::minRange;
	POINT m_dragOriginPoint = {};
	int m_dragOriginRange = 0;

	std::unique_ptr<Mutex> m_mutex;
	std::unique_ptr<SimpleFileMapping> m_fileMapping;

	void load();
	void save();
	BOOL config(HWND hwnd);
	void updateDesign();
	static BOOL setupPixelFormat(HDC dc);
	BOOL initOpenGL();
	BOOL termOpenGL();
	void doPaint();
	void doPaint(HDC dc, const RECT& rc);
	void doPaint(const RECT& rc);

	LRESULT onCreate(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onDestroy(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onPaint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onLButtonDown(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onMouseMove(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onLButtonUp(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onAviUtlFilterResize(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onAviUtlFilterExit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onAviUtlFilterConfig(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onAviUtlFilterLevelMeter(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

};

 extern MainWindow g_mainWindow;

//--------------------------------------------------------------------
