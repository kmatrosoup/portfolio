//////////////////////
//   プリプロセス   //
//////////////////////
#include "InputMgr.h"
#include "Input.h"
#include "DxLib.h"

//////////////////
//   公開関数   //
//////////////////

/* 押した瞬間 */
bool cInputMgr::In(eINPUT_DEVICE InputDevice, eINPUT_BUTTON InputButton)
{
	// パッド
	if (InputDevice == eINPUT_DEVICE::ALL || ((int)InputDevice >= (int)eINPUT_DEVICE::PAD_1 && (int)InputDevice <= (int)eINPUT_DEVICE::PAD_16))
	{
		int s = (InputDevice == eINPUT_DEVICE::ALL ? (int)eINPUT_DEVICE::PAD_1 : (int)InputDevice);
		int e = (InputDevice == eINPUT_DEVICE::ALL ? (int)eINPUT_DEVICE::PAD_16 : (int)InputDevice);
		for (int i = s; i <= e; ++i) {
			switch (InputButton)
			{
			break;	case eINPUT_BUTTON::STICK_DOWN:		if (Input::Joypad::In(i, eJOYPAD_BUTTON::STICK_DOWN))		return true;
			break;	case eINPUT_BUTTON::STICK_LEFT:		if (Input::Joypad::In(i, eJOYPAD_BUTTON::STICK_LEFT))		return true;
			break;	case eINPUT_BUTTON::STICK_RIGHT:	if (Input::Joypad::In(i, eJOYPAD_BUTTON::STICK_RIGHT))	return true;
			break;	case eINPUT_BUTTON::STICK_UP:		if (Input::Joypad::In(i, eJOYPAD_BUTTON::STICK_UP))		return true;
			break;	case eINPUT_BUTTON::BUTTON_1:		if (Input::Joypad::In(i, eJOYPAD_BUTTON::B))		return true;
			break;	case eINPUT_BUTTON::BUTTON_2:		if (Input::Joypad::In(i, eJOYPAD_BUTTON::A))		return true;
			break;	case eINPUT_BUTTON::BUTTON_L:		if (Input::Joypad::In(i, eJOYPAD_BUTTON::L))		return true;
			break;	case eINPUT_BUTTON::BUTTON_R:		if (Input::Joypad::In(i, eJOYPAD_BUTTON::R))		return true;
			break;	case eINPUT_BUTTON::BUTTON_PAUSE:	if (Input::Joypad::In(i, eJOYPAD_BUTTON::PAUSE))	return true;
			}
		}
	}

	// キーボード
	if (InputDevice == eINPUT_DEVICE::ALL || InputDevice == eINPUT_DEVICE::KEY_MOUSE)
	{
		switch (InputButton)
		{
		break;	case eINPUT_BUTTON::STICK_DOWN:		if (Input::Keyboard::In(KEY_INPUT_S))		return true;
		break;	case eINPUT_BUTTON::STICK_LEFT:		if (Input::Keyboard::In(KEY_INPUT_A))		return true;
		break;	case eINPUT_BUTTON::STICK_RIGHT:	if (Input::Keyboard::In(KEY_INPUT_D))		return true;
		break;	case eINPUT_BUTTON::STICK_UP:		if (Input::Keyboard::In(KEY_INPUT_W))		return true;
		break;	case eINPUT_BUTTON::BUTTON_1:		if (Input::Mouse::In(MOUSE_INPUT_RIGHT))	return true;
		break;	case eINPUT_BUTTON::BUTTON_2:		if (Input::Mouse::In(MOUSE_INPUT_LEFT))		return true;
		break;	case eINPUT_BUTTON::BUTTON_L:		if (Input::Keyboard::In(KEY_INPUT_LSHIFT))	return true;
		break;	case eINPUT_BUTTON::BUTTON_R:		if (Input::Keyboard::In(KEY_INPUT_SPACE))	return true;
		break;	case eINPUT_BUTTON::BUTTON_PAUSE:	if (Input::Keyboard::In(KEY_INPUT_ESCAPE))	return true;
		}
	}

	return false;
}

