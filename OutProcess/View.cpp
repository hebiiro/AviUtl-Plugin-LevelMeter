#include "pch.h"
#include "View.h"

std::shared_ptr<PaintWindow> View::createPainter(HWND parent, int painterType)
{
	MY_TRACE(_T("View::createPainter(0x%08X, %d)\n"), parent, painterType);

	auto painter = std::make_shared<PaintWindow>(painterType);
	painters.push_back(painter);
	painter->create(hive.instance, parent, _T("Painter"));
	return painter;
}

void View::doPaint()
{
	for (auto& window : painters)
		window->doPaint();
}

void View::updateDesign()
{
	for (auto& window : painters)
		window->updateDesign();
}
