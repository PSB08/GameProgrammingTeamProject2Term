#include "pch.h"
#include "Floor.h"
#include "Collider.h"
Floor::Floor()
{
	auto* col = AddComponent <Collider>();
	col->SetSize({ 1000.f, 50.f });
	col->SetName(L"Floor");
}

Floor::~Floor()
{
}

void Floor::Update()
{
}

void Floor::Render(HDC _hdc)
{
	ComponentRender(_hdc);
}
