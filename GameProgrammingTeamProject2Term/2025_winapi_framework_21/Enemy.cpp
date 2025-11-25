#include "pch.h"
#include "Enemy.h"
#include "Collider.h"
#include "SceneManager.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::EnterCollision(Collider* _other)
{

}

void Enemy::StayCollision(Collider* _other)
{
}

void Enemy::ExitCollision(Collider* _other)
{
}

void Enemy::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();
	RECT_RENDER(_hdc, pos.x, pos.y
		, size.x, size.y);
	//::SelectObject(_hdc, holdbrush);
	//::DeleteObject(hbrush);
	ComponentRender(_hdc);
}