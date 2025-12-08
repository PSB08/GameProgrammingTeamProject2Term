#include "pch.h"
#include "Button.h"
#include "Collider.h"
#include "Boss3.h"
#include "ResourceManager.h"

Button::Button(Boss3* owner)
	: m_owner(owner)
{
	m_Texture = GET_SINGLE(ResourceManager)->GetTexture(L"Button");
	auto* col = AddComponent <Collider>();
	col->SetSize({ 100.f, 50.f });
	col->SetName(L"Button");
}

Button::~Button()
{

}

void Button::Render(HDC _hdc)
{
	if (m_Texture == NULL)
	{
		cout << "Null";
		return;
	}

	Vec2 pos = GetPos();
	Vec2 size = GetSize();
	int width = m_Texture->GetWidth();
	int height = m_Texture->GetHeight();


	::TransparentBlt(_hdc
		, (int)(pos.x - size.x / 2)
		, (int)(pos.y - size.y / 2)
		, size.x, size.y,
		m_Texture->GetTextureDC(),
		0, 0, width, height, RGB(0, 0, 0));
}

void Button::EnterCollision(Collider* _other)
{
	cout << "DO";
	if (_other->GetName() == L"Player")
	{
		m_owner->PressedButton();
		SetDead();
	}
}
