#include "pch.h"
#include "Design.h"

//--------------------------------------------------------------------

Design g_design;

//--------------------------------------------------------------------

Design::Design()
{
	fontDefault = L"C:\\Windows\\Fonts\\segoeui.ttf";
	fontDefault2 = L"C:\\Windows\\Fonts\\meiryo.ttc";
	imageFileName = L"";
	imageOffset.x = 0;
	imageOffset.y = 0;
	imageScale = 1.0f;
	imageAngle = 0.0f;
	imageAlpha = 1.0f;
	fillColor = nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f);
	rmsColor0 = nvgRGBAf(0.8f, 0.8f, 0.0f, 0.9f);
	rmsColor1 = nvgRGBAf(0.8f, 0.4f, 0.0f, 0.9f);
	peakColor = nvgRGBAf(0.8f, 0.0f, 0.0f, 0.9f);
	peakWidth = 2;
	scaleColor = nvgRGBAf(0.8f, 0.8f, 0.8f, 0.9f);
	scaleWidth = 1;
	scaleTextHeight = 16;
	scaleTextColor = nvgRGBAf(0.8f, 0.8f, 0.8f, 0.9f);
	separatorColor = nvgRGBAf(0.0f, 0.0f, 0.0f, 0.9f);
	zebraStep = 3;
	zebraWidth = 1;
	zebraColor = nvgRGBAf(0.0f, 0.0f, 0.0f, 0.9f);
	shadowBlur = 2.0f;
	shadowOffset.x = 0;
	shadowOffset.y = 1;
	shadowColor = nvgRGBAf(0.0f, 0.0f, 0.0f, 0.9f);
}

Design::~Design()
{
}

//--------------------------------------------------------------------
