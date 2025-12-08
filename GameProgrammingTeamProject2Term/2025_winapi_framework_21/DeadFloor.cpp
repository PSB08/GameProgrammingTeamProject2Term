#include "pch.h"
#include "DeadFloor.h"
#include "Collider.h"

DeadFloor::DeadFloor()
{
	auto* col = AddComponent<Collider>();
	col->SetSize({ 10000.f, 100.f });
	col->SetName(L"DeadFloor");
}

DeadFloor::~DeadFloor()
{
}

void DeadFloor::Update()
{
}

void DeadFloor::Render(HDC _hdc)
{
	ComponentRender(_hdc);
}
