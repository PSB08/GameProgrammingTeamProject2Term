#pragma once
enum class KEY_TYPE
{
	LEFT, RIGHT, UP, DOWN,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	CTRL, LALT, LSHIFT, SPACE,
	ENTER, TAB, ESC, LBUTTON, RBUTTON,
	NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, LAST
	// LAST의미: 끝이라는 뜻이야
};

enum class KEY_STATE
{					// HOLD
	NONE, DOWN, UP, PRESS, END
};

struct KeyInfo
{
	KEY_STATE state;
	bool isPrevCheck;
};
class InputManager
{
	DECLARE_SINGLE(InputManager);
public:
	void Init();
	void Update();
private:
	void UpdateKeys();
	void UpdateMouse();
	void ResetAll();
public:
	const KEY_STATE& GetKey(KEY_TYPE _key) const
	{
		return m_vecKey[(int)_key].state;
	}
	bool IsDown(KEY_TYPE _key) const { return GetKey(_key) == KEY_STATE::DOWN; }
	bool IsUp(KEY_TYPE _key) const { return GetKey(_key) == KEY_STATE::UP; }
	bool IsPress(KEY_TYPE _key) const { return GetKey(_key) == KEY_STATE::PRESS; }
	bool IsPressWith(KEY_TYPE _mainKey, KEY_TYPE _modKey)
	{
		return IsPress(_mainKey) && IsPress(_modKey);
	}
	const POINT& GetMousePos() const { return m_mousePos; }
	const POINT& GetMouseDelta() const { return m_mouseDelta; }

private:
	vector<KeyInfo> m_vecKey;
	int m_vkKey[(int)KEY_TYPE::LAST] =
	{ VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		'Q','W','E','R',
	'T','Y','U','I','O','P',
	'A','S','D','F','G','H','J','K','L',
	'Z','X','C','V','B','N','M',
	VK_CONTROL, VK_LMENU, VK_LSHIFT, VK_SPACE,
	VK_RETURN, VK_TAB, VK_ESCAPE,
	VK_LBUTTON, VK_RBUTTON,'1','2','3','4','5'};

	POINT m_mousePos;
	POINT m_prevMousePos;
	POINT m_mouseDelta;


};
