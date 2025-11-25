#include "pch.h"
#include "Player.h"
#include "InputManager.h"
#include "Projectile.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Collider.h"
#include "Animator.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "SceneManager.h"

Player::Player()
	: m_pTexture(nullptr),
	jumpPower(10.f),
	dashPower(1.f),
	playerCanDamaged(true),
	dashCooltime(1.f),
	shieldCooltime(5.f),
	dashTime(0.f),
	shieldTime(0.f),
	InvincibleTime(0.f),
	InvincibleHoldTime(0.5f),
	playerIsInvincibility(false),
	JumpDelayTime(0.02f),
	JumpTime(0.f),
	m_pendingSceneChange(false),
	m_delay(0.f)
{
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"jiwoo");
	AddComponent<Collider>();
	auto* animator = AddComponent<Animator>();
	animator->CreateAnimation
	(L"jiwooFront"
	,m_pTexture
	,{0.f,150.f}
	,{50.f,50.f}
	,{50.f,0.f}
	,5,0.1f);
	animator->Play(L"jiwooFront");
	AddComponent<Rigidbody>();
}

Player::~Player()
{
}

void Player::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();
	int width = m_pTexture->GetWidth();
	int height = m_pTexture->GetHeight();

	if (!playerCanDamaged)
		ELLIPSE_RENDER(_hdc, pos.x, pos.y, width / 3, height / 3);

	ComponentRender(_hdc);
}

void Player::EnterCollision(Collider* _other)
{
	if (_other->GetName() == L"Floor" || _other->GetName() == L"Platform")
	{
		Rigidbody* rb = GetComponent<Rigidbody>();
		rb->SetGrounded(true);
		JumpTime = 0.f;
	}

	if ((_other->GetName() == L"LaserLeft" || _other->GetName() == L"LaserRight" 
		|| _other->GetName() == L"BossProjectile")
		&& playerCanDamaged && !playerIsInvincibility)
	{
		m_delay = 0.2f;
		m_pendingSceneChange = true;
	}
	else if ((_other->GetName() == L"LaserLeft" || _other->GetName() == L"LaserRight" 
		|| _other->GetName() == L"BossProjectile")
		&& !playerCanDamaged || playerIsInvincibility)
	{
		playerCanDamaged = true;
	}
	else if (_other->GetName() == L"DeadFloor")
	{
		m_delay = 0.2f;
		m_pendingSceneChange = true;
	}
}

void Player::ExitCollision(Collider* _other)
{
	if (_other->GetName() == L"Floor" || _other->GetName() == L"Platform")
	{
		Rigidbody* rb = GetComponent<Rigidbody>();
		rb->SetGrounded(false);
		JumpTime = 0.f;
	}
}

void Player::Update()
{
	Vec2 dir = {};
	if (GET_KEY(KEY_TYPE::A)) dir.x -= (1.f * dashPower);
	
	if (GET_KEY(KEY_TYPE::D)) dir.x += (1.f * dashPower);

	if (GET_KEY(KEY_TYPE::SPACE)) PlayerJump();
	if (GET_KEY(KEY_TYPE::L) &&
		playerCanDamaged && shieldTime >= shieldCooltime) PlayerShield();
	if (GET_KEY(KEY_TYPE::K) && dashTime >= dashCooltime) PlayerDash();

	Rigidbody* rigid = GetComponent<Rigidbody>();

#pragma region 쿨타임 처리 부분
	if (playerCanDamaged && shieldTime < shieldCooltime)
	{
		shieldTime += fDT;
	}
	if (dashTime < dashCooltime)
	{
		dashTime += fDT;
	}
	if (JumpTime < JumpDelayTime && rigid->IsGrounded())
	{
		JumpTime += fDT;
	}
	if (dashPower > 1.f)
	{
		dashPower -= (fDT * ((5.f + dashPower) / 1.f));
	}
	if (InvincibleTime < InvincibleHoldTime)
	{
		InvincibleTime += fDT;
	}
	else
	{
		playerIsInvincibility = false;
	}

#pragma endregion

	Translate({ dir.x * 300.f * fDT, dir.y * 300.f * fDT });

	if (GET_KEYDOWN(KEY_TYPE::J))
		CreateProjectile();

	if (m_pendingSceneChange)
	{
		m_delay -= fDT;

		if (m_delay <= 0.f)
		{
			GET_SINGLE(SceneManager)->LoadScene(L"DeadScene");
			m_pendingSceneChange = false;
			SetDead();
		}
		return;
	}
}



void Player::CreateProjectile()
{
	Projectile* proj = new Projectile;
	// set
	Vec2 pos = GetPos();
	pos.y -= GetSize().y / 2.f;
	proj->SetPos(pos);
	proj->SetSize({ 20.f,20.f });
	static float angle = 0.f;
	proj->SetAngle(angle * PI / 180);
	angle += 10.f;
	proj->SetDir({ 0.f,-1.f });
	GET_SINGLE(SceneManager)->GetCurScene()
		->AddObject(proj, Layer::PROJECTILE);
	//scene->addobjet();
}

void Player::PlayerJump()
{
	Rigidbody* rigid = GetComponent<Rigidbody>();
	Vec2 jump = {0, -30};
	if (rigid->IsGrounded() && JumpTime > JumpDelayTime)
	{
		rigid->AddImpulse(jump * jumpPower);
		rigid->SetGrounded(false); 
	}
}

void Player::PlayerShield()
{
	playerCanDamaged = false;
	shieldTime = 0.f;
}

void Player::PlayerDash()
{
	dashPower = 5.f;
	playerIsInvincibility = true;
	dashTime = 0.f;
	InvincibleTime = 0.f;
}

