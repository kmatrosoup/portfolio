#pragma once

#include "BaseEffect.h"
#include "VEC2.h"

class cNormalBullet_Trajectory : public cBaseEffect
{
	/* 公開関数 */
public:
	cNormalBullet_Trajectory(VEC2F pos, float angle, unsigned int color, double max_scale);	// コンストラクタ
	~cNormalBullet_Trajectory() override;													// デストラクタ
	void Update() override;																	// 更新
	void Draw_Default() const override;														// 描画・DEFAULT

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const int m_MaxTime;	// 最大生存時間

	VEC2F m_Pos;
	const float m_Angle;
	const unsigned int m_Color;
	const double m_MaxScale;
};
