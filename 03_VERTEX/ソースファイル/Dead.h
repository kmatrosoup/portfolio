#pragma once

#include "BaseEffect.h"

class cDead : public cBaseEffect
{
	/* 公開関数 */
public:
	cDead(VEC2F pos, unsigned int color);	// コンストラクタ
	~cDead() override;						// デストラクタ
	void Update() override;					// 更新
	void Draw_Default() const override;		// 描画・DEFAULT

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const int m_MaxTime;	// 最大生存時間

	VEC2F m_Pos;
	unsigned int m_Color;
};
