#pragma once
class Object;
class UIObject;
class Player;

struct SpawnObject
{
	Object* obj;
	Layer type;
};
class Scene
{
public:
	Scene();
	virtual ~Scene(); // 가상 소멸자
public:
	virtual void Init() abstract;
	virtual void Update();
	virtual void LateUpdate();
	virtual void FixedUpdate(float _fixedDT);
	
	virtual void Render(HDC _hdc);
	virtual void Release();
public:
	void AddObject(Object* _obj, Layer _type)
	{
		m_vecObj[(UINT)_type].push_back(_obj);
	}
	const vector<Object*>& GetLayerObjects(Layer _type) const
	{
		return m_vecObj[(UINT)_type];
	}
	template<typename T>
	void Spawn(Layer _type, Vec2 _pos, Vec2 _size)
	{
		static_assert(std::is_base_of<Object, T>::value, "Object로부터 상속받아야 함");
		T* obj = new T;
		obj->SetPos(_pos);
		obj->SetSize(_size);
		AddObject(obj, _type);
	}
	void RequestDestroy(Object* obj);
	void RequestDestroyUI(UIObject* obj);
	void RequestSpawn(Object* obj, Layer _type);
	void FlushEvent();

	void SetPlayer(Player* player) { m_player = player; }
	Player* GetPlayer() const { return m_player; }

private:
	void RemoveObject(Object* _obj);
private:
	vector<Object*> m_vecObj[(UINT)Layer::END];
	vector<Object*> m_killObject;
	vector<SpawnObject> m_spawnObject;

	vector<UIObject*> m_vecUI;
	vector<UIObject*> m_killUI;

	Player* m_player = nullptr;
};

