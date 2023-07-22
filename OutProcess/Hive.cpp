#include "pch.h"
#include "Hive.h"
#include "ConfigDialog.h"

void Hive::getConfigFileName(LPWSTR fileName, DWORD size) const
{
	::GetModuleFileNameW(instance,  fileName, size);
	::PathRenameExtensionW(fileName, L".ini");
}

void Hive::init(HWND director)
{
	this->director = director;

	mutex.init(0, FALSE, FormatText(_T("LevelMeter.Mutex.%08X"), director));
	fileMapping.init(sizeof(Volume), FormatText(_T("LevelMeter.FileMapping.%08X"), director));
}

void Hive::exit()
{
}

void Hive::load()
{
	MY_TRACE(_T("Hive::load()\n"));

	WCHAR fileName[MAX_PATH] = {};
	getConfigFileName(fileName, std::size(fileName));
	MY_TRACE_WSTR(fileName);

	design.load(fileName);

	getPrivateProfileInt(fileName, L"Config", L"minRange", minRange);
	getPrivateProfileInt(fileName, L"Config", L"maxRange", maxRange);
}

void Hive::save()
{
	MY_TRACE(_T("Hive::save()\n"));

	WCHAR fileName[MAX_PATH] = {};
	getConfigFileName(fileName, std::size(fileName));
	MY_TRACE_WSTR(fileName);

	setPrivateProfileInt(fileName, L"Config", L"minRange", minRange);
	setPrivateProfileInt(fileName, L"Config", L"maxRange", maxRange);

	design.save(fileName);
}

BOOL Hive::redraw()
{
	Volume volume = {};

	{
//		Synchronizer sync(mutex);
		Volume* shared = (Volume*)fileMapping.getBuffer();

		volume = *shared;
	}

	for (int i = 0; i < _countof(level); i++)
	{
		level[i] = volume.level[i];
		peak[i] = volume.peak[i];
	}

	if (volume.time != 0)
	{
		DWORD time = ::timeGetTime();

		if (volume.time > time)
		{
			DWORD sleepTime = volume.time - time;
			MY_TRACE(_T("%d, %.3f, %.3f, %.3f秒間スリープします\n"), volume.frame, time / 1000.0, volume.time / 1000.0, sleepTime / 1000.0);
			::Sleep(sleepTime / 2);
		}
		else if (time - volume.time > 5) // 許容範囲以上に遅れている場合は描画を行わない。
		{
			MY_TRACE(_T("%d, %.3f, %.3f, 遅れているので無視します\n"), volume.frame, time / 1000.0, volume.time / 1000.0);
			return FALSE;
		}
	}
	else
	{
		MY_TRACE_INT(volume.frame);
	}

	return TRUE;
}

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

int WINAPI GetDlgItemComboBox(HWND dlg, int id)
{
	HWND control = ::GetDlgItem(dlg, id);
	return ComboBox_GetCurSel(control);
}

int WINAPI SetDlgItemComboBox(HWND dlg, int id, int value)
{
	HWND control = ::GetDlgItem(dlg, id);
	return ComboBox_SetCurSel(control, value);
}

