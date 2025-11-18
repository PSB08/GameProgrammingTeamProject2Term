#include "pch.h"
#include "Animator.h"
#include "Animation.h"
#include "Texture.h"

Animator::Animator()
    : m_curAnimation(nullptr)
    , m_paused(false)
{

}
Animator::~Animator()
{
    for (auto& e : m_mapAnim)
        SAFE_DELETE(e.second);
    m_mapAnim.clear();
}

void Animator::Init()
{

}

void Animator::LateUpdate()
{
    if (m_paused || !m_curAnimation)
        return;
    m_curAnimation->Update();
}

void Animator::Render(HDC hdc)
{
    if (!m_curAnimation)
        return;
    m_curAnimation->Render(hdc);
}

void Animator::CreateAnimation(const wstring& _name, Texture* _tex,
    Vec2 _lt, Vec2 _sliceSize, Vec2 _step,
    UINT _frameCount, float _durationPerFrame)
{
    Animation* pAnim = FindAnimation(_name);
    if (pAnim != nullptr)
        return;

    pAnim = new Animation;
    pAnim->SetName(_name);
    pAnim->SetOwner(this);
    pAnim->Create(_tex, _lt, _sliceSize, _step, _frameCount, _durationPerFrame);
    m_mapAnim.insert({ _name,pAnim });
}

Animation* Animator::FindAnimation(const std::wstring& _name)
{
    std::unordered_map<wstring, Animation*>::iterator iter = m_mapAnim.find(_name);
    if (iter == m_mapAnim.end())
        return nullptr;
    return iter->second;
}

void Animator::Play(const std::wstring& _name, PlayMode _mode, int _loopCount,
    float _speed)
{
    m_curAnimation = FindAnimation(_name);
    m_paused = false;

    // Animation이 재생 상태를 모두 관리
    m_curAnimation->ConfigurePlayback(_mode, _loopCount, _speed);
    m_curAnimation->ResetToFirstFrame();
}

void Animator::Stop()
{
    m_curAnimation = nullptr;
    m_paused = false;
}

void Animator::Pause()
{
    m_paused = true;
}
void Animator::Resume()
{
    m_paused = false;
}

