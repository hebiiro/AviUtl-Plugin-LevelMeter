#include "pch.h"
#include "MainWindow.h"

//--------------------------------------------------------------------

void MainWindow::doPaint()
{
	ClientDC dc(m_hwnd);
	RECT rc; ::GetClientRect(m_hwnd, &rc);

	doPaint(dc, rc);
}

void MainWindow::doPaint(HDC dc, const RECT& rc)
{
	MakeCurrent makeCurrent(dc, m_rc);
	int w = getWidth(rc);
	int h = getHeight(rc);

	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(m_vg, (float)w, (float)h, 1.0f);
	nvgSave(m_vg);

	doPaint(rc);

	nvgRestore(m_vg);
	nvgEndFrame(m_vg);

	glFlush();
	::SwapBuffers(dc);
}

void MainWindow::doPaint(const RECT& rc)
{
	int w = getWidth(rc);
	int h = getHeight(rc);

	if (w <= 0 || h <= 0) return;

	{
		// 背景を描画する。

		nvgBeginPath(m_vg);
		nvgRect(m_vg, 0.0f, 0.0f, (float)w, (float)h);
		nvgFillColor(m_vg, g_design.fillColor);
		nvgFill(m_vg);
	}

	if (m_image)
	{
		int imgw = 0, imgh = 0;
		nvgImageSize(m_vg, m_image, &imgw, &imgh);

		NVGpaint imgPaint = nvgImagePattern(m_vg,
			(float)g_design.imageOffset.x, (float)g_design.imageOffset.y,
			(float)imgw * g_design.imageScale, (float)imgh * g_design.imageScale,
			g_design.imageAngle / 360.0f * (2.0f * NVG_PI), m_image, g_design.imageAlpha);
		nvgBeginPath(m_vg);
		nvgRect(m_vg, 0.0f, 0.0f, (float)w, (float)h);
		nvgFillPaint(m_vg, imgPaint);
		nvgFill(m_vg);
	}

	{
		// RMS とピークを描画する。

		RECT rects[2] = { rc, rc };
		rects[0].left = rc.left + ::MulDiv(w, 1, 4);
		rects[0].right = rc.left + ::MulDiv(w, 2, 4);
		rects[1].left = rc.left + ::MulDiv(w, 2, 4);
		rects[1].right = rc.left + ::MulDiv(w, 3, 4);

		for (int i = 0; i < 2; i++)
		{
			int top = rects[i].top;
			int bottom = rects[i].bottom;
			int h = rects[i].bottom - rects[i].top;

			if (m_level[i] > m_minRange)
			{
				double level = (m_level[i] - m_minRange) / (m_maxRange - m_minRange);

				rects[i].top = bottom - (int)(h * level);

				float x = (float)(rects[i].left);
				float y = (float)(rects[i].top);
				float w = (float)(getWidth(rects[i]));
				float h = (float)(getHeight(rects[i]));

				NVGpaint paint = nvgLinearGradient(m_vg,
					x, (float)rc.top, x, (float)rc.bottom,
					g_design.rmsColor0, g_design.rmsColor1);
				nvgBeginPath(m_vg);
				nvgRect(m_vg, x, y, w, h);
				nvgFillPaint(m_vg, paint);
				nvgFill(m_vg);
			}

			if (m_peak[i] > m_minRange)
			{
				double peak = (m_peak[i] - m_minRange) / (m_maxRange - m_minRange);

				int peakY = bottom - (int)(h * peak);

				{
					float mx = (float)(rects[i].left);
					float my = (float)(peakY);
					float lx = (float)(rects[i].right);
					float ly = (float)(peakY);

					nvgBeginPath(m_vg);
					nvgMoveTo(m_vg, mx, my);
					nvgLineTo(m_vg, lx, ly);
					nvgStrokeWidth(m_vg, (float)g_design.peakWidth);
					nvgStrokeColor(m_vg, g_design.peakColor);
					nvgStroke(m_vg);
				}

				if (g_design.zebraStep > 0)
				{
					for (int y = 0; y < (bottom - peakY) - g_design.zebraStep / 2; y += g_design.zebraStep)
					{
						float mx = (float)(rects[i].left);
						float my = (float)(bottom - y);
						float lx = (float)(rects[i].right);
						float ly = (float)(bottom - y);

						nvgBeginPath(m_vg);
						nvgMoveTo(m_vg, mx, my);
						nvgLineTo(m_vg, lx, ly);
						nvgStrokeWidth(m_vg, (float)g_design.zebraWidth);
						nvgStrokeColor(m_vg, g_design.zebraColor);
						nvgStroke(m_vg);
					}
				}
			}
		}
	}

	{
		// ゲージを描画する。

		nvgStrokeWidth(m_vg, (float)g_design.scaleWidth);
		nvgStrokeColor(m_vg, g_design.scaleColor);
		nvgFontSize(m_vg, (float)g_design.scaleTextHeight);
		nvgFontFaceId(m_vg, m_fontDefault);
		nvgTextAlign(m_vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

		int textPadding = 1;
		int textHeight = g_design.scaleTextHeight + textPadding * 2;

		int range = m_maxRange - m_minRange;
		int freq = range * textHeight / h + 1;

		for (int i = m_maxRange; i > m_minRange; i--)
		{
			if (i % freq) continue;

			double di = (double)i - m_minRange;

			RECT rcText = rc;
			rcText.top = (int)(rc.top + h * (1.0 - di / range));
			rcText.bottom = rcText.top + textHeight;

			{
				float mx = (float)(rc.left);
				float my = (float)(rcText.bottom);
				float lx = (float)(rc.left + ::MulDiv(w, 1, 4));
				float ly = (float)(rcText.bottom);

				nvgBeginPath(m_vg);
				nvgMoveTo(m_vg, mx, my);
				nvgLineTo(m_vg, lx, ly);
				nvgStroke(m_vg);
			}

			{
				float mx = (float)(rc.right);
				float my = (float)(rcText.bottom);
				float lx = (float)(rc.left + ::MulDiv(w, 3, 4));
				float ly = (float)(rcText.bottom);

				nvgBeginPath(m_vg);
				nvgMoveTo(m_vg, mx, my);
				nvgLineTo(m_vg, lx, ly);
				nvgStroke(m_vg);
			}

			char text[MAX_PATH] = {};
			::StringCbPrintfA(text, sizeof(text), "%+d", i);

			{
				nvgTextAlign(m_vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

				nvgFontBlur(m_vg, g_design.shadowBlur);
				nvgFillColor(m_vg, g_design.shadowColor);
				nvgText(m_vg, (float)(rcText.left + g_design.shadowOffset.x), (float)(rcText.top + textPadding + g_design.shadowOffset.y), text, 0);

				nvgFontBlur(m_vg, 0.0f);
				nvgFillColor(m_vg, g_design.scaleTextColor);
				nvgText(m_vg, (float)rcText.left, (float)(rcText.top + textPadding), text, 0);
			}

			{
				nvgTextAlign(m_vg, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP);

				nvgFontBlur(m_vg, g_design.shadowBlur);
				nvgFillColor(m_vg, g_design.shadowColor);
				nvgText(m_vg, (float)(rcText.right + g_design.shadowOffset.x), (float)(rcText.top + textPadding + g_design.shadowOffset.y), text, 0);

				nvgFontBlur(m_vg, 0.0f);
				nvgFillColor(m_vg, g_design.scaleTextColor);
				nvgText(m_vg, (float)rcText.right, (float)(rcText.top + textPadding), text, 0);
			}
		}
	}

	{
		// セパレータを描画する。

		for (int i = 2; i <= 2; i++)
		{
			float mx = (float)(rc.left + ::MulDiv(w, i, 4));
			float my = (float)(rc.top);
			float lx = (float)(rc.left + ::MulDiv(w, i, 4));
			float ly = (float)(rc.bottom);

			nvgBeginPath(m_vg);
			nvgMoveTo(m_vg, mx, my);
			nvgLineTo(m_vg, lx, ly);
			nvgStrokeWidth(m_vg, 1.0f);
			nvgStrokeColor(m_vg, g_design.separatorColor);
			nvgStroke(m_vg);
		}
	}
}

//--------------------------------------------------------------------

LRESULT MainWindow::onPaint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PaintDC dc(hwnd);
	RECT rc = dc.m_ps.rcPaint;
	doPaint(dc, rc);

	return 0;
}

//--------------------------------------------------------------------
