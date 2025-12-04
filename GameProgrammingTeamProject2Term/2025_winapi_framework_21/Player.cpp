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
#include "EventBus.h"

Player::Player()
	: m_pNormalTexture(nullptr),
	jumpPower(10.f),
	dashPower(1.f),
	playerCanDamaged(true),
	dashCooltime(1.f),
	shieldCooltime(5.f),
	dashTime(0.f),
	shieldTime(0.f),
	InvincibleTime(0.f),
	InvincibleHoldTime(0.15f),
	playerIsInvincibility(false),
	JumpDelayTime(0.02f),
	JumpTime(0.f),
	m_pendingSceneChange(false),
	m_delay(0.f),
	m_shootDelayTime(0.f)
{
	m_pNormalTexture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerMove");
	m_pShootingTexture = GET_SINGLE(ResourceManager)->GetTexture(L"Player_Back");
	//m_rigid->SetOwner(this);
	auto* col = AddComponent<Collider>();
	col->SetName(L"Player");
	col->SetSize({32.f,40.f});
	AddComponent<Animator>();
	m_animator = GetComponent<Animator>();
	m_animator->CreateAnimation
	(L"playerMove"
	,m_pNormalTexture
	,{0.f,0.f}
	,{64.f,64.f}
	,{64.f,0.f}
	,10,0.1f);

	m_animator->CreateAnimation
	(L"playerShoot"
		, m_pShootingTexture
		, { 0.f,0.f }
		, { 64.f,64.f }
		, { 64.f,0.f }
	, 1, 0.1f);

	m_animator->Play(L"playerMove");

	m_playerJumpListener = EventBus::AddListener(L"PlayerBounce", [this]()
		{
			PlayerBounce();
		});
	m_rigid = AddComponent<Rigidbody>();
}

Player::~Player()
{
	EventBus::RemoveListener(L"PlayerBounce", m_playerJumpListener);
	m_rigid->SetOwner(nullptr);
}

void Player::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();
	int width = m_pNormalTexture->GetWidth();
	int height = m_pNormalTexture->GetHeight();

	if (!playerCanDamaged)
		ELLIPSE_RENDER(_hdc, pos.x, pos.y, width / 10, height);

	ComponentRender(_hdc);
}

void Player::EnterCollision(Collider* _other)
{
	if (_other->GetName() == L"Floor" || _other->GetName() == L"Platform")
	{
		m_rigid->SetGrounded(true);
		JumpTime = 0.f;
	}

	if ((_other->GetName() == L"LaserLeft" || _other->GetName() == L"LaserRight" 
		|| _other->GetName() == L"BossProjectile" || _other->GetName() == L"BigBullet" || _other->GetName() == L"ExploseProjectile"
		|| _other->GetName() == L"Explosion")  && playerCanDamaged && !playerIsInvincibility)
	{
		m_delay = 0.2f;
		m_pendingSceneChange = true;
	}
	else if ((_other->GetName() == L"LaserLeft" || _other->GetName() == L"LaserRight" 
		|| _other->GetName() == L"BossProjectile" || _other->GetName() == L"BigBullet" || _other->GetName() == L"ExploseProjectile" ||
		_other->GetName() == L"Explosion")
		&& !playerCanDamaged || playerIsInvincibility)
	{
		if(playerIsInvincibility == false)
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
		m_rigid->SetGrounded(false);
		JumpTime = 0.f;
	}
}

void Player::Update()
{
	m_rigid->SetOwner(this);
	Vec2 dir = {};
	Animation* cur = m_animator->GetCurrent();
	if (GET_KEY(KEY_TYPE::A)) dir.x -= (1.f * dashPower);
	
	if (GET_KEY(KEY_TYPE::D)) dir.x += (1.f * dashPower);

	if (GET_KEY(KEY_TYPE::SPACE)) PlayerJump();
	if (GET_KEY(KEY_TYPE::L) &&
		playerCanDamaged && shieldTime >= shieldCooltime) PlayerShield();
	if (GET_KEY(KEY_TYPE::K) && dashTime >= dashCooltime
		&& m_dashCount < m_dashMaxCount) PlayerDash();
	if (!isShooting)
	{
		if (!isMoving)
		{
			m_animator->Play(L"playerMove");
			isMoving = true;
		}
	}
	else
	{
		m_animator->Play(L"playerShoot");
		m_shootDelayTime += fDT;
		if (m_shootDelayTime > 0.4f)
		{
			isShooting = false;
			isMoving = false;

		}
	}

#pragma region 쿨타임 처리 부분
	if (m_dashCount > 0)
	{
		m_dashRecoverTimer += fDT;
		if (m_dashCount <= 2)
		{
			if (m_dashRecoverTimer > 4.f)
				m_dashCount--;
		}
		else
		{
			if (m_dashRecoverTimer > 6.5f)
				m_dashCount--;
		}
	}

	if (playerCanDamaged && shieldTime < shieldCooltime)
	{
		shieldTime += fDT;
	}
	if (dashTime < dashCooltime)
	{
		dashTime += fDT;
	}
	if (JumpTime < JumpDelayTime && m_rigid->IsGrounded())
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
	m_playerPos = GetPos();
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
	isShooting = true;
	m_shootDelayTime = 0.f;
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
	m_rigid = GetComponent<Rigidbody>();
	Vec2 jump = {0, -40};
	if (m_rigid->IsGrounded() && JumpTime > JumpDelayTime)
	{
		m_rigid->AddImpulse(jump * jumpPower);
		m_rigid->SetGrounded(false); 
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
	m_dashCount++;
	m_dashRecoverTimer = 0.f;
}

void Player::PlayerBounce()
{
	auto* rid = GetComponent<Rigidbody>();
	if (rid == nullptr)
	{
		return;
	}
	if (rid->IsGrounded() && JumpTime > JumpDelayTime)
	{
		Vec2 jump = { 0, -95 };
		rid->AddImpulse(jump * jumpPower);
		rid->SetGrounded(false);
	}
	else if (!m_rigid->IsGrounded())
	{
		Vec2 jump = { 0, -55 };
		rid->AddImpulse(jump * jumpPower);
	}
}

