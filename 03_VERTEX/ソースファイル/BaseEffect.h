#pragma once

#include "VEC2.h"

class cBaseEffect
{
	/* 公開関数 */
public:
	cBaseEffect(int time);					// コンストラクタ
	virtual ~cBaseEffect();					// デストラクタ
	virtual void Update() = 0;				// 更新
	virtual void Draw_Default() const {};	// 描画・DEFAULT
	virtual void Draw_FadeAdd() const {};	// 描画・FADE_ADD

	virtual bool Active() const = 0;	// アクティブ状態

	/* 非公開変数 */
protected:
	int m_Timer;	// 生存時間
};
