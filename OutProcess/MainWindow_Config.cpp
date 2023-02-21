#include "pch.h"
#include "MainWindow.h"
#include "ConfigDialog.h"

//--------------------------------------------------------------------

_bstr_t WINAPI GetDlgItemBSTR(HWND dlg, int id)
{
	WCHAR text[MAX_PATH];
	::GetDlgItemTextW(dlg, id, text, MAX_PATH);
	return text;
}

BOOL WINAPI SetDlgItemBSTR(HWND dlg, int id, BSTR value)
{
	return ::SetDlgItemTextW(dlg, id, value);
}

float WINAPI GetDlgItemFloat(HWND dlg, int id)
{
	TCHAR text[MAX_PATH];
	::GetDlgItemText(dlg, id, text, MAX_PATH);
	return _tcstof(text, 0);
}

BOOL WINAPI SetDlgItemFloat(HWND dlg, int id, float value)
{
	TCHAR text[MAX_PATH];
	::StringCbPrintf(text, sizeof(text), _T("%.3f"), value);
	return ::SetDlgItemText(dlg, id, text);
}

//--------------------------------------------------------------------

void MainWindow::load()
{
	MY_TRACE(_T("MainWindow::load()\n"));

	WCHAR fileName[MAX_PATH] = {};
	::GetModuleFileNameW(g_instance,  fileName, MAX_PATH);
	::PathRenameExtensionW(fileName, L".ini");
	MY_TRACE_WSTR(fileName);

	getPrivateProfileBSTR(fileName, L"Config", L"fontDefault", g_design.fontDefault);
	getPrivateProfileBSTR(fileName, L"Config", L"fontDefault2", g_design.fontDefault2);
	getPrivateProfileBSTR(fileName, L"Config", L"imageFileName", g_design.imageFileName);
	getPrivateProfileInt(fileName, L"Config", L"imageOffset.x", g_design.imageOffset.x);
	getPrivateProfileInt(fileName, L"Config", L"imageOffset.y", g_design.imageOffset.y);
	getPrivateProfileReal(fileName, L"Config", L"imageScale", g_design.imageScale);
	getPrivateProfileReal(fileName, L"Config", L"imageAngle", g_design.imageAngle);
	getPrivateProfileReal(fileName, L"Config", L"imageAlpha", g_design.imageAlpha);
	getPrivateProfileColor(fileName, L"Config", L"fillColor", g_design.fillColor);
	getPrivateProfileColor(fileName, L"Config", L"rmsColor0", g_design.rmsColor0);
	getPrivateProfileColor(fileName, L"Config", L"rmsColor1", g_design.rmsColor1);
	getPrivateProfileColor(fileName, L"Config", L"peakColor", g_design.peakColor);
	getPrivateProfileInt(fileName, L"Config", L"peakWidth", g_design.peakWidth);
	getPrivateProfileColor(fileName, L"Config", L"scaleColor", g_design.scaleColor);
	getPrivateProfileInt(fileName, L"Config", L"scaleWidth", g_design.scaleWidth);
	getPrivateProfileColor(fileName, L"Config", L"scaleTextColor", g_design.scaleTextColor);
	getPrivateProfileColor(fileName, L"Config", L"separatorColor", g_design.separatorColor);
	getPrivateProfileColor(fileName, L"Config", L"zebraColor", g_design.zebraColor);
	getPrivateProfileInt(fileName, L"Config", L"zebraWidth", g_design.zebraWidth);
	getPrivateProfileInt(fileName, L"Config", L"zebraStep", g_design.zebraStep);
	getPrivateProfileReal(fileName, L"Config", L"shadowBlur", g_design.shadowBlur);
	getPrivateProfileInt(fileName, L"Config", L"shadowOffset.x", g_design.shadowOffset.x);
	getPrivateProfileInt(fileName, L"Config", L"shadowOffset.y", g_design.shadowOffset.y);
	getPrivateProfileColor(fileName, L"Config", L"shadowColor", g_design.shadowColor);
	getPrivateProfileInt(fileName, L"Config", L"minRange", m_minRange);
	getPrivateProfileInt(fileName, L"Config", L"maxRange", m_maxRange);
}

