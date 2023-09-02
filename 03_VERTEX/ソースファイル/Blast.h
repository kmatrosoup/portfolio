#pragma once

#include <list>
#include "BaseAtkobj.h"
class cPlayer;

class cBlast : public cBaseAtkobj
{
	/* 公開関数 */
public:
	cBlast(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Radius);	// コンストラクタ
	~cBlast() override;												// デストラクタ
	void Update() override;											// 更新
	void Draw_FadeAdd() const override;								// 描画・ADD

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const int m_MaxTime;	// 最大生存時間

	std::list<int> m_HitPlayerID;	// 衝突済判定
	const int m_AtkDamage;			// 攻撃力
	VEC2F m_Pos;					// 位置
	const float m_Radius;			// 半径
	int m_Timer;					// 生存時間
};
