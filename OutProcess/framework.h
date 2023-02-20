#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <commdlg.h>
#pragma comment(lib, "comdlg32.lib")
#include <uxtheme.h>
#pragma comment(lib, "uxtheme.lib")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <gl/glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#include <strsafe.h>
#include <comdef.h>

#include <tchar.h>
#include <clocale>
#include <algorithm>
#include <memory>
#include <vector>

#include "NanoVG/nanovg.h"
#include "Common/Tracer.h"
#include "Common/Profile.h"
#include "Common/Dialog.h"
#include "Common/Gdi.h"
#include "Common/WinUtility.h"
#include "LevelMeter/LevelMeter/Common.h"
