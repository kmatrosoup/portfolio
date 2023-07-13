#pragma once

#include "System.h"

/* 入力媒体 */
enum class eINPUT_DEVICE
{
	PAD_1,		// パッドID・1
	PAD_2,		// パッドID・2
	PAD_3,		// パッドID・3
	PAD_4,		// パッドID・4
	PAD_5,		// パッドID・5
	PAD_6,		// パッドID・6
	PAD_7,		// パッドID・7
	PAD_8,		// パッドID・8
	PAD_9,		// パッドID・9
	PAD_10,		// パッドID・10
	PAD_11,		// パッドID・11
	PAD_12,		// パッドID・12
	PAD_13,		// パッドID・13
	PAD_14,		// パッドID・14
	PAD_15,		// パッドID・15
	PAD_16,		// パッドID・16
	KEY_MOUSE,	// キーボード＆マウス

	ALL
};

/* 入力ボタン */
enum class eINPUT_BUTTON
{
	STICK_DOWN,		// 方向・下
	STICK_LEFT,		// 方向・左
	STICK_RIGHT,	// 方向・右
	STICK_UP,		// 方向・上
	BUTTON_1,		// ボタン・決定
	BUTTON_2,		// ボタン・キャンセル
	BUTTON_L,		// ボタン・L
	BUTTON_R,		// ボタン・R
	BUTTON_PAUSE,	// ボタン・ポーズ
};

/* 入力管理クラス */
class cInputMgr
{
	/* 公開関数 */
public:
	static bool In(eINPUT_DEVICE InputDevice, eINPUT_BUTTON InputButton);	// 押した瞬間
	static bool Keep(eINPUT_DEVICE InputDevice, eINPUT_BUTTON InputButton);	// 押している間
	static bool Out(eINPUT_DEVICE InputDevice, eINPUT_BUTTON InputButton);	// 離した瞬間

	/* 非公開関数 */
private:
	cInputMgr() = delete;
	~cInputMgr() = delete;
	::DISALLOW_COPY_CONSTRUCTOR(cInputMgr);
	::DISALLOW_MOVE_CONSTRUCTOR(cInputMgr);
};
