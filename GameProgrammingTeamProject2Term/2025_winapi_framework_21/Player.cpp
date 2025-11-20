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

Player::Player()
	: m_pTexture(nullptr),
	jumpPower(10.f),
	dashPower(80.f),
	playerCanDamaged(true),
	dashCooltime(1.f),
	shieldCooltime(5.f),
	dashTime(0.f),
	shieldTime(0.f),
	InvincibleTime(0.f),
	InvincibleHoldTime(0.5f),
	playerIsInvincibility(false)
{
	//m_pTexture = new Texture;
	//wstring path = GET_SINGLE(ResourceManager)->GetResPath();
	//path += L"Texture\\plane.bmp";
	//m_pTexture->Load(path);
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
	//SAFE_DELETE(m_pTexture);
}

void Player::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = GetSize();
	int width = m_pTexture->GetWidth();
	int height = m_pTexture->GetHeight();

	if (!playerCanDamaged)
		ELLIPSE_RENDER(_hdc, pos.x, pos.y, width / 3, height / 3);

	// blt 종류
	// 1. bitblt - 1:1 매칭, 가장 빠름
	//::BitBlt(_hdc
	//	, (int)(pos.x - size.x / 2)
	//	, (int)(pos.y - size.y / 2)
	//	, width, height,
	//	m_pTexture->GetTextureDC(),
	//	0, 0, SRCCOPY);
	
	////// 2. TransparentBlt - 색상 빼기, 느림 
	//::TransparentBlt(_hdc
	//	, (int)(pos.x - size.x / 2)
	//	, (int)(pos.y - size.y / 2)
	//	, width * 2, height,
	//	m_pTexture->GetTextureDC(),
	//	0, 0, width, height, RGB(255,0,255));
	
	////// 3. StretchBlt - 크기조절, 반전, 빠름
	//::StretchBlt(_hdc
	//	, (int)(pos.x - size.x / 2)
	//	, (int)(pos.y - size.y / 2)
	//	, size.x, size.y,
	//	m_pTexture->GetTextureDC(),
	//	0, 0, width, height, SRCCOPY);

	// 4. 회전 
	//::PlgBlt()

	// 5. alpha 값 조절
	//::AlphaBlend()
	ComponentRender(_hdc);
}

void Player::EnterCollision(Collider* _other)
{
	if (_other->GetName() == L"Floor")
	{
		cout << "땅 닿음";
		Rigidbody* rb = GetComponent<Rigidbody>();
		rb->SetGrounded(true);
	}

	if ((_other->GetName() == L"LaserLeft" || _other->GetName() == L"LaserRight" || _other->GetName() == L"BossProjectile")
		&& playerCanDamaged && !playerIsInvincibility)
	{
		cout << "님쥬금";
	}
	else if ((_other->GetName() == L"LaserLeft" || _other->GetName() == L"LaserRight" || _other->GetName() == L"BossProjectile")
		&& !playerCanDamaged || playerIsInvincibility)
	{
		playerCanDamaged = true;
	}
}

void Player::ExitCollision(Collider* _other)
{
}

void Player::Update()
{
	//Vec2 pos = GetPos();
	//if (GET_KEY(KEY_TYPE::A))
	//	pos.x -= 300.f * fDT;
	//if (GET_KEY(KEY_TYPE::D))
	//	pos.x += 300.f * fDT;
	//if (GET_KEY(KEY_TYPE::W))
	//	pos.y -= 300.f * fDT;
	//if (GET_KEY(KEY_TYPE::S))
	//	pos.y += 300.f * fDT;
	//SetPos(pos);
	Vec2 dir = {};
	if (GET_KEY(KEY_TYPE::A)) dir.x -= 1.f;
	
	if (GET_KEY(KEY_TYPE::D)) dir.x += 1.f;

	if (GET_KEY(KEY_TYPE::SPACE)) PlayerJump();
	if (GET_KEY(KEY_TYPE::L) &&
		playerCanDamaged && shieldTime >= shieldCooltime) PlayerShield();
	if (GET_KEY(KEY_TYPE::K) && dashTime >= dashCooltime) PlayerDash();

	if (playerCanDamaged && shieldTime < shieldCooltime)
	{
		shieldTime += fDT;
	}
	if (dashTime < dashCooltime)
	{
		dashTime += fDT;
	}
	if (InvincibleTime < InvincibleHoldTime)
	{
		InvincibleTime += fDT;
	}
	else
	{
		playerIsInvincibility = false;
	}

	Translate({ dir.x * 300.f * fDT, dir.y * 300.f * fDT });

	if (GET_KEYDOWN(KEY_TYPE::J))
		CreateProjectile();
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
	if (rigid->IsGrounded())
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
	Rigidbody* rigid = GetComponent<Rigidbody>();
	Vec2 dash = { 100, 0 };

	if (GET_KEY(KEY_TYPE::A)) rigid->AddImpulse(dash * -dashPower);

	if (GET_KEY(KEY_TYPE::D)) rigid->AddImpulse(dash * dashPower);


	cout << "Dash";
	playerIsInvincibility = true;
	dashTime = 0.f;
	InvincibleTime = 0.f;
}

// DevScene에서 Enter를 누르면 TestScene으로 넘어갑니다.
// TestScene에는 Enemy가 사각형으로 랜덤 위치에 랜덤색깔로 깜빡입니다.
