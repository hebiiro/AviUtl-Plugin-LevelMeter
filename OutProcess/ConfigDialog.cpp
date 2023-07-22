#include "pch.h"
#include "ConfigDialog.h"
#include "Design.h"

ConfigDialog::ConfigDialog(HINSTANCE instance, HWND parent)
{
	create(instance, MAKEINTRESOURCE(IDD_CONFIG), parent);
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
	case WM_INITDIALOG:
		{
			MY_TRACE(_T("ConfigDialog::onDlgProc(WM_INITDIALOG)\n"));

			initColorButton(*this, IDC_BACKGROUND_FILL_START);
			initColorButton(*this, IDC_BACKGROUND_FILL_END);

			initColorButton(*this, IDC_VOLUME_LEVEL_FILL_START);
			initColorButton(*this, IDC_VOLUME_LEVEL_FILL_END);

			initColorButton(*this, IDC_VOLUME_PEAK_STROKE_COLOR);
			initLineStyleComboBox(*this, IDC_VOLUME_PEAK_STROKE_STYLE);

			initColorButton(*this, IDC_VOLUME_ZEBRA_STROKE_COLOR);
			initLineStyleComboBox(*this, IDC_VOLUME_ZEBRA_STROKE_STYLE);

			initColorButton(*this, IDC_SCALE_STROKE_COLOR);
			initLineStyleComboBox(*this, IDC_SCALE_STROKE_STYLE);

			initColorButton(*this, IDC_SCALE_TEXT_COLOR);
			initColorButton(*this, IDC_SCALE_TEXT_SHADOW_COLOR);

			break;
		}
	case WM_COMMAND:
		{
			UINT id = LOWORD(wParam);
			HWND control = (HWND)lParam;

			switch (id)
			{
			case IDC_FONT_DEFAULT_BROWSE:
			case IDC_FONT_DEFAULT_2_BROWSE:
				{
					browse(hwnd, id - 1,
						_T("Font Files {*.ttf;*.ttc;*.otf}\0*.ttf;*.ttc;*.otf\0")
						_T("All Files {*.*}\0*.*\0"));

					return TRUE;
				}
			case IDC_IMAGE_FILE_NAME_BROWSE:
				{
					browse(hwnd, IDC_IMAGE_FILE_NAME,
						_T("PNG Files {*.png}\0*.png\0")
						_T("All Files {*.*}\0*.*\0"));

					return TRUE;
				}
			}

			if (isColorButton(control))
			{
				DWORD rgba = ::GetDlgItemInt(hwnd, id, 0, FALSE);
				COLORREF color = Tools::MyColor::getCOLORREF(rgba);

				static COLORREF customColors[16] = {};
				CHOOSECOLOR cc { sizeof(cc) };
				cc.hwndOwner = hwnd;
				cc.lpCustColors = customColors;
				cc.rgbResult = color;
				cc.Flags = CC_RGBINIT | CC_FULLOPEN;
				if (!::ChooseColor(&cc)) return TRUE;

				color = Tools::MyColor::getDWORD(cc.rgbResult, rgba);
				::SetDlgItemInt(hwnd, id, color, FALSE);

				::InvalidateRect(control, 0, FALSE);

				return TRUE;
			}

			break;
		}
	case WM_DRAWITEM:
		{
			UINT id = wParam;
			DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;

			if (isColorButton(dis->hwndItem))
			{
				DWORD rgba = ::GetDlgItemInt(hwnd, id, 0, FALSE);
				COLORREF color = Tools::MyColor::getCOLORREF(rgba);
				HBRUSH brush = ::CreateSolidBrush(color);
				::FillRect(dis->hDC, &dis->rcItem, brush);
				::DeleteObject(brush);

				return TRUE;
			}

			break;
		}
	}

	return Dialog::onDlgProc(hwnd, message, wParam, lParam);
}
