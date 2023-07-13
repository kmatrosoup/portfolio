#pragma once

#include "BaseEffect.h"

class cDeath : public cBaseEffect
{
	/* 公開関数 */
public:
	cDeath(VEC2F pos);					// コンストラクタ
	~cDeath() override;					// デストラクタ
	void Update() override;				// 更新
	void Draw_Default() const override;	// 描画・DEFAULT

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const int m_MaxTime;	// 最大生存時間

	VEC2F m_Pos;	// 位置
	double m_Angle;	// 角度
};
