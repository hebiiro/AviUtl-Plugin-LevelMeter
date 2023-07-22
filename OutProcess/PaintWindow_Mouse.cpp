#include "pch.h"
#include "PaintWindow.h"

int PaintWindow::hitTest(const POINT& point) const
{
	RECT rc; ::GetClientRect(*this, &rc);
	int cy = (rc.top + rc.bottom) / 2;
	return (point.y > cy) ? Hive::HitTest::minRange : Hive::HitTest::maxRange;
}

LRESULT PaintWindow::onLButtonDown(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("PaintWindow::onLButtonDown(0x%08X, 0x%08X)\n"), wParam, lParam);

	::SetCapture(hwnd);

	hive.drag.origPoint = LP2PT(lParam);
	hive.drag.ht = hitTest(hive.drag.origPoint);

	switch (hive.drag.ht)
	{
	case Hive::HitTest::minRange: hive.drag.origValue = hive.minRange; break;
	case Hive::HitTest::maxRange: hive.drag.origValue = hive.maxRange; break;
	}

	return __super::onWndProc(hwnd, message, wParam, lParam);
}

LRESULT PaintWindow::onMouseMove(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("PaintWindow::onMouseMove(0x%08X, 0x%08X)\n"), wParam, lParam);

	if (::GetCapture() == hwnd)
	{
		POINT point = LP2PT(lParam);
		int offset = point.y - hive.drag.origPoint.y;

		switch (hive.drag.ht)
		{
		case Hive::HitTest::minRange: hive.minRange = hive.drag.origValue + offset; break;
		case Hive::HitTest::maxRange: hive.maxRange = hive.drag.origValue + offset; break;
		}

		::InvalidateRect(hwnd, 0, FALSE);
	}

	return __super::onWndProc(hwnd, message, wParam, lParam);
}

LRESULT PaintWindow::onLButtonUp(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("PaintWindow::onLButtonUp(0x%08X, 0x%08X)\n"), wParam, lParam);

	::ReleaseCapture();

	return __super::onWndProc(hwnd, message, wParam, lParam);
}
