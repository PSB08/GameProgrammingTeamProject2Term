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
	InvincibleHoldTime(0.4f),
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

	// blt Á¾·ù
	// 1. bitblt - 1:1 ¸ÅÄª, °¡Àå ºü¸§
	//::BitBlt(_hdc
	//	, (int)(pos.x - size.x / 2)
	//	, (int)(pos.y - size.y / 2)
	//	, width, height,
	//	m_pTexture->GetTextureDC(),
	//	0, 0, SRCCOPY);
	
	////// 2. TransparentBlt - »ö»ó »©±â, ´À¸² 
	//::TransparentBlt(_hdc
	//	, (int)(pos.x - size.x / 2)
	//	, (int)(pos.y - size.y / 2)
	//	, width * 2, height,
	//	m_pTexture->GetTextureDC(),
	//	0, 0, width, height, RGB(255,0,255));
	
	////// 3. StretchBlt - Å©±âÁ¶Àý, ¹ÝÀü, ºü¸§
	//::StretchBlt(_hdc
	//	, (int)(pos.x - size.x / 2)
	//	, (int)(pos.y - size.y / 2)
	//	, size.x, size.y,
	//	m_pTexture->GetTextureDC(),
	//	0, 0, width, height, SRCCOPY);

	// 4. È¸Àü 
	//::PlgBlt()

	// 5. alpha °ª Á¶Àý
	//::AlphaBlend()
	ComponentRender(_hdc);
}

void Player::EnterCollision(Collider* _other)
{
	if (_other->GetName() == L"Floor")
	{
		cout << "¶¥ ´êÀ½";
		Rigidbody* rb = GetComponent<Rigidbody>();
		rb->SetGrounded(true);
	}

	if (_other->GetName() == L"Bullet" && playerCanDamaged)
	{
		cout << "´ÔÁê±Ý";
	}
	else if (_other->GetName() == L"Bullet" && !playerCanDamaged)
	{
		cout << "¹æ¾î ±úÁü";
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
	if (GET_KEY(KEY_TYPE::LEFT)) dir.x -= 1.f;
	
	if (GET_KEY(KEY_TYPE::RIGHT)) dir.x += 1.f;

	if (GET_KEY(KEY_TYPE::SPACE)) PlayerJump();
	if (GET_KEY(KEY_TYPE::Z) &&
		playerCanDamaged && shieldTime < shieldCooltime) PlayerShield();
	if (GET_KEY(KEY_TYPE::X) && dashTime >= dashCooltime) PlayerDash();

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

	if (GET_KEYDOWN(KEY_TYPE::CTRL))
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
	Vec2 dash = { 10, 0 };

	if (GET_KEY(KEY_TYPE::LEFT)) rigid->AddImpulse(dash * -dashPower);

	if (GET_KEY(KEY_TYPE::RIGHT)) rigid->AddImpulse(dash * dashPower);


	cout << "Dash";
	playerIsInvincibility = true;
	dashTime = 0.f;
	InvincibleTime = 0.f;
}

// DevScene¿¡¼­ Enter¸¦ ´©¸£¸é TestSceneÀ¸·Î ³Ñ¾î°©´Ï´Ù.
// TestScene¿¡´Â Enemy°¡ »ç°¢ÇüÀ¸·Î ·£´ý À§Ä¡¿¡ ·£´ý»ö±ò·Î ±ôºýÀÔ´Ï´Ù.
