#pragma once

#include "VEC2.h"

#define Sw		(960)			// 画面の大きさ・横幅
#define Sh		(540)			// 画面の大きさ・縦幅
#define SSize	(VEC2(Sw, Sh))	// 画面の大きさ
#define SScale	(2)				// 画面スケール

#define FULL_SCREEN_MODE		(false)	// フルスクリーンモード
#define NEEDLESS_PPOINT_MODE	(false)	// 必要ポイント0モード

// コピーコンストラクタの禁止
#define DISALLOW_COPY_CONSTRUCTOR(c_name)\
	c_name(const c_name&) = delete;\
	c_name& operator=(const c_name&) = delete

// ムーブコンストラクタの禁止
#define DISALLOW_MOVE_CONSTRUCTOR(c_name)\
	c_name(c_name&&) = delete;\
	c_name& operator=(c_name&&) = delete
