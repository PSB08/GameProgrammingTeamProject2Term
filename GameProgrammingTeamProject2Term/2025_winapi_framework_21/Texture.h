#pragma once
#include "ResourceBase.h"
class Texture :
    public ResourceBase
{
public:
    Texture();
    ~Texture();
public:
    LONG GetWidth() const { return m_bitInfo.bmWidth; }
    LONG GetHeight() const { return m_bitInfo.bmHeight; }
    const HDC& GetTextureDC() const { return m_hDC; }
    void Load(const wstring& _path);
private:
    HBITMAP m_hBit;
    HDC     m_hDC;
    BITMAP  m_bitInfo;
};

