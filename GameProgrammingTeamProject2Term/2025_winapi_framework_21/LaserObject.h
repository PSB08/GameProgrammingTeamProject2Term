#pragma once
#include "pch.h"
#include "Object.h"
#include "Texture.h"
#include "Animator.h"

class Collider;

class LaserObject : public Object
{
public:
    LaserObject(bool isLeftLaser);
    virtual ~LaserObject();

public:
    virtual void Update() override;
    virtual void Render(HDC _hdc) override;
    virtual void EnterCollision(Collider* _other) override;

    void SetSpeed(float s) { m_speed = s; }
    void SetDir(int d) { m_dir = d; } // -1 왼쪽 -> 안쪽 / 1 오른쪽 -> 안쪽

private:
    void SetupAnimations();

private:
    float m_speed = 400.f;
    int   m_dir = -1;
    bool  m_isLeft;

    float m_length = 20.f;

    Texture* m_pTexture = nullptr;
    Animator* m_animator = nullptr;
};
