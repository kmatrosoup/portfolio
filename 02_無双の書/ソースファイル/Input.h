#pragma once

#include "VEC2.h"

// ジョイパッドのボタンID
enum class eJOYPAD_BUTTON
{
	STICK_DOWN = 1,			// スティック・下
	STICK_LEFT = 2,			// スティック・左
	STICK_RIGHT = 4,		// スティック・右
	STICK_UP = 8,			// スティック・上
	A = 16,					// ボタン・A
	B = 32,					// ボタン・B
	X = 64,					// ボタン・X
	Y = 128,				// ボタン・Y
	L = 256,				// ボタン・L
	R = 512,				// ボタン・R
	PAUSE = 4096 | 8192,	// ボタン・ホーム
	PUSH = 16384 | 32768,	// スティック押し込み
};

namespace Input
{
	extern void Update();	// 更新

	// キーボード
	namespace Keyboard
	{
		extern bool In(int KeyCode);	// 押された瞬間
		extern bool Keep(int KeyCode);	// 押されている間
		extern bool Out(int KeyCode);	// 離された瞬間
	}

	// マウス
	namespace Mouse
	{
		extern bool In(int MouseCode);		// 押された瞬間
		extern bool Keep(int MouseCode);	// 押されている間
		extern bool Out(int MouseCode);		// 離された瞬間
		extern VEC2 Pos();					// マウスの座標
	}

	// ジョイパッド
	namespace Joypad
	{
		extern int ActiveNum();										// 接続数
		extern bool In(int JoypadID, eJOYPAD_BUTTON ButtonID);		// 押された瞬間
		extern bool Keep(int JoypadID, eJOYPAD_BUTTON ButtonID);	// 押されている間
		extern bool Out(int JoypadID, eJOYPAD_BUTTON ButtonID);		// 離された瞬間
	};
}
