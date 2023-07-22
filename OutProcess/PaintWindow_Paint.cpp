#include "pch.h"
#include "PaintWindow.h"

using namespace Tools;

void PaintWindow::doPaint()
{
	ClientDC dc(*this);
	RECT rc; ::GetClientRect(*this, &rc);

	doPaint(dc, rc);
}

void PaintWindow::doPaint(HDC dc, const RECT& rc)
{
	MakeCurrent makeCurrent(dc, glrc);
	int w = getWidth(rc);
	int h = getHeight(rc);

	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(vg, (float)w, (float)h, 1.0f);
	nvgSave(vg);

	doPaint(rc);

	nvgRestore(vg);
	nvgEndFrame(vg);

	::SwapBuffers(dc);
}

void PaintWindow::doPaint(const RECT& rc)
{
	int w = getWidth(rc);
	int h = getHeight(rc);

	if (w <= 0 || h <= 0) return;

	int textPadding = (int)(design.scale.text.dilate + design.scale.text.blur +
		design.scale.text.shadow.dilate + design.scale.text.shadow.blur);
	int textHeight = design.scale.text.height + textPadding * 2;

	int range = hive.maxRange - hive.minRange;
	int freq = range * textHeight / h + 1;

	{
		// 背景を描画する。

		XYWHRect background(rc.left, rc.top, w, h);

		// 縦のグラデーションで背景を塗りつぶす。
		NVGpaint paint = nvgVertGradient(vg, background, design.background.fill);
		nvgBeginPath(vg);
		nvgXYWHRect(vg, background);
		nvgFillPaint(vg, paint);
		nvgFill(vg);

		// 背景画像を描画する。
		drawImage(vg, image, background, design.image);
	}

	struct Rect { float x, y, w, h; };

	struct Layout {
		BOOL enable;
		float left;
		float center;
		float right;
		int align;
	} layouts[2] = {};

	switch (painterType)
	{
	case PainterType::Both:
		{
			// 左のレイアウト。
			layouts[0].enable = TRUE;
			layouts[0].left = (float)(rc.left + ::MulDiv(w, 0, 4)),
			layouts[0].center = (float)(rc.left + ::MulDiv(w, 1, 4)),
			layouts[0].right = (float)(rc.left + ::MulDiv(w, 2, 4)) - 0.5f,
			layouts[0].align = NVG_ALIGN_LEFT | NVG_ALIGN_BOTTOM;

			// 右のレイアウト。
			layouts[1].enable = TRUE;
			layouts[1].left = (float)(rc.left + ::MulDiv(w, 4, 4)),
			layouts[1].center = (float)(rc.left + ::MulDiv(w, 3, 4)),
			layouts[1].right = (float)(rc.left + ::MulDiv(w, 2, 4)) + 0.5f,
			layouts[1].align = NVG_ALIGN_RIGHT | NVG_ALIGN_BOTTOM;

			break;
		}
	case PainterType::Left:
		{
			// 左のレイアウト。
			layouts[0].enable = TRUE;
			layouts[0].left = (float)(rc.left + ::MulDiv(w, 0, 4)),
			layouts[0].center = (float)(rc.left + ::MulDiv(w, 2, 4)),
			layouts[0].right = (float)(rc.left + ::MulDiv(w, 4, 4)),
			layouts[0].align = NVG_ALIGN_LEFT | NVG_ALIGN_BOTTOM;

			// 右のレイアウト。
			layouts[1].enable = FALSE;

			break;
		}
	case PainterType::Right:
		{
			// 左のレイアウト。
			layouts[0].enable = FALSE;

			// 右のレイアウト。
			layouts[1].enable = TRUE;
			layouts[1].left = (float)(rc.left + ::MulDiv(w, 4, 4)),
			layouts[1].center = (float)(rc.left + ::MulDiv(w, 2, 4)),
			layouts[1].right = (float)(rc.left + ::MulDiv(w, 0, 4)),
			layouts[1].align = NVG_ALIGN_RIGHT | NVG_ALIGN_BOTTOM;

			break;
		}
	}

	{
		// 音量のレベルとピークを描画する。

		for (int j = 0; j < 2; j++)
		{
			if (!layouts[j].enable) continue;

			if (hive.level[j] > hive.minRange)
			{
				// 音量レベルを描画する。

				auto ratio = (hive.level[j] - hive.minRange) / range;
				int y = rc.bottom - (int)(h * ratio);

				Rect level;
				level.x = (float)(layouts[j].center) + DesignBase::offset;
				level.y = (float)(y) + DesignBase::offset;
				level.w = (float)(layouts[j].right - layouts[j].center);
				level.h = (float)(rc.bottom - y);

				Rect gradient;
				gradient.x = (float)(layouts[j].left);
				gradient.y = (float)(rc.top);
				gradient.w = (float)(layouts[j].left);
				gradient.h = (float)(rc.bottom);

				NVGpaint paint = nvgVertGradient(vg, gradient, design.volume.level.fill);
				nvgBeginPath(vg);
				nvgXYWHRect(vg, level);
				nvgFillPaint(vg, paint);
				nvgFill(vg);
			}

			if (hive.peak[j] > hive.minRange)
			{
				auto ratio = (hive.peak[j] - hive.minRange) / range;
				int y = rc.bottom - (int)(h * ratio);

				{
					float mx = (float)(layouts[j].center);
					float my = (float)(y);
					float lx = (float)(layouts[j].right);
					float ly = (float)(y);

					drawLine(vg, mx, my, lx, ly, design.volume.peak.stroke);
				}

				if (design.volume.zebra.step > 0)
				{
					for (int k = 0; k < (rc.bottom - y) - design.volume.zebra.step / 2; k += design.volume.zebra.step)
					{
						float mx = (float)(layouts[j].center);
						float my = (float)(rc.bottom - k);
						float lx = (float)(layouts[j].right);
						float ly = (float)(rc.bottom - k);

						drawLine(vg, mx, my, lx, ly, design.volume.zebra.stroke);
					}
				}
			}
		}
	}

	{
		// 目盛りを描画する。

		for (int i = hive.minRange; i <= hive.maxRange; i++)
		{
			int offset = i - hive.minRange; // minRange から i までのオフセット。
			if (offset % freq) continue; // 描画頻度が高すぎる場合は何もしない。

			auto ratio = (float)offset / range; // 高さに対する比率。
			int y = (int)(rc.bottom - h * ratio); // 描画位置の Y 座標。(ピクセル単位)

			char text[MAX_PATH] = {};
			::StringCbPrintfA(text, sizeof(text), "%+d", i);

			for (int j = 0; j < 2; j++)
			{
				if (!layouts[j].enable) continue;

				{
					// 目盛りの水平線を描画する。

					float mx = (float)(layouts[j].left);
					float my = (float)(y);
					float lx = (float)(layouts[j].center);
					float ly = (float)(y);

					drawLine(vg, mx, my, lx, ly, design.scale.stroke);
				}

				{
					// 目盛りの文字列を描画する。

					float tx = (float)layouts[j].left;
					float ty = (float)(y - textPadding / 2);

					if (layouts[j].align & NVG_ALIGN_RIGHT)
						tx -= (float)textPadding;
					else
						tx += (float)textPadding;

					nvgTextAlign(vg, layouts[j].align);
					drawText(vg, text, tx, ty, design.scale.text, fontDefault);
				}
			}
		}
	}
}

LRESULT PaintWindow::onPaint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PaintDC dc(hwnd);
	RECT rc = dc.m_ps.rcPaint;
	doPaint(dc, rc);

	return 0;
}
