#pragma once
#include "PaintWindow.h"

//
// このクラスはレベルメーターを描画するウィンドウのコレクションを管理します。
//
inline struct View
{
	inline static std::vector<std::shared_ptr<PaintWindow>> painters;

	std::shared_ptr<PaintWindow> createPainter(HWND parent, int painterType);
	void doPaint();
	void updateDesign();
} view;
