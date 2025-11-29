#pragma once
#include "Object.h"
#include "Animator.h"
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
public:
	void PlayerBounce();
	Vec2 GetPlayerPos() { return m_playerPos; }
private:
	Texture* m_pNormalTexture;
	Texture* m_pShootingTexture;
	Animator* m_animator;
	Vec2 m_playerPos;
private: // bool 변수
	bool playerCanDamaged;
	bool playerIsInvincibility;
	bool isShooting = false;
	bool isMoving = false;
private: //float변수
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
	float m_shootDelayTime;
private:
	bool m_pendingSceneChange = false;
	float m_delay = 0.f;

};

