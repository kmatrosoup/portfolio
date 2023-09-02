#pragma once

#include "BaseAtkobj.h"
#include <utility>
#include <list>
class cPlayer;

/* レーザー */
class cLaser : public cBaseAtkobj
{
	/* 公開関数 */
public:
	cLaser(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet);	// コンストラクタ
	~cLaser() override;																// デストラクタ
	void Update() override;															// 更新
	void Draw_Default() const override;												// 描画・DEFAULT
	void Draw_FadeAdd() const override;												// 描画・ADD

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const float m_HitWidth;			// 衝突幅
	static const float m_HitWidth_UpScale;	// 攻撃力による追加衝突幅
	static const int m_MaxTime;				// 生存最大時間

	std::list<int> m_HitPlayerID;				// 衝突済判定
	const int m_AtkDamage;						// 攻撃力
	std::list<std::pair<VEC2F, VEC2F>> m_Ray;	// 判定線

	int m_Timer;	// 生存時間
};
