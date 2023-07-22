#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#pragma comment(lib, "msimg32.lib")
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <commdlg.h>
#pragma comment(lib, "comdlg32.lib")
#include <vsstyle.h>
#include <uxtheme.h>
#pragma comment(lib, "uxtheme.lib")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <strsafe.h>
#include <comdef.h>

#include <tchar.h>
#include <crtdbg.h>
#include <locale.h>

#include <cmath>
#include <algorithm>
#include <memory>
#include <vector>
#include <map>

#include "AviUtl/aviutl_exedit_sdk/aviutl.hpp"
#include "AviUtl/aviutl_exedit_sdk/exedit.hpp"
#include "Common/Tracer.h"
#include "Common/WinUtility.h"
#include "Common/Profile.h"
#include "Common/Gdi.h"
#include "Common/Window2.h"
#include "Common/Dialog.h"
#include "Common/Hook.h"
#include "Common/AviUtlInternal.h"
#include "Common/AviUtl//Window.h"
#include "Common/AviUtl//WindowHolder.h"
#include "Detours.4.0.1/detours.h"
#pragma comment(lib, "Detours.4.0.1/detours.lib")
#include "Share.h"
