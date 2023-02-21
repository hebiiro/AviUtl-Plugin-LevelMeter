#include "pch.h"
#include "ConfigDialog.h"
#include "OutProcess.h"

//---------------------------------------------------------------------

ConfigDialog::ConfigDialog(HWND hwnd)
	: Dialog(g_instance, MAKEINTRESOURCE(IDD_CONFIG), hwnd)
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
			case IDC_IMAGE_FILE_NAME_BROWSE:
				{
					TCHAR fileName[MAX_PATH];
					::GetDlgItemText(hwnd, IDC_IMAGE_FILE_NAME, fileName, MAX_PATH);

					OPENFILENAME ofn = { sizeof(ofn) };
					ofn.Flags = OFN_FILEMUSTEXIST;
					ofn.hwndOwner = hwnd;
					ofn.lpstrFilter =
						_T("PNG Files {*.png}\0*.png\0")
						_T("All Files {*.*}\0*.*\0");
					ofn.lpstrFile = fileName;
					ofn.nMaxFile = MAX_PATH;

					if (::GetOpenFileName(&ofn))
					{
						::SetDlgItemText(hwnd, IDC_IMAGE_FILE_NAME, fileName);
					}

					break;
				}
			case IDC_FILL_COLOR:
			case IDC_RMS_COLOR_0:
			case IDC_RMS_COLOR_1:
			case IDC_PEAK_COLOR:
			case IDC_SCALE_COLOR:
			case IDC_SCALE_TEXT_COLOR:
			case IDC_SEPARATOR_COLOR:
			case IDC_ZEBRA_COLOR:
			case IDC_SHADOW_COLOR:
				{
					HWND control = (HWND)lParam;

					DWORD rgba = ::GetDlgItemInt(hwnd, id, 0, FALSE);
					COLORREF color = MyColor::getCOLORREF(rgba);

					static COLORREF customColors[16] = {};
					CHOOSECOLOR cc { sizeof(cc) };
					cc.hwndOwner = hwnd;
					cc.lpCustColors = customColors;
					cc.rgbResult = color;
					cc.Flags = CC_RGBINIT | CC_FULLOPEN;
					if (!::ChooseColor(&cc)) return TRUE;

					color = MyColor::getDWORD(cc.rgbResult, rgba);

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
			case IDC_SCALE_COLOR:
			case IDC_SCALE_TEXT_COLOR:
			case IDC_SEPARATOR_COLOR:
			case IDC_ZEBRA_COLOR:
			case IDC_SHADOW_COLOR:
				{
					DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;

					DWORD rgba = ::GetDlgItemInt(hwnd, id, 0, FALSE);
					COLORREF color = MyColor::getCOLORREF(rgba);
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
