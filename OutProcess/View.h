#pragma once
#include "PaintWindow.h"

//
// ���̃N���X�̓��x�����[�^�[��`�悷��E�B���h�E�̃R���N�V�������Ǘ����܂��B
//
inline struct View
{
	inline static std::vector<std::shared_ptr<PaintWindow>> painters;

	std::shared_ptr<PaintWindow> createPainter(HWND parent, int painterType);
	void doPaint();
	void updateDesign();
} view;
