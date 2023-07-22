#pragma once
#include "Hive.h"
#include "PluginWindow.h"
#include "SubPluginWindow.h"
#include "OutProcess.h"

//
// このクラスはプラグインウィンドウのコレクションを管理します。
//
inline struct View
{
	//
	// このクラスはプラグインウィンドウを監視します。
	//
	template<class T>
	struct PluginWindowObserver : T
	{
		//
		// コンストラクタです。
		//
		PluginWindowObserver(LPCWSTR internalName)
			: T(internalName)
		{
		}

		//
		// ウィンドウプロシージャです。
		//
		LRESULT onWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch (message)
			{
			case WM_GETMINMAXINFO:
				{
					MY_TRACE(_T("PluginWindowObserver::onWndProc(WM_CONTEXTMENU, 0x%08X, 0x%08X)\n"), wParam, lParam);

					auto mmi = (MINMAXINFO*)lParam;
					mmi->ptMinTrackSize.x = 0;
					mmi->ptMinTrackSize.y = 0;

					break;
				}
			case WM_CONTEXTMENU:
				{
					MY_TRACE(_T("PluginWindowObserver::onWndProc(WM_CONTEXTMENU, 0x%08X, 0x%08X)\n"), wParam, lParam);

					// コンテキストメニューを表示します。

					onContextMenu();

					break;
				}
			}

			return __super::onWndProc(hwnd, message, wParam, lParam);
		}

		//
		// コンテキストメニューを表示します。
		//
		void onContextMenu()
		{
			// このクラスをテンプレートクラスにしたら
			// view が見つからなくなったのでこうしている。
			View& view = getView();

			POINT cursorPos; ::GetCursorPos(&cursorPos);

			HMENU menu = ::CreatePopupMenu();

			::AppendMenu(menu, MF_STRING, Hive::CommandID::ENABLE_MODE_ON, _T("有効"));
			::AppendMenu(menu, MF_STRING, Hive::CommandID::ENABLE_MODE_ON_WITHOUT_PLAYING, _T("有効(再生時以外)"));
			::AppendMenu(menu, MF_STRING, Hive::CommandID::ENABLE_MODE_OFF, _T("無効"));
			::AppendMenu(menu, MF_SEPARATOR, 0, 0);
			::AppendMenu(menu, MF_STRING, Hive::CommandID::CONFIG, _T("設定"));
			std::size_t c = view.collection.size();
			if (c != 0)
			{
				::AppendMenu(menu, MF_SEPARATOR, 0, 0);
				for (std::size_t i = 0; i < c; i++)
				{
					HWND hwnd = *view.collection[i];
					UINT id = Hive::CommandID::WINDOW_BEGIN + i;

					TCHAR text[MAX_PATH] = {};
					::GetWindowText(hwnd, text, std::size(text));
					::AppendMenu(menu, MF_STRING, id, text);
					if (::IsWindowVisible(hwnd)) ::CheckMenuItem(menu, id, MF_CHECKED);
				}
			}

			switch (hive.enableMode)
			{
			case Hive::EnableMode::OFF: ::CheckMenuItem(menu, Hive::CommandID::ENABLE_MODE_OFF, MF_CHECKED); break;
			case Hive::EnableMode::ON: ::CheckMenuItem(menu, Hive::CommandID::ENABLE_MODE_ON, MF_CHECKED); break;
			case Hive::EnableMode::ON_WITHOUT_PLAYING: ::CheckMenuItem(menu, Hive::CommandID::ENABLE_MODE_ON_WITHOUT_PLAYING, MF_CHECKED); break;
			}

			int id = ::TrackPopupMenu(menu, TPM_NONOTIFY | TPM_RETURNCMD, cursorPos.x, cursorPos.y, 0, *this, 0);

			switch (id)
			{
			case Hive::CommandID::ENABLE_MODE_OFF:
				{
					hive.enableMode = Hive::EnableMode::OFF;

					break;
				}
			case Hive::CommandID::ENABLE_MODE_ON:
				{
					hive.enableMode = Hive::EnableMode::ON;

					break;
				}
			case Hive::CommandID::ENABLE_MODE_ON_WITHOUT_PLAYING:
				{
					hive.enableMode = Hive::EnableMode::ON_WITHOUT_PLAYING;

					break;
				}
			case Hive::CommandID::CONFIG:
				{
					auto windowHolderExtension = T::template getExtension<Tools::AviUtl::WindowHolderExtension>();
					if (windowHolderExtension)
					{
						HWND target = windowHolderExtension->getTarget();
						if (target)
							::PostMessage(target, WM_AVIUTL_FILTER_CONFIG, 0, 0);
					}

					break;
				}
			}

			if (id >= Hive::CommandID::WINDOW_BEGIN && id < Hive::CommandID::WINDOW_END)
				::SendMessage(*view.collection[id - Hive::CommandID::WINDOW_BEGIN], WM_CLOSE, 0, 0);

			::DestroyMenu(menu);
		}
	};

	//
	// プラグインウィンドウのコレクションです。
	//
	std::vector<std::shared_ptr<PluginWindow>> collection;

	template<class T>
	std::shared_ptr<T> createPluginWindow(LPCWSTR internalName)
	{
		std::shared_ptr<PluginWindowObserver<T>> window =
			std::make_shared<PluginWindowObserver<T>>(internalName);
		collection.push_back(window);
		return window;
	}

	void init()
	{
		HWND parent = ::GetWindow(hive.fp->hwnd, GW_OWNER);

		std::shared_ptr<PluginWindow> mainWindow = createPluginWindow<PluginWindow>(L"LR-Window");
		mainWindow->subclass(hive.fp->hwnd);
		::PostThreadMessage(outProcess.pi.dwThreadId, WM_AVIUTL_FILTER_INIT, (WPARAM)(HWND)*mainWindow, PainterType::Both);

		std::shared_ptr<SubPluginWindow> leftWindow = createPluginWindow<SubPluginWindow>(L"L-Window");
		Tools::AviUtl::WindowExtension::create(*leftWindow, hive.fp->dll_hinst, parent, hive.leftName);
		::PostThreadMessage(outProcess.pi.dwThreadId, WM_AVIUTL_FILTER_INIT, (WPARAM)(HWND)*leftWindow, PainterType::Left);

		std::shared_ptr<SubPluginWindow> rightWindow = createPluginWindow<SubPluginWindow>(L"R-Window");
		Tools::AviUtl::WindowExtension::create(*rightWindow, hive.fp->dll_hinst, parent, hive.rightName);
		::PostThreadMessage(outProcess.pi.dwThreadId, WM_AVIUTL_FILTER_INIT, (WPARAM)(HWND)*rightWindow, PainterType::Right);
	}

	void exit()
	{
	}

	void load()
	{
		WCHAR fileName[MAX_PATH] = {};
		hive.getConfigFileName(fileName, std::size(fileName));
		MY_TRACE_WSTR(fileName);

		for (const auto& window : collection)
			getPrivateProfileWindow(fileName, window->getInternalName(), *window);
	}

	void save()
	{
		WCHAR fileName[MAX_PATH] = {};
		hive.getConfigFileName(fileName, std::size(fileName));
		MY_TRACE_WSTR(fileName);

		for (const auto& window : collection)
			setPrivateProfileWindow(fileName, window->getInternalName(), *window);
	}

	//
	// ビューのインスタンスを返します。
	//
	inline static View& getView()
	{
		return view;
	}
} view;
