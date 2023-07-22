#pragma once
#include "Hive.h"

//
// このクラスはレベルメーターを描画するウィンドウです。
//
struct PaintWindow : Tools::Window
{
	int painterType = 0;
	HGLRC glrc = 0;
	NVGcontext* vg = 0;
	int fontDefault = 0;
	int fontDefault2 = 0;
	int image = 0;

	PaintWindow(int painterType);
	virtual ~PaintWindow();

	BOOL create(HINSTANCE instance, HWND parent, LPCTSTR name);

	static BOOL setupPixelFormat(HDC dc);
	BOOL initOpenGL();
	BOOL termOpenGL();
	void updateDesign();
	void updateDesignInternal();

	void doPaint();
	void doPaint(HDC dc, const RECT& rc);
	void doPaint(const RECT& rc);

	int hitTest(const POINT& point) const;

	LRESULT onCreate(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onDestroy(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onPaint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onLButtonDown(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onMouseMove(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onLButtonUp(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onAviUtlFilterRedraw(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onAviUtlFilterConfig(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT onWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};
