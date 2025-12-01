#pragma once
#include "Object.h"
#include "Texture.h"
#include "Animator.h"

class Effect : public Object
{
public:
	Effect();
	~Effect();

	void Init(Texture* tex,
		const std::wstring& animName,
		Vec2 lt, Vec2 sliceSize, Vec2 step,
		UINT frameCount, float durPerFrame);
	void Update() override;
	void Render(HDC _hdc) override;

private:
	Animator* m_animator;

};