void MainWindow::save()
{
	MY_TRACE(_T("MainWindow::save()\n"));

	WCHAR fileName[MAX_PATH] = {};
	::GetModuleFileNameW(g_instance,  fileName, MAX_PATH);
	::PathRenameExtensionW(fileName, L".ini");
	MY_TRACE_WSTR(fileName);

	setPrivateProfileBSTR(fileName, L"Config", L"fontDefault", g_design.fontDefault);
	setPrivateProfileBSTR(fileName, L"Config", L"fontDefault2", g_design.fontDefault2);
	setPrivateProfileBSTR(fileName, L"Config", L"imageFileName", g_design.imageFileName);
	setPrivateProfileInt(fileName, L"Config", L"imageOffset.x", g_design.imageOffset.x);
	setPrivateProfileInt(fileName, L"Config", L"imageOffset.y", g_design.imageOffset.y);
	setPrivateProfileReal(fileName, L"Config", L"imageScale", g_design.imageScale);
	setPrivateProfileReal(fileName, L"Config", L"imageAngle", g_design.imageAngle);
	setPrivateProfileReal(fileName, L"Config", L"imageAlpha", g_design.imageAlpha);
	setPrivateProfileColor(fileName, L"Config", L"fillColor", g_design.fillColor);
	setPrivateProfileColor(fileName, L"Config", L"rmsColor0", g_design.rmsColor0);
	setPrivateProfileColor(fileName, L"Config", L"rmsColor1", g_design.rmsColor1);
	setPrivateProfileColor(fileName, L"Config", L"peakColor", g_design.peakColor);
	setPrivateProfileInt(fileName, L"Config", L"peakWidth", g_design.peakWidth);
	setPrivateProfileColor(fileName, L"Config", L"scaleColor", g_design.scaleColor);
	setPrivateProfileInt(fileName, L"Config", L"scaleWidth", g_design.scaleWidth);
	setPrivateProfileColor(fileName, L"Config", L"scaleTextColor", g_design.scaleTextColor);
	setPrivateProfileColor(fileName, L"Config", L"separatorColor", g_design.separatorColor);
	setPrivateProfileColor(fileName, L"Config", L"zebraColor", g_design.zebraColor);
	setPrivateProfileInt(fileName, L"Config", L"zebraWidth", g_design.zebraWidth);
	setPrivateProfileInt(fileName, L"Config", L"zebraStep", g_design.zebraStep);
	setPrivateProfileReal(fileName, L"Config", L"shadowBlur", g_design.shadowBlur);
	setPrivateProfileInt(fileName, L"Config", L"shadowOffset.x", g_design.shadowOffset.x);
	setPrivateProfileInt(fileName, L"Config", L"shadowOffset.y", g_design.shadowOffset.y);
	setPrivateProfileColor(fileName, L"Config", L"shadowColor", g_design.shadowColor);
	setPrivateProfileInt(fileName, L"Config", L"minRange", m_minRange);
	setPrivateProfileInt(fileName, L"Config", L"maxRange", m_maxRange);
}

