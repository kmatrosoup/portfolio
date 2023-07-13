#pragma once

#include "System.h"
#include "VEC2.h"

/* ジョイパッドのボタン */
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

/* 入力制御クラス */
class Input
{
	/* 公開関数 */
public:
	static void Init();		// 初期化
	static void Update();	// 更新

	/* 非公開関数 */
private:
	Input() = delete;
	~Input() = delete;
	::DISALLOW_COPY_CONSTRUCTOR(Input);
	::DISALLOW_MOVE_CONSTRUCTOR(Input);

	/* 公開クラス */
public:
	class Keyboard	// キーボード
	{
		/* 公開関数 */
	public:
		static bool In(int KeyCode);	// 押された瞬間
		static bool Keep(int KeyCode);	// 押されている間
		static bool Out(int KeyCode);	// 離された瞬間

		/* 非公開関数 */
	private:
		Keyboard() = delete;
		~Keyboard() = delete;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		Keyboard(Keyboard&&) = delete;
		Keyboard& operator=(Keyboard&&) = delete;
	};

	class Mouse	// マウス
	{
		/* 公開関数 */
	public:
		static bool In(int MouseCode);		// 押された瞬間
		static bool Keep(int MouseCode);	// 押されている間
		static bool Out(int MouseCode);		// 離された瞬間
		static VEC2F Pos();					// マウスの座標
		static VEC2F Pos_prev();			// マウスの座標（１フレーム前）
		static int Wheel();					// マウスホイール回転量

		/* 非公開関数 */
	private:
		Mouse() = delete;
		~Mouse() = delete;
		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;
		Mouse(Mouse&&) = delete;
		Mouse& operator=(Mouse&&) = delete;
	};

	class Joypad	// ジョイパッド
	{
		/* 公開関数 */
	public:
		static int ActiveNum();										// 接続数
		static bool In(int JoypadID, eJOYPAD_BUTTON ButtonID);		// 押された瞬間
		static bool Keep(int JoypadID, eJOYPAD_BUTTON ButtonID);	// 押されている間
		static bool Out(int JoypadID, eJOYPAD_BUTTON ButtonID);		// 離された瞬間

		/* 非公開関数 */
	private:
		Joypad() = delete;
		~Joypad() = delete;
		Joypad(const Joypad&) = delete;
		Joypad& operator=(const Joypad&) = delete;
		Joypad(Joypad&&) = delete;
		Joypad& operator=(Joypad&&) = delete;
	};

	/* 非公開変数 */
private:
	static bool m_Key[256];				// キー入力情報
	static bool m_Key_prev[256];		// キー入力情報・1フレーム前

	static int m_Mouse;					// マウス入力情報
	static int m_Mouse_prev;			// マウス入力情報・1フレーム前
	static VEC2F m_MousePos;			// マウス座標
	static VEC2F m_MousePos_prev;		// マウス座標・1フレーム前
	static int m_MouseWheelRotVol;		// マウスホイールの回転量

	static int m_JoypadNum;				// ジョイパッドの接続数
	static int m_JoypadState[16];		// ジョイパッド入力情報
	static int m_JoypadState_prev[16];	// ジョイパッド入力情報・1フレーム前
};