/* 押している間 */
bool cInputMgr::Keep(eINPUT_DEVICE InputDevice, eINPUT_BUTTON InputButton)
{
	// パッド
	if (InputDevice == eINPUT_DEVICE::ALL || ((int)InputDevice >= (int)eINPUT_DEVICE::PAD_1 && (int)InputDevice <= (int)eINPUT_DEVICE::PAD_16))
	{
		int s = (InputDevice == eINPUT_DEVICE::ALL ? (int)eINPUT_DEVICE::PAD_1 : (int)InputDevice);
		int e = (InputDevice == eINPUT_DEVICE::ALL ? (int)eINPUT_DEVICE::PAD_16 : (int)InputDevice);
		for (int i = s; i <= e; ++i) {
			switch (InputButton)
			{
			break;	case eINPUT_BUTTON::STICK_DOWN:		if (Input::Joypad::Keep(i, eJOYPAD_BUTTON::STICK_DOWN))	return true;
			break;	case eINPUT_BUTTON::STICK_LEFT:		if (Input::Joypad::Keep(i, eJOYPAD_BUTTON::STICK_LEFT))	return true;
			break;	case eINPUT_BUTTON::STICK_RIGHT:	if (Input::Joypad::Keep(i, eJOYPAD_BUTTON::STICK_RIGHT))	return true;
			break;	case eINPUT_BUTTON::STICK_UP:		if (Input::Joypad::Keep(i, eJOYPAD_BUTTON::STICK_UP))		return true;
			break;	case eINPUT_BUTTON::BUTTON_1:		if (Input::Joypad::Keep(i, eJOYPAD_BUTTON::B))		return true;
			break;	case eINPUT_BUTTON::BUTTON_2:		if (Input::Joypad::Keep(i, eJOYPAD_BUTTON::A))		return true;
			break;	case eINPUT_BUTTON::BUTTON_L:		if (Input::Joypad::Keep(i, eJOYPAD_BUTTON::L))		return true;
			break;	case eINPUT_BUTTON::BUTTON_R:		if (Input::Joypad::Keep(i, eJOYPAD_BUTTON::R))		return true;
			break;	case eINPUT_BUTTON::BUTTON_PAUSE:	if (Input::Joypad::Keep(i, eJOYPAD_BUTTON::PAUSE))	return true;
			}
		}
	}

	// キーボード
	if (InputDevice == eINPUT_DEVICE::ALL || InputDevice == eINPUT_DEVICE::KEY_MOUSE)
	{
		switch (InputButton)
		{
		break;	case eINPUT_BUTTON::STICK_DOWN:		if (Input::Keyboard::Keep(KEY_INPUT_S))			return true;
		break;	case eINPUT_BUTTON::STICK_LEFT:		if (Input::Keyboard::Keep(KEY_INPUT_A))			return true;
		break;	case eINPUT_BUTTON::STICK_RIGHT:	if (Input::Keyboard::Keep(KEY_INPUT_D))			return true;
		break;	case eINPUT_BUTTON::STICK_UP:		if (Input::Keyboard::Keep(KEY_INPUT_W))			return true;
		break;	case eINPUT_BUTTON::BUTTON_1:		if (Input::Mouse::Keep(MOUSE_INPUT_RIGHT))		return true;
		break;	case eINPUT_BUTTON::BUTTON_2:		if (Input::Mouse::Keep(MOUSE_INPUT_LEFT))		return true;
		break;	case eINPUT_BUTTON::BUTTON_L:		if (Input::Keyboard::Keep(KEY_INPUT_LSHIFT))	return true;
		break;	case eINPUT_BUTTON::BUTTON_R:		if (Input::Keyboard::Keep(KEY_INPUT_SPACE))		return true;
		break;	case eINPUT_BUTTON::BUTTON_PAUSE:	if (Input::Keyboard::Keep(KEY_INPUT_ESCAPE))	return true;
		}
	}

	return false;
}

