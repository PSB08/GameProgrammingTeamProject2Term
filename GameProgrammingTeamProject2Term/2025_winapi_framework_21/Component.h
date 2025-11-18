#pragma once
class Object;
class Component
{
public:
	Component();
	virtual ~Component();
public:
	virtual void Init() abstract;
	virtual void LateUpdate() abstract;
	virtual void Render(HDC _hdc) abstract;
public:
	void SetOwner(Object* _owner)
	{
		m_owner = _owner;
	}
	Object* GetOwner() const { return m_owner; }
private:
	Object* m_owner;
};

