#pragma once

#include "BaseAtkobj.h"
class cPlayer;

/* 風圧 */
class cWind : public cBaseAtkobj
{
	/* 公開関数 */
public:
	cWind(cPlayer* Owner, VEC2F Pos, float Radius, float Power, int Time);	// コンストラクタ
	~cWind() override;														// デストラクタ
	void Update() override;													// 更新

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	VEC2F m_Pos;			// 位置
	const float m_Radius;	// 半径
	const float m_Power;	// 強さ
	int m_Timer;			// 生存時間
};