/* 離した瞬間 */
bool cInputMgr::Out(eINPUT_DEVICE InputDevice, eINPUT_BUTTON InputButton)
{
	// パッド
	if (InputDevice == eINPUT_DEVICE::ALL || ((int)InputDevice >= (int)eINPUT_DEVICE::PAD_1 && (int)InputDevice <= (int)eINPUT_DEVICE::PAD_16))
	{
		int s = (InputDevice == eINPUT_DEVICE::ALL ? (int)eINPUT_DEVICE::PAD_1 : (int)InputDevice);
		int e = (InputDevice == eINPUT_DEVICE::ALL ? (int)eINPUT_DEVICE::PAD_16 : (int)InputDevice);
		for (int i = s; i <= e; ++i) {
			switch (InputButton)
			{
			break;	case eINPUT_BUTTON::STICK_DOWN:		if (Input::Joypad::Out(i, eJOYPAD_BUTTON::STICK_DOWN))	return true;
			break;	case eINPUT_BUTTON::STICK_LEFT:		if (Input::Joypad::Out(i, eJOYPAD_BUTTON::STICK_LEFT))	return true;
			break;	case eINPUT_BUTTON::STICK_RIGHT:	if (Input::Joypad::Out(i, eJOYPAD_BUTTON::STICK_RIGHT))	return true;
			break;	case eINPUT_BUTTON::STICK_UP:		if (Input::Joypad::Out(i, eJOYPAD_BUTTON::STICK_UP))		return true;
			break;	case eINPUT_BUTTON::BUTTON_1:		if (Input::Joypad::Out(i, eJOYPAD_BUTTON::B))		return true;
			break;	case eINPUT_BUTTON::BUTTON_2:		if (Input::Joypad::Out(i, eJOYPAD_BUTTON::A))		return true;
			break;	case eINPUT_BUTTON::BUTTON_L:		if (Input::Joypad::Out(i, eJOYPAD_BUTTON::L))		return true;
			break;	case eINPUT_BUTTON::BUTTON_R:		if (Input::Joypad::Out(i, eJOYPAD_BUTTON::R))		return true;
			break;	case eINPUT_BUTTON::BUTTON_PAUSE:	if (Input::Joypad::Out(i, eJOYPAD_BUTTON::PAUSE))	return true;
			}
		}
	}

	// キーボード
	if (InputDevice == eINPUT_DEVICE::ALL || InputDevice == eINPUT_DEVICE::KEY_MOUSE)
	{
		switch (InputButton)
		{
		break;	case eINPUT_BUTTON::STICK_DOWN:		if (Input::Keyboard::Out(KEY_INPUT_S))		return true;
		break;	case eINPUT_BUTTON::STICK_LEFT:		if (Input::Keyboard::Out(KEY_INPUT_A))		return true;
		break;	case eINPUT_BUTTON::STICK_RIGHT:	if (Input::Keyboard::Out(KEY_INPUT_D))		return true;
		break;	case eINPUT_BUTTON::STICK_UP:		if (Input::Keyboard::Out(KEY_INPUT_W))		return true;
		break;	case eINPUT_BUTTON::BUTTON_1:		if (Input::Mouse::Out(MOUSE_INPUT_RIGHT))	return true;
		break;	case eINPUT_BUTTON::BUTTON_2:		if (Input::Mouse::Out(MOUSE_INPUT_LEFT))	return true;
		break;	case eINPUT_BUTTON::BUTTON_L:		if (Input::Keyboard::Out(KEY_INPUT_LSHIFT))	return true;
		break;	case eINPUT_BUTTON::BUTTON_R:		if (Input::Keyboard::Out(KEY_INPUT_SPACE))	return true;
		break;	case eINPUT_BUTTON::BUTTON_PAUSE:	if (Input::Keyboard::Out(KEY_INPUT_ESCAPE))	return true;
		}
	}

	return false;
}
