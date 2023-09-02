#pragma once

#include "BaseAtkobj.h"
class cPlayer;

class cElectricTrap : public cBaseAtkobj
{
	/* 公開関数 */
public:
	cElectricTrap(cPlayer* Owner, VEC2F Pos);	// コンストラクタ
	~cElectricTrap() override;					// デストラクタ
	void Update() override;						// 更新
	void Draw_FadeAdd() const override;			// 描画・ADD

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const float m_Radius;	// 半径
	static const int m_MaxTime;		// 最大生存時間

	VEC2F m_Pos;	// 位置
	int m_Timer;	// 生存時間
};
