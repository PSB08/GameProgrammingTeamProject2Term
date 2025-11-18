#include "pch.h"
#include "GDISelector.h"
#include "ResourceManager.h"
GDISelector::GDISelector(HDC _hdc, PenType _pen)
	: m_hDC(_hdc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
	, m_hDefaultFont(nullptr)
{
	HPEN hColorPen = GET_SINGLE(ResourceManager)->GetPen(_pen);
	m_hDefaultPen = (HPEN)::SelectObject(_hdc, hColorPen);
}

GDISelector::GDISelector(HDC _hdc, BrushType _brush)
	: m_hDC(_hdc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
	, m_hDefaultFont(nullptr)
{
	HBRUSH hColorBrush = GET_SINGLE(ResourceManager)->GetBrush(_brush);
	m_hDefaultBrush = (HBRUSH)::SelectObject(_hdc, hColorBrush);
}

GDISelector::GDISelector(HDC _hdc, FontType _font)
	: m_hDC(_hdc)
	, m_hDefaultBrush(nullptr)
	, m_hDefaultPen(nullptr)
	, m_hDefaultFont(nullptr)
{
	HFONT hfont = GET_SINGLE(ResourceManager)->GetFont(_font);
	m_hDefaultFont = (HFONT)::SelectObject(_hdc, hfont);
	::SetBkMode(_hdc, TRANSPARENT); // 글자 배경색 투명
}

GDISelector::~GDISelector()
{
	::SelectObject(m_hDC, m_hDefaultPen);
	::SelectObject(m_hDC, m_hDefaultBrush);
	::SelectObject(m_hDC, m_hDefaultFont);
}

