// プリプロセス
#include "Input.h"
#include "DxLib.h"

// 変数の宣言
bool	m_Key[256]{};				// キー入力情報
bool	m_Key_prev[256]{};			// キー入力情報・1フレーム前

int		m_Mouse(0);					// マウス入力情報
int		m_Mouse_prev(0);			// マウス入力情報・1フレーム前

int		m_JoypadNum(0);				// ジョイパッドの接続数
int		m_JoypadState[16]{};		// ジョイパッド入力情報
int		m_JoypadState_prev[16]{};	// ジョイパッド入力情報・1フレーム前

// 更新
void Input::Update()
{
	// キーボード
	for (int i = 0; i < 256; i++)
		m_Key_prev[i] = m_Key[i];
	{
		char tmpKey[256];
		GetHitKeyStateAll(tmpKey);
		for (int i = 0; i < 256; i++)
			m_Key[i] = (tmpKey[i] != 0);
	}

	// マウス
	m_Mouse_prev = m_Mouse;
	m_Mouse = GetMouseInput();

	// ジョイパッド
	m_JoypadNum = GetJoypadNum();
	for (int i = 0; i < m_JoypadNum; ++i) {
		m_JoypadState_prev[i] = m_JoypadState[i];
		m_JoypadState[i] = GetJoypadInputState(i + 1);
	}
}

////////////////////////
//   <Keyboard>関数   //
////////////////////////

// 押された瞬間
bool Input::Keyboard::In(int KeyCode)
{
	if (KeyCode < 0 || KeyCode >= 256)
		return false;

	return (m_Key[KeyCode] && !m_Key_prev[KeyCode]);
}

// 押されている間
bool Input::Keyboard::Keep(int KeyCode)
{
	if (KeyCode < 0 || KeyCode >= 256)
		return false;

	return (m_Key[KeyCode]);
}

// 離された瞬間
bool Input::Keyboard::Out(int KeyCode)
{
	if (KeyCode < 0 || KeyCode >= 256)
		return false;

	return (!m_Key[KeyCode] && m_Key_prev[KeyCode]);
}

/////////////////////
//   <Mouse>関数   //
/////////////////////

// 押された瞬間
bool Input::Mouse::In(int MouseCode)
{
	return (((m_Mouse & MouseCode) != 0) && !((m_Mouse_prev & MouseCode) != 0));
}

// 押されている間
bool Input::Mouse::Keep(int MouseCode)
{
	return ((m_Mouse & MouseCode) != 0);
}

// 離された瞬間
bool Input::Mouse::Out(int MouseCode)
{
	return (!((m_Mouse & MouseCode) != 0) && ((m_Mouse_prev & MouseCode) != 0));
}

// 座標
VEC2 Input::Mouse::Pos()
{
	int mx, my;
	GetMousePoint(&mx, &my);
	return VEC2(mx, my);
}

//////////////////////
//   <Joypad>関数   //
//////////////////////

// 接続数
int Input::Joypad::ActiveNum()
{
	return m_JoypadNum;
}

// 押された瞬間
bool Input::Joypad::In(int JoypadID, eJOYPAD_BUTTON ButtonID)
{
	if (JoypadID < 0 || JoypadID >= m_JoypadNum || JoypadID >= 16)
		return false;

	return (((m_JoypadState[JoypadID] & (int)ButtonID) != 0) && ((m_JoypadState_prev[JoypadID] & (int)ButtonID) == 0));
}

// 押されている間
bool Input::Joypad::Keep(int JoypadID, eJOYPAD_BUTTON ButtonID)
{
	if (JoypadID < 0 || JoypadID >= m_JoypadNum || JoypadID >= 16)
		return false;

	return ((m_JoypadState[JoypadID] & (int)ButtonID) != 0);
}

// 離された瞬間
bool Input::Joypad::Out(int JoypadID, eJOYPAD_BUTTON ButtonID)
{
	if (JoypadID < 0 || JoypadID >= m_JoypadNum || JoypadID >= 16)
		return false;

	return (((m_JoypadState[JoypadID] & (int)ButtonID) == 0) && ((m_JoypadState_prev[JoypadID] & (int)ButtonID) != 0));
}
