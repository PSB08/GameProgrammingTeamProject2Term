#include "pch.h"
#include "Animation.h"
#include "Animator.h"
#include "Object.h"
#include "Texture.h"

Animation::Animation()
    : m_owner(nullptr)
    , m_tex(nullptr)
    , m_curFrame(0)
    , m_accTime(0.f)
    , m_mode{}
    , m_loopCount(1)
    , m_speed(1.f)
    , m_finished(false)
{

}

Animation::~Animation()
{

}

void Animation::Create(Texture* _tex, Vec2 _lt, Vec2 _sliceSize,
    Vec2 _step, UINT _frameCount, float _durationPerFrame)
{
    m_tex = _tex;
    if (!_tex || _frameCount <= 0 || _durationPerFrame <= 0.f)
        return;
    m_frames.clear();
    m_frames.reserve(_frameCount);
    for (UINT i = 0; i < _frameCount; ++i)
    {
        // 가독성
        tAnimFrame _fr;
        _fr.vLT = _lt + _step * i;
        _fr.vSlice = _sliceSize;
        _fr.fDuration = _durationPerFrame;
        _fr.vOffset = { 0.f, 0.f };
        m_frames.push_back(_fr);
    }
}

void Animation::ConfigurePlayback(PlayMode _mode, int _loopCount, float _speed)
{
    m_mode = _mode;
    m_loopCount = (_mode == PlayMode::Counted) ? std::max(1, _loopCount) : 1;
    m_speed = _speed;
    m_finished = false;
}

void Animation::ResetToFirstFrame()
{
    m_curFrame = 0;
    m_accTime = 0.f;
    m_finished = false;
}

void Animation::SetFrameOffset(int _index, Vec2 _offset)
{
    if (_index < 0 || _index >= (int)m_frames.size())
        return;
    m_frames[(size_t)_index].vOffset = _offset;
}



void Animation::Update()
{
    if (!m_owner || !m_tex || m_frames.empty() || m_finished)
        return;
    AdvanceFrame();
}

void Animation::AdvanceFrame()
{
    const tAnimFrame& _fr = m_frames[(size_t)m_curFrame];
    m_accTime += fDT * m_speed;

    if (m_accTime >= _fr.fDuration)
    {
        m_accTime -= _fr.fDuration;
        ++m_curFrame;

        if (m_curFrame >= (int)m_frames.size())
        {
            switch (m_mode)
            {
            case PlayMode::Once:
                m_curFrame = (int)m_frames.size() - 1;
                m_finished = true;
                break;
            case PlayMode::Loop:
                m_curFrame = 0;
                break;
            case PlayMode::Counted:
            {
                --m_loopCount;
                if (m_loopCount > 0)
                    m_curFrame = 0;
                else
                {
                    m_curFrame = (int)m_frames.size() - 1;
                    m_finished = true;
                }
            }
            break;
            }
        }
    }
}

void Animation::Render(HDC _hdc)
{
    if (!m_owner || !m_tex || m_frames.empty())
        return;

    Object* obj = m_owner->GetOwner();
    Vec2 pos = obj->GetPos();

    const tAnimFrame& fr = m_frames[(size_t)m_curFrame];
    pos = pos + fr.vOffset;

    // 목적지(화면) 사각형: 스프라이트 중심 정렬
    int dx = (int)(pos.x - fr.vSlice.x / 2);
    int dy = (int)(pos.y - fr.vSlice.y / 2);
    int dw = (int)fr.vSlice.x;
    int dh = (int)fr.vSlice.y;

    // 소스(시트) 사각형
    int sx = (int)fr.vLT.x;
    int sy = (int)fr.vLT.y;
    int sw = (int)fr.vSlice.x;
    int sh = (int)fr.vSlice.y;
    BOOL debug = TransparentBlt(_hdc,
        dx, dy, dw, dh,
        m_tex->GetTextureDC(),
        sx, sy, sw, sh,
        RGB(255, 0, 255));
}
