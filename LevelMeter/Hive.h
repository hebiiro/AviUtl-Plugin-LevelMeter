#pragma once

//
// このクラスは他クラスから共通して使用される変数を保持します。
//
inline struct Hive
{
	struct CommandID
	{
		inline static const UINT ENABLE_MODE_OFF = 1000;
		inline static const UINT ENABLE_MODE_ON = 1001;
		inline static const UINT ENABLE_MODE_ON_WITHOUT_PLAYING = 1002;
		inline static const UINT CONFIG = 2000;
		inline static const UINT WINDOW_BEGIN = 3000;
		inline static const UINT WINDOW_END = 3100;
	};

	struct EnableMode
	{
		inline static const int OFF = 0;
		inline static const int ON = 1;
		inline static const int ON_WITHOUT_PLAYING = 2;
	};

	AviUtl::FilterPlugin* fp = 0;
	int enableMode = EnableMode::ON;
	_bstr_t leftName = L"左レベ";
	_bstr_t rightName = L"右レベ";

	void init(AviUtl::FilterPlugin* fp)
	{
		this->fp = fp;
	}

	void exit()
	{
		this->fp = 0;
	}

	void getConfigFileName(LPWSTR fileName, DWORD size) const
	{
		::GetModuleFileNameW(fp->dll_hinst,  fileName, size);
		::PathRemoveExtensionW(fileName);
		::PathAppendW(fileName, L"LevelMeter.ini");
	}

	void load()
	{
		WCHAR fileName[MAX_PATH] = {};
		getConfigFileName(fileName, std::size(fileName));
		MY_TRACE_WSTR(fileName);

		getPrivateProfileBSTR(fileName, L"Config", L"leftName", leftName);
		getPrivateProfileBSTR(fileName, L"Config", L"rightName", rightName);
		getPrivateProfileInt(fileName, L"Config", L"enableMode", enableMode);
	}

	void save()
	{
		WCHAR fileName[MAX_PATH] = {};
		getConfigFileName(fileName, std::size(fileName));
		MY_TRACE_WSTR(fileName);

		setPrivateProfileBSTR(fileName, L"Config", L"leftName", leftName);
		setPrivateProfileBSTR(fileName, L"Config", L"rightName", rightName);
		setPrivateProfileInt(fileName, L"Config", L"enableMode", enableMode);
	}
} hive;
