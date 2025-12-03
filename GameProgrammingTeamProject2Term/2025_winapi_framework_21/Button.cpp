#include "pch.h"
#include "Button.h"
#include "Collider.h"
#include "Boss3.h"

Button::Button(Boss3* owner)
	: m_owner(owner)
{
	auto* col = AddComponent <Collider>();
	col->SetSize({ 100.f, 50.f });
	col->SetName(L"Button");
}

Button::~Button()
{

}

void Button::Render(HDC _hdc)
{
	ComponentRender(_hdc);
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
