#pragma once

#include "BaseAtkobj.h"
class cPlayer;

class cChargeBullet : public cBaseAtkobj
{
	/* 公開関数 */
public:
	cChargeBullet(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet, int ChargePower);	// コンストラクタ
	~cChargeBullet() override;																				// デストラクタ
	void Update() override;																					// 更新
	void Draw_Default() const override;																		// 描画・DEFAULT
	void Draw_FadeAdd() const override;																		// 描画・ADD

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const float m_Radius_0;		// 半径・溜め0
	static const float m_Radius_1;		// 半径・溜め1
	static const float m_Radius_2;		// 半径・溜め2
	static const float m_Speed_Start_0;	// 初速・溜め0
	static const float m_Speed_Start_1;	// 初速・溜め1
	static const float m_Speed_Start_2;	// 初速・溜め2
	static const float m_Speed_End;		// 最低速度

	bool m_HitFlag;				// 衝突済判定
	const int m_AtkDamage;		// 攻撃力
	VEC2F m_Pos;				// 位置
	VEC2F m_Pos_prev;			// 位置(1フレーム前)
	const float m_Radius;		// 半径
	float m_Speed;				// 移動速度
	float m_Angle;				// 角度
	int m_Ricochet;				// 跳弾数
	const int m_ChargePower;	// チャージ値
};
