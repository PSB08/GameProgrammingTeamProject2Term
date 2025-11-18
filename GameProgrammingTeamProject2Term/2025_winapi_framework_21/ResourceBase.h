#pragma once
class ResourceBase
{
public:
    virtual ~ResourceBase() = default;
    // set get
    void        SetKey(const wstring& _key) { m_key = _key; }
    const wstring& GetKey() const { return m_key; }

    void        SetRelativePath(const wstring& _path) { m_relPath = _path; }
    const wstring& GetRelativePath() const { return m_relPath; }
private:
    wstring m_key;
    wstring m_relPath;

};

