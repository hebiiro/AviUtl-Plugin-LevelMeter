#pragma once

inline struct Design : Tools::DesignBase
{
	_bstr_t fontDefault;
	_bstr_t fontDefault2;
	Image image;
	struct Left {
		Image image;
	} left;
	struct Right {
		Image image;
	} right;
	struct Background {
		GradientFill fill;
	} background;
	struct Scale {
		Stroke stroke;
		Text text;
	} scale;
	struct Volume {
		struct Level {
			GradientFill fill;
		} level;
		struct Peak {
			Stroke stroke;
		} peak;
		struct Zebra {
			int step;
			Stroke stroke;
		} zebra;
	} volume;

	Design();
	~Design();
	void load(LPCWSTR fileName);
	void save(LPCWSTR fileName);
} design;
