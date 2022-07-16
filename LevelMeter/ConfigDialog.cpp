#include "pch.h"
#include "ConfigDialog.h"
#include "LevelMeter.h"

//---------------------------------------------------------------------

ConfigDialog::ConfigDialog(HWND hwnd)
	: Dialog(g_fp->dll_hinst, MAKEINTRESOURCE(IDD_CONFIG), hwnd)
{
}

void ConfigDialog::onOK()
{
	Dialog::onOK();
}

void ConfigDialog::onCancel()
{
	Dialog::onCancel();
}

INT_PTR ConfigDialog::onDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			UINT id = LOWORD(wParam);

			switch (id)
			{
			case IDC_FILL_COLOR:
			case IDC_RMS_COLOR_0:
			case IDC_RMS_COLOR_1:
			case IDC_PEAK_COLOR:
			case IDC_GUAGE_COLOR:
			case IDC_GUAGE_TEXT_COLOR:
			case IDC_SEPARATOR_COLOR:
			case IDC_ZEBRA_COLOR:
				{
					HWND control = (HWND)lParam;

					COLORREF color = ::GetDlgItemInt(hwnd, id, 0, FALSE);

					static COLORREF customColors[16] = {};
					CHOOSECOLOR cc { sizeof(cc) };
					cc.hwndOwner = hwnd;
					cc.lpCustColors = customColors;
					cc.rgbResult = color;
					cc.Flags = CC_RGBINIT | CC_FULLOPEN;
					if (!::ChooseColor(&cc)) return TRUE;

					color = cc.rgbResult;

					::SetDlgItemInt(hwnd, id, color, FALSE);
					::InvalidateRect(control, 0, FALSE);

					return TRUE;
				}
			}

			break;
		}
	case WM_DRAWITEM:
		{
			UINT id = wParam;

			switch (id)
			{
			case IDC_FILL_COLOR:
			case IDC_RMS_COLOR_0:
			case IDC_RMS_COLOR_1:
			case IDC_PEAK_COLOR:
			case IDC_GUAGE_COLOR:
			case IDC_GUAGE_TEXT_COLOR:
			case IDC_SEPARATOR_COLOR:
			case IDC_ZEBRA_COLOR:
				{
					DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;

					COLORREF color = ::GetDlgItemInt(hwnd, id, 0, FALSE);

					HBRUSH brush = ::CreateSolidBrush(color);
					FillRect(dis->hDC, &dis->rcItem, brush);
					::DeleteObject(brush);

					return TRUE;
				}
			}

			break;
		}
	}

	return Dialog::onDlgProc(hwnd, message, wParam, lParam);
}

//---------------------------------------------------------------------
