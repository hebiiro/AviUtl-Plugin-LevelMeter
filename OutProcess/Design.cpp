#include "pch.h"
#include "Design.h"

//--------------------------------------------------------------------

Design g_design;

//--------------------------------------------------------------------

Design::Design()
{
	::StringCbPrintf(fontDefault, sizeof(fontDefault), L"C:\\Windows\\Fonts\\segoeui.ttf");
	::StringCbPrintf(fontDefault2, sizeof(fontDefault2), L"C:\\Windows\\Fonts\\meiryo.ttc");
	fillColor = RGB(0, 0, 0);
	rmsColor0 = RGB(220, 220, 0);
	rmsColor1 = RGB(220, 110, 0);
	peakColor = RGB(220, 0, 0);
	peakWidth = 2;
	scaleColor = RGB(220, 220, 220);
	scaleWidth = 1;
	scaleTextHeight = 16;
	scaleTextColor = RGB(220, 220, 220);
	separatorColor = RGB(0, 0, 0);
	zebraStep = 3;
	zebraWidth = 1;
	zebraColor = RGB(0, 0, 0);
}

Design::~Design()
{
}

//--------------------------------------------------------------------
