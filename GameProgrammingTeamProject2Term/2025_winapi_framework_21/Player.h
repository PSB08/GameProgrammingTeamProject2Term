#pragma once
#include "Object.h"
class Texture;
class Player : public Object
{
public:
	Player();
	~Player();
public:
	void Update() override;
	void Render(HDC _hdc) override;
	virtual void EnterCollision(Collider* _other)override;
	virtual void ExitCollision(Collider* _other)override;
private:
	void CreateProjectile();
	void PlayerJump();
	void PlayerShield();
	void PlayerDash();
private:
	Texture* m_pTexture;
	bool playerCanDamaged;
	bool playerIsInvincibility;
	float jumpPower;
	float dashPower;
	float dashCooltime;
	float shieldCooltime;
	float shieldTime;
	float dashTime;
	float InvincibleHoldTime;
	float InvincibleTime;
	float JumpDelayTime;
	float JumpTime;

private:
	bool m_pendingSceneChange = false;
	float m_delay = 0.f;

};