BOOL MainWindow::config(HWND hwnd)
{
	MY_TRACE(_T("MainWindow::config()\n"));

	ConfigDialog dialog(hwnd);

	::SetDlgItemBSTR(dialog, IDC_FONT_DEFAULT, g_design.fontDefault);
	::SetDlgItemBSTR(dialog, IDC_FONT_DEFAULT_2, g_design.fontDefault2);
	::SetDlgItemBSTR(dialog, IDC_IMAGE_FILE_NAME, g_design.imageFileName);
	::SetDlgItemInt(dialog, IDC_IMAGE_X, g_design.imageOffset.x, TRUE);
	::SetDlgItemInt(dialog, IDC_IMAGE_Y, g_design.imageOffset.y, TRUE);
	::SetDlgItemFloat(dialog, IDC_IMAGE_SCALE, g_design.imageScale);
	::SetDlgItemFloat(dialog, IDC_IMAGE_ANGLE, g_design.imageAngle);
	::SetDlgItemFloat(dialog, IDC_IMAGE_ALPHA, g_design.imageAlpha);
	::SetDlgItemInt(dialog, IDC_FILL_COLOR, g_design.fillColor, FALSE);
	::SetDlgItemInt(dialog, IDC_RMS_COLOR_0, g_design.rmsColor0, FALSE);
	::SetDlgItemInt(dialog, IDC_RMS_COLOR_1, g_design.rmsColor1, FALSE);
	::SetDlgItemInt(dialog, IDC_PEAK_COLOR, g_design.peakColor, FALSE);
	::SetDlgItemInt(dialog, IDC_PEAK_WIDTH, g_design.peakWidth, FALSE);
	::SetDlgItemInt(dialog, IDC_SCALE_COLOR, g_design.scaleColor, FALSE);
	::SetDlgItemInt(dialog, IDC_SCALE_WIDTH, g_design.scaleWidth, FALSE);
	::SetDlgItemInt(dialog, IDC_SCALE_TEXT_COLOR, g_design.scaleTextColor, FALSE);
	::SetDlgItemInt(dialog, IDC_SEPARATOR_COLOR, g_design.separatorColor, FALSE);
	::SetDlgItemInt(dialog, IDC_ZEBRA_COLOR, g_design.zebraColor, FALSE);
	::SetDlgItemInt(dialog, IDC_ZEBRA_WIDTH, g_design.zebraWidth, FALSE);
	::SetDlgItemInt(dialog, IDC_ZEBRA_STEP, g_design.zebraStep, FALSE);
	::SetDlgItemFloat(dialog, IDC_SHADOW_BLUR, g_design.shadowBlur);
	::SetDlgItemInt(dialog, IDC_SHADOW_X, g_design.shadowOffset.x, TRUE);
	::SetDlgItemInt(dialog, IDC_SHADOW_Y, g_design.shadowOffset.y, TRUE);
	::SetDlgItemInt(dialog, IDC_SHADOW_COLOR, g_design.shadowColor, FALSE);
	::SetDlgItemInt(dialog, IDC_MIN_RANGE, m_minRange, TRUE);
	::SetDlgItemInt(dialog, IDC_MAX_RANGE, m_maxRange, TRUE);

	int retValue = dialog.doModal();

	if (IDOK != retValue)
		return FALSE;

	g_design.fontDefault = ::GetDlgItemBSTR(dialog, IDC_FONT_DEFAULT);
	g_design.fontDefault2 = ::GetDlgItemBSTR(dialog, IDC_FONT_DEFAULT_2);
	g_design.imageFileName = ::GetDlgItemBSTR(dialog, IDC_IMAGE_FILE_NAME);
	g_design.imageOffset.x = ::GetDlgItemInt(dialog, IDC_IMAGE_X, 0, TRUE);
	g_design.imageOffset.y = ::GetDlgItemInt(dialog, IDC_IMAGE_Y, 0, TRUE);
	g_design.imageScale = ::GetDlgItemFloat(dialog, IDC_IMAGE_SCALE);
	g_design.imageAngle = ::GetDlgItemFloat(dialog, IDC_IMAGE_ANGLE);
	g_design.imageAlpha = ::GetDlgItemFloat(dialog, IDC_IMAGE_ALPHA);
	g_design.fillColor = ::GetDlgItemInt(dialog, IDC_FILL_COLOR, 0, FALSE);
	g_design.rmsColor0 = ::GetDlgItemInt(dialog, IDC_RMS_COLOR_0, 0, FALSE);
	g_design.rmsColor1 = ::GetDlgItemInt(dialog, IDC_RMS_COLOR_1, 0, FALSE);
	g_design.peakColor = ::GetDlgItemInt(dialog, IDC_PEAK_COLOR, 0, FALSE);
	g_design.peakWidth = ::GetDlgItemInt(dialog, IDC_PEAK_WIDTH, 0, FALSE);
	g_design.scaleColor = ::GetDlgItemInt(dialog, IDC_SCALE_COLOR, 0, FALSE);
	g_design.scaleWidth = ::GetDlgItemInt(dialog, IDC_SCALE_WIDTH, 0, FALSE);
	g_design.scaleTextColor = ::GetDlgItemInt(dialog, IDC_SCALE_TEXT_COLOR, 0, FALSE);
	g_design.separatorColor = ::GetDlgItemInt(dialog, IDC_SEPARATOR_COLOR, 0, FALSE);
	g_design.zebraColor = ::GetDlgItemInt(dialog, IDC_ZEBRA_COLOR, 0, FALSE);
	g_design.zebraWidth = ::GetDlgItemInt(dialog, IDC_ZEBRA_WIDTH, 0, FALSE);
	g_design.zebraStep = ::GetDlgItemInt(dialog, IDC_ZEBRA_STEP, 0, FALSE);
	g_design.shadowBlur = ::GetDlgItemFloat(dialog, IDC_SHADOW_BLUR);
	g_design.shadowOffset.x = ::GetDlgItemInt(dialog, IDC_SHADOW_X, 0, TRUE);
	g_design.shadowOffset.y = ::GetDlgItemInt(dialog, IDC_SHADOW_Y, 0, TRUE);
	g_design.shadowColor = ::GetDlgItemInt(dialog, IDC_SHADOW_COLOR, 0, FALSE);
	m_minRange = ::GetDlgItemInt(dialog, IDC_MIN_RANGE, 0, TRUE);
	m_maxRange = ::GetDlgItemInt(dialog, IDC_MAX_RANGE, 0, TRUE);

	ClientDC dc(m_hwnd);
	MakeCurrent makeCurrent(dc, m_rc);

	updateDesign();

	return TRUE;
}

void MainWindow::updateDesign()
{
	MY_TRACE(_T("MainWindow::updateDesign()\n"));

	m_fontDefault = nvgCreateFont(m_vg, u8"default", StringU8(g_design.fontDefault));
	m_fontDefault2 = nvgCreateFont(m_vg, u8"default2", StringU8(g_design.fontDefault2));
	nvgAddFallbackFontId(m_vg, m_fontDefault, m_fontDefault2);

	if (m_image)
		nvgDeleteImage(m_vg, m_image);

	m_image = nvgCreateImage(m_vg, StringU8(g_design.imageFileName), 0);
}

//--------------------------------------------------------------------
