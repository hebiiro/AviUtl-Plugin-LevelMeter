#pragma once
#include "PluginWindow.h"

//
// このクラスは独自に作成したプラグインウィンドウです。
// AviUtl が作成したプラグインウィンドウとも互換性があります。
//
struct SubPluginWindow : PluginWindow
{
	//
	// コンストラクタです。
	//
	SubPluginWindow(LPCWSTR internalName)
		: PluginWindow(internalName)
	{
		addExtension(std::make_shared<Tools::AviUtl::WindowExtension>());
	}

	//
	// ウィンドウ作成時のスタイルやウィンドウサイズを指定します。
	//
	virtual BOOL create(DWORD exStyle, LPCTSTR className, LPCTSTR windowName, DWORD style,
		int x, int y, int w, int h, HWND parent, HMENU menu, HINSTANCE instance, LPVOID param)
	{
		MY_TRACE(_T("SubPluginWindow::create()\n"));

		return __super::create(
			exStyle,
			className,
			windowName,
			WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			100, 100, 100, 400,
			parent, menu, instance, param);
	}
};
