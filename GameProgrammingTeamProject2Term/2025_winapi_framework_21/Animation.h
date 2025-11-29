#pragma once

#include <functional>

class Animator;
class Texture;
struct tAnimFrame
{
    Vec2  vLT;        // 시트 상 좌상단
    Vec2  vSlice;     // 잘라낼 크기(폭, 높이)
    float fDuration;  // 해당 프레임 유지시간(초)
    Vec2  vOffset;    // 원점 보정(선택)
};

class Animation
{
public:
    Animation();
    ~Animation();
public:
    void Update();
    void Render(HDC _hdc);

    // 시트에서 등간격으로 프레임 생성
    void Create(Texture* _tex, Vec2 _lt, Vec2 _sliceSize,
        Vec2 _step, UINT _frameCount, float _durationPerFrame);

    // 재생 파라미터 설정(Animator::Play에서 호출)
    void ConfigurePlayback(PlayMode _mode, int _loopCount, float _speed);
    // flip 필요시 추후 추가

    // 첫 프레임/누적시간 초기화
    void ResetToFirstFrame();

public:
    void SetOwner(Animator* _owner) { m_owner = _owner; }
    void SetName(const wstring& _name) { m_name = _name; }
    const wstring& GetName() const { return m_name; }

    // 필요시 사용
    void SetFrameOffset(int _index, Vec2 _offset);
    int  GetCurFrame() const { return m_curFrame; }
    int  GetMaxFrame() const { return (int)m_frames.size(); }
    bool IsFinished()  const { return m_finished; }

    void SetFinishCallback(const std::function<void()>& func);

private:
    void AdvanceFrame();

private:
    Animator* m_owner;
    Texture* m_tex;
    std::wstring            m_name;
    std::vector<tAnimFrame> m_frames;

    // 재생 상태
    int      m_curFrame;
    float    m_accTime;
    PlayMode m_mode;
    int      m_loopCount;
    float    m_speed;
    bool     m_finished;

    std::function<void()> m_onFinish;
};
