#pragma once

#include "BaseEffect.h"
#include "VEC2.h"

class cAura : public cBaseEffect
{

	/* 公開関数 */
public:
	cAura(VEC2F pos, unsigned int color, float scale, float angle, float motion);	// コンストラクタ
	~cAura() override;												// デストラクタ
	void Update() override;												// 更新
	void Draw_Default() const override;									// 描画・DEFAULT

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const int m_MaxTime;	// 最大生存時間

	VEC2F m_Pos;
	unsigned int m_Color;
	float m_Scale;
	VEC2F m_Motion;
};
