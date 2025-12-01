#pragma once
#include "Enemy.h"
#include "Texture.h"
#include "Animator.h"

class Boss2;

class Boss2MainCore : public Enemy
{
public:
	Boss2MainCore(Boss2* boss);
	~Boss2MainCore();

	virtual void Update() override;
	void Render(HDC _hdc) override;
	void EnterCollision(Collider* _other) override;

private:
	void SetupAnimations();

private:
	Boss2* m_owner; // ÆÄ±«ÇÒ Boss

	bool m_pendingSceneChange = false;
	float m_delay = 0.f;

	Texture* m_pTexture;
	Animator* m_animator;
};

