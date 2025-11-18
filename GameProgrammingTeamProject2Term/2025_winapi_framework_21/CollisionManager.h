#pragma once
union COLLIDER_ID
{
	struct
	{
		UINT lo_ID;
		UINT hi_ID;
	};
	ULONGLONG ID;
};
class Collider;
class CollisionManager
{
	DECLARE_SINGLE(CollisionManager);
public:
	void Update();
	void CheckLayer(Layer _left, Layer _right);
	void CheckReset();
private:
	void CollisionLayerUpdate(Layer _left, Layer _right);
	bool IsCollision(Collider* _left, Collider* _right);
	ULONGLONG MakePairKey(UINT a, UINT b);
private:
	// 그룹 간의 충돌체크 배열
	UINT m_objectLayer[(UINT)Layer::END];
	std::unordered_map<ULONGLONG, bool> m_mapCollisionInfo;
};