BOOL Hive::config(HWND hwnd)
{
	MY_TRACE(_T("Hive::config()\n"));

	ConfigDialog dialog(instance, hwnd);

	::SetDlgItemBSTR(dialog, IDC_FONT_DEFAULT, design.fontDefault);
	::SetDlgItemBSTR(dialog, IDC_FONT_DEFAULT_2, design.fontDefault2);
	::SetDlgItemBSTR(dialog, IDC_IMAGE_FILE_NAME, design.image.fileName);
	::SetDlgItemInt(dialog, IDC_IMAGE_X, design.image.offset.x, TRUE);
	::SetDlgItemInt(dialog, IDC_IMAGE_Y, design.image.offset.y, TRUE);
	::SetDlgItemFloat(dialog, IDC_IMAGE_SCALE, design.image.scale);
	::SetDlgItemFloat(dialog, IDC_IMAGE_ANGLE, design.image.angle);
	::SetDlgItemFloat(dialog, IDC_IMAGE_ALPHA, design.image.alpha);
	::SetDlgItemInt(dialog, IDC_BACKGROUND_FILL_START, design.background.fill.start, FALSE);
	::SetDlgItemInt(dialog, IDC_BACKGROUND_FILL_END, design.background.fill.end, FALSE);
	::SetDlgItemInt(dialog, IDC_VOLUME_LEVEL_FILL_START, design.volume.level.fill.start, FALSE);
	::SetDlgItemInt(dialog, IDC_VOLUME_LEVEL_FILL_END, design.volume.level.fill.end, FALSE);
	::SetDlgItemInt(dialog, IDC_VOLUME_PEAK_STROKE_COLOR, design.volume.peak.stroke.color, FALSE);
	::SetDlgItemInt(dialog, IDC_VOLUME_PEAK_STROKE_WIDTH, design.volume.peak.stroke.width, FALSE);
	::SetDlgItemComboBox(dialog, IDC_VOLUME_PEAK_STROKE_STYLE, design.volume.peak.stroke.style);
	::SetDlgItemInt(dialog, IDC_VOLUME_ZEBRA_STROKE_COLOR, design.volume.zebra.stroke.color, FALSE);
	::SetDlgItemInt(dialog, IDC_VOLUME_ZEBRA_STROKE_WIDTH, design.volume.zebra.stroke.width, FALSE);
	::SetDlgItemComboBox(dialog, IDC_VOLUME_ZEBRA_STROKE_STYLE, design.volume.zebra.stroke.style);
	::SetDlgItemInt(dialog, IDC_VOLUME_ZEBRA_STEP, design.volume.zebra.step, FALSE);
	::SetDlgItemInt(dialog, IDC_SCALE_STROKE_COLOR, design.scale.stroke.color, FALSE);
	::SetDlgItemInt(dialog, IDC_SCALE_STROKE_WIDTH, design.scale.stroke.width, FALSE);
	::SetDlgItemComboBox(dialog, IDC_SCALE_STROKE_STYLE, design.scale.stroke.style);
	::SetDlgItemFloat(dialog, IDC_SCALE_TEXT_DILATE, design.scale.text.dilate);
	::SetDlgItemFloat(dialog, IDC_SCALE_TEXT_BLUR, design.scale.text.blur);
	::SetDlgItemInt(dialog, IDC_SCALE_TEXT_COLOR, design.scale.text.color, FALSE);
	::SetDlgItemInt(dialog, IDC_SCALE_TEXT_HEIGHT, design.scale.text.height, TRUE);
	::SetDlgItemInt(dialog, IDC_SCALE_TEXT_SHADOW_COLOR, design.scale.text.shadow.color, FALSE);
	::SetDlgItemFloat(dialog, IDC_SCALE_TEXT_SHADOW_DILATE, design.scale.text.shadow.dilate);
	::SetDlgItemFloat(dialog, IDC_SCALE_TEXT_SHADOW_BLUR, design.scale.text.shadow.blur);
	::SetDlgItemInt(dialog, IDC_SCALE_TEXT_SHADOW_OFFSET_X, design.scale.text.shadow.offset.x, TRUE);
	::SetDlgItemInt(dialog, IDC_SCALE_TEXT_SHADOW_OFFSET_Y, design.scale.text.shadow.offset.y, TRUE);
	::SetDlgItemInt(dialog, IDC_MIN_RANGE, minRange, TRUE);
	::SetDlgItemInt(dialog, IDC_MAX_RANGE, maxRange, TRUE);

	int retValue = dialog.doModal();

	if (IDOK != retValue)
		return FALSE;

	design.fontDefault = ::GetDlgItemBSTR(dialog, IDC_FONT_DEFAULT);
	design.fontDefault2 = ::GetDlgItemBSTR(dialog, IDC_FONT_DEFAULT_2);
	design.image.fileName = ::GetDlgItemBSTR(dialog, IDC_IMAGE_FILE_NAME);
	design.image.offset.x = ::GetDlgItemInt(dialog, IDC_IMAGE_X, 0, TRUE);
	design.image.offset.y = ::GetDlgItemInt(dialog, IDC_IMAGE_Y, 0, TRUE);
	design.image.scale = ::GetDlgItemFloat(dialog, IDC_IMAGE_SCALE);
	design.image.angle = ::GetDlgItemFloat(dialog, IDC_IMAGE_ANGLE);
	design.image.alpha = ::GetDlgItemFloat(dialog, IDC_IMAGE_ALPHA);
	design.background.fill.start = ::GetDlgItemInt(dialog, IDC_BACKGROUND_FILL_START, 0, FALSE);
	design.background.fill.end = ::GetDlgItemInt(dialog, IDC_BACKGROUND_FILL_END, 0, FALSE);
	design.volume.level.fill.start = ::GetDlgItemInt(dialog, IDC_VOLUME_LEVEL_FILL_START, 0, FALSE);
	design.volume.level.fill.end = ::GetDlgItemInt(dialog, IDC_VOLUME_LEVEL_FILL_END, 0, FALSE);
	design.volume.peak.stroke.color = ::GetDlgItemInt(dialog, IDC_VOLUME_PEAK_STROKE_COLOR, 0, FALSE);
	design.volume.peak.stroke.width = ::GetDlgItemInt(dialog, IDC_VOLUME_PEAK_STROKE_WIDTH, 0, FALSE);
	design.volume.peak.stroke.style = ::GetDlgItemComboBox(dialog, IDC_VOLUME_PEAK_STROKE_STYLE);
	design.volume.zebra.stroke.color = ::GetDlgItemInt(dialog, IDC_VOLUME_ZEBRA_STROKE_COLOR, 0, FALSE);
	design.volume.zebra.stroke.width = ::GetDlgItemInt(dialog, IDC_VOLUME_ZEBRA_STROKE_WIDTH, 0, FALSE);
	design.volume.zebra.stroke.style = ::GetDlgItemComboBox(dialog, IDC_VOLUME_ZEBRA_STROKE_STYLE);
	design.volume.zebra.step = ::GetDlgItemInt(dialog, IDC_VOLUME_ZEBRA_STEP, 0, FALSE);
	design.scale.stroke.color = ::GetDlgItemInt(dialog, IDC_SCALE_STROKE_COLOR, 0, FALSE);
	design.scale.stroke.width = ::GetDlgItemInt(dialog, IDC_SCALE_STROKE_WIDTH, 0, FALSE);
	design.scale.stroke.style = ::GetDlgItemComboBox(dialog, IDC_SCALE_STROKE_STYLE);
	design.scale.text.dilate = ::GetDlgItemFloat(dialog, IDC_SCALE_TEXT_DILATE);
	design.scale.text.blur = ::GetDlgItemFloat(dialog, IDC_SCALE_TEXT_BLUR);
	design.scale.text.color = ::GetDlgItemInt(dialog, IDC_SCALE_TEXT_COLOR, 0, FALSE);
	design.scale.text.height = ::GetDlgItemInt(dialog, IDC_SCALE_TEXT_HEIGHT, 0, TRUE);
	design.scale.text.shadow.color = ::GetDlgItemInt(dialog, IDC_SCALE_TEXT_SHADOW_COLOR, 0, FALSE);
	design.scale.text.shadow.dilate = ::GetDlgItemFloat(dialog, IDC_SCALE_TEXT_SHADOW_DILATE);
	design.scale.text.shadow.blur = ::GetDlgItemFloat(dialog, IDC_SCALE_TEXT_SHADOW_BLUR);
	design.scale.text.shadow.offset.x = ::GetDlgItemInt(dialog, IDC_SCALE_TEXT_SHADOW_OFFSET_X, 0, TRUE);
	design.scale.text.shadow.offset.y = ::GetDlgItemInt(dialog, IDC_SCALE_TEXT_SHADOW_OFFSET_Y, 0, TRUE);
	minRange = ::GetDlgItemInt(dialog, IDC_MIN_RANGE, 0, TRUE);
	maxRange = ::GetDlgItemInt(dialog, IDC_MAX_RANGE, 0, TRUE);

	return TRUE;
}
