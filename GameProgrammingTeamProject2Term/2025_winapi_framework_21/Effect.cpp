#include "pch.h"
#include "Effect.h"
#include "Animation.h"

Effect::Effect()
	: m_animator(nullptr)
{
	SetSize({ 150.f, 150.f });
	m_animator = AddComponent<Animator>();
}

Effect::~Effect()
{
}

void Effect::Init(Texture* tex, const std::wstring& animName, Vec2 lt, Vec2 sliceSize, Vec2 step, UINT frameCount, float durPerFrame)
{
	if (!tex || !m_animator)
		return;

	m_animator->CreateAnimation(
		animName,
		tex,
		lt,
		sliceSize,
		step,
		frameCount,
		durPerFrame
	);

	m_animator->Play(animName, PlayMode::Once, 1, 1.f);
}

void Effect::Update()
{
	Object::Update();

	if (!m_animator)
		return;

	Animation* curAnim = m_animator->GetCurrent();
	if (curAnim && curAnim->IsFinished())
		SetDead();
}

void Effect::Render(HDC _hdc)
{
	ComponentRender(_hdc);
}

//¿¹½Ã
/*void BossProjectile::Explode()
{
	auto* effect = new Effect;

	effect->SetPos(GetPos());
	effect->SetSize({ 160.f, 160.f });

	Texture* tex = GET_SINGLE(ResourceManager)->GetTexture(L"explosion");
	effect->Init(
		tex,
		L"explosionOnce",
		{ 0.f, 0.f },
		{ 160.f, 160.f },
		{ 160.f, 0.f },
		8,
		0.06f
	);

	GET_SINGLE(SceneManager)->GetCurScene()->AddObject(effect, Layer::EFFECT);

	SetDead();
}*/