#pragma once

#include "BaseEffect.h"
#include "VEC2.h"

class cLine : public cBaseEffect
{
	/* 公開関数 */
public:
	cLine(VEC2F pos, float angle, float motion, unsigned int color);	// コンストラクタ
	~cLine() override;													// デストラクタ
	void Update() override;												// 更新
	void Draw_Default() const override;									// 描画・DEFAULT

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const int m_MaxTime;	// 最大生存時間

	VEC2F m_Pos;
	const float m_Angle;
	float m_Motion;
	const unsigned int m_Color;
};
