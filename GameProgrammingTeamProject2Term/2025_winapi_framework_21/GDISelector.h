#pragma once
class GDISelector
{
public:
	GDISelector(HDC _hdc, PenType _pen);
	GDISelector(HDC _hdc, BrushType _brush);
	GDISelector(HDC _hdc, FontType _font);
	~GDISelector();
private:
	HDC		m_hDC;
	HPEN	m_hDefaultPen;
	HBRUSH	m_hDefaultBrush;
	HFONT   m_hDefaultFont;
};

