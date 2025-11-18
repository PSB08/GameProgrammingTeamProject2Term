#pragma once
#include "Component.h"
class Texture;
class Animation;
class Animator : public Component
{
public:
    Animator();
    ~Animator();

public:
    // Component을(를) 통해 상속됨
    void Init() override;
    void LateUpdate() override;
    void Render(HDC _hdc) override;

public:
    // 애니메이션 생성(스프라이트 시트에서 일정 간격으로 자르기)
    void CreateAnimation(const wstring& _name, Texture* _tex,
        Vec2 _lt, Vec2 _sliceSize, Vec2 _step,
        UINT _frameCount, float _durationPerFrame);

    // 찾기
    Animation* FindAnimation(const wstring& _name);

    // speed=1.0f(기본), PlayMode는 loop 기본, loopCount는 Counted에서 사용
    void Play(const wstring& _name, PlayMode _mode = PlayMode::Loop,
        int _loopCount = 1, float _speed = 1.0f);

    void Stop();
    void Pause();
    void Resume();

public:
    Animation* GetCurrent() const { return m_curAnimation; }
    bool IsPaused() const { return m_paused; }

private:
    std::unordered_map<std::wstring, Animation*> m_mapAnim; // 소유권
    Animation* m_curAnimation; // 선택만
    bool       m_paused;


};
