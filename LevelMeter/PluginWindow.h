#pragma once

//
// このクラスは AviUtl が作成したプラグインウィンドウです。
//
struct PluginWindow : Tools::Window2
{
	//
	// コンフィグファイルのセクション名に使用されます。
	//
	const std::wstring internalName;

	//
	// コンストラクタです。
	//
	PluginWindow(LPCWSTR internalName)
		: internalName(internalName)
	{
		// このウィンドウにウィンドウホルダー拡張を追加します。
		// これによって、このクラス及び派生クラスでクライアント領域にターゲット(他のウィンドウ)を保持できるようになります。
		addExtension(std::make_shared<Tools::AviUtl::WindowHolderExtension>());
	}

	//
	// 内部で使用される名前を返します。
	//
	LPCWSTR getInternalName() const
	{
		return internalName.c_str();
	}

	//
	// ウィンドウプロシージャです。
	//
	LRESULT onWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_AVIUTL_FILTER_INIT) // このメッセージはサブプロセスのウィンドウの初期化が完了したときに通知されます。
		{
			MY_TRACE(_T("PluginWindow::onWndProc(WM_AVIUTL_FILTER_INIT, 0x%08X, 0x%08X)\n"), wParam, lParam);

			// ウィンドウホルダー拡張を取得します。(コンストラクタで追加されています)
			auto windowHolderExtension = getExtension<Tools::AviUtl::WindowHolderExtension>();
			if (windowHolderExtension)
			{
				// サブプロセスのウィンドウの初期化が完了したので、
				// ホルダー(このウィンドウ)のターゲットに指定し、リサイズします。

				windowHolderExtension->setTarget((HWND)wParam);
				windowHolderExtension->resize(hwnd);
			}
		}

		return __super::onWndProc(hwnd, message, wParam, lParam);
	}
};
