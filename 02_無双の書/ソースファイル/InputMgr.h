#pragma once

enum class eINPUT
{
	DECIDE,	// 決定
	UP,		// 上入力
	DOWN,	// 下入力
	LEFT,	// 左入力
	RIGHT,	// 右入力
};

namespace InputMgr
{
	extern void Update();	// 更新

	extern bool In(eINPUT input);	// 押された瞬間
	extern bool Keep(eINPUT input);	// 押されている間
	extern bool Out(eINPUT input);	// 離された瞬間
}
