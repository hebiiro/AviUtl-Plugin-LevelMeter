#pragma once
#include "Resource.h"

struct ConfigDialog : Tools::Dialog
{
	inline static const LPCTSTR PROP_NAME_COLOR_BUTTON = _T("ColorButton");

	inline static BOOL isColorButton(HWND control)
	{
		return !!::GetProp(control, PROP_NAME_COLOR_BUTTON);
	}

	inline static BOOL initColorButton(HWND dialog, UINT id)
	{
		HWND control = ::GetDlgItem(dialog, id);

		return ::SetProp(control, PROP_NAME_COLOR_BUTTON, (HANDLE)1);
	}

	inline static BOOL initLineStyleComboBox(HWND dialog, UINT id)
	{
		HWND control = ::GetDlgItem(dialog, id);

		ComboBox_AddString(control, _T("通常"));
		ComboBox_AddString(control, _T("ソリッド"));
		ComboBox_AddString(control, _T("ダッシュ"));
		ComboBox_AddString(control, _T("ドット"));
		ComboBox_AddString(control, _T("グロー"));

		return TRUE;
	}

	inline static BOOL browse(HWND hwnd, UINT id, LPCTSTR filter)
	{
		TCHAR fileName[MAX_PATH];
		::GetDlgItemText(hwnd, id, fileName, MAX_PATH);

		OPENFILENAME ofn = { sizeof(ofn) };
		ofn.Flags = OFN_FILEMUSTEXIST;
		ofn.hwndOwner = hwnd;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = fileName;
		ofn.nMaxFile = MAX_PATH;

		if (!::GetOpenFileName(&ofn))
			return FALSE;

		::SetDlgItemText(hwnd, id, fileName);

		return TRUE;
	}

	ConfigDialog(HINSTANCE instance, HWND parent);

	void onOK() override;
	void onCancel() override;
	INT_PTR onDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};
