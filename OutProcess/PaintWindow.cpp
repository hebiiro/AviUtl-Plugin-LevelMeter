#include "pch.h"
#include "PaintWindow.h"
#include "View.h"

PaintWindow::PaintWindow(int painterType)
	: painterType(painterType)
{
}

PaintWindow::~PaintWindow()
{
}

BOOL PaintWindow::create(HINSTANCE instance, HWND parent, LPCTSTR name)
{
	const LPCTSTR className = _T("LevelMeter.PaintWindow");

	WNDCLASSEXW wc = { sizeof(wc) };
	wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc    = ::DefWindowProc;
	wc.hInstance      = instance;
	wc.hCursor        = ::LoadCursor(0, IDC_ARROW);
	wc.lpszClassName  = className;
	::RegisterClassExW(&wc);

	return __super::create(
		WS_EX_NOPARENTNOTIFY,
		className,
		name,
		WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, 0, 0,
		parent, 0, instance, 0);
}

LRESULT PaintWindow::onCreate(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("PaintWindow::onCreate()\n"));

	if (!initOpenGL()) return -1;

	return __super::onWndProc(hwnd, message, wParam, lParam);
}

LRESULT PaintWindow::onDestroy(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("PaintWindow::onDestroy()\n"));

	termOpenGL();

	return __super::onWndProc(hwnd, message, wParam, lParam);
}

LRESULT PaintWindow::onAviUtlFilterRedraw(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("onAviUtlFilterRedraw(0x%08X, 0x%08X)\n"), wParam, lParam);

	if (hive.redraw())
		view.doPaint();

	return 0;
}

LRESULT PaintWindow::onAviUtlFilterConfig(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("onAviUtlFilterConfig(0x%08X, 0x%08X)\n"), wParam, lParam);

	if (hive.config(hwnd))
		view.updateDesign();

	return 0;
}

LRESULT PaintWindow::onWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE: return onCreate(hwnd, message, wParam, lParam);
	case WM_DESTROY: return onDestroy(hwnd, message, wParam, lParam);
	case WM_PAINT: return onPaint(hwnd, message, wParam, lParam);
	case WM_LBUTTONDOWN: return onLButtonDown(hwnd, message, wParam, lParam);
	case WM_MOUSEMOVE: return onMouseMove(hwnd, message, wParam, lParam);
	case WM_LBUTTONUP: return onLButtonUp(hwnd, message, wParam, lParam);
	}

	if (message == WM_AVIUTL_FILTER_REDRAW) return onAviUtlFilterRedraw(hwnd, message, wParam, lParam);
	else if (message == WM_AVIUTL_FILTER_CONFIG) return onAviUtlFilterConfig(hwnd, message, wParam, lParam);

	return __super::onWndProc(hwnd, message, wParam, lParam);
}
