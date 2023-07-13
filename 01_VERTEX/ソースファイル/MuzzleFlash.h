#pragma once

#include "BaseEffect.h"
#include "VEC2.h"

class cMuzzleFlash : public cBaseEffect
{
	/* 公開関数 */
public:
	cMuzzleFlash(VEC2F pos, float scale, float angle);	// コンストラクタ
	~cMuzzleFlash() override;							// デストラクタ
	void Update() override;								// 更新
	void Draw_Default() const override;					// 描画・DEFAULT

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const int m_MaxTime;	// 最大生存時間

	VEC2F m_Pos;
	const float m_Scale;
	const float m_Angle;
};
