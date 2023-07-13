// プリプロセス
#include "InputMgr.h"
#include "Input.h"
#include "DxLib.h"

// 変数の宣言
bool m_Input[5]{};
bool m_Input_prev[5]{};

// 更新
void InputMgr::Update()
{
	Input::Update();

	for (int i = 0; i < 5; ++i)
		m_Input_prev[i] = m_Input[i];

	m_Input[(int)eINPUT::DECIDE] = Input::Mouse::Keep(MOUSE_INPUT_LEFT);
	m_Input[(int)eINPUT::UP] = Input::Keyboard::Keep(KEY_INPUT_W);
	m_Input[(int)eINPUT::DOWN] = Input::Keyboard::Keep(KEY_INPUT_S);
	m_Input[(int)eINPUT::LEFT] = Input::Keyboard::Keep(KEY_INPUT_A);
	m_Input[(int)eINPUT::RIGHT] = Input::Keyboard::Keep(KEY_INPUT_D);

	for (int i = 0; i < Input::Joypad::ActiveNum(); ++i)
	{
		m_Input[(int)eINPUT::DECIDE] |= Input::Joypad::Keep(i, eJOYPAD_BUTTON::A);
		m_Input[(int)eINPUT::UP] |= Input::Joypad::Keep(i, eJOYPAD_BUTTON::STICK_UP);
		m_Input[(int)eINPUT::DOWN] |= Input::Joypad::Keep(i, eJOYPAD_BUTTON::STICK_DOWN);
		m_Input[(int)eINPUT::LEFT] |= Input::Joypad::Keep(i, eJOYPAD_BUTTON::STICK_LEFT);
		m_Input[(int)eINPUT::RIGHT] |= Input::Joypad::Keep(i, eJOYPAD_BUTTON::STICK_RIGHT);
	}
}

// 押された瞬間
bool InputMgr::In(eINPUT input)
{
	return (m_Input[(int)input] && !m_Input_prev[(int)input]);
}

// 押されている間
bool InputMgr::Keep(eINPUT input)
{
	return (m_Input[(int)input]);
}

// 離された瞬間
bool InputMgr::Out(eINPUT input)
{
	return (!m_Input[(int)input] && m_Input_prev[(int)input]);
}

