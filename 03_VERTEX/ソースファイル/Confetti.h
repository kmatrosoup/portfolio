#pragma once

#include "BaseEffect.h"
#include "VEC2.h"

class cConfetti : public cBaseEffect
{

	/* 公開関数 */
public:
	cConfetti(VEC2F pos, unsigned int color, float angle, float motion);	// コンストラクタ
	~cConfetti() override;													// デストラクタ
	void Update() override;													// 更新
	void Draw_Default() const override;										// 描画・DEFAULT

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const int m_MaxTime;	// 最大生存時間

	VEC2F m_Pos;
	unsigned int m_Color;
	VEC2F m_Motion;
	int m_FrameTimer;
	float m_LightAngle;
};
