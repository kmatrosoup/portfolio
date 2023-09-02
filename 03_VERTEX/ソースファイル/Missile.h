#pragma once

#include "BaseAtkobj.h"
class cPlayer;

class cMissile : public cBaseAtkobj
{
	/* 公開関数 */
public:
	cMissile(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet);	// コンストラクタ
	~cMissile() override;															// デストラクタ
	void Update() override;															// 更新
	void Draw_Default() const override;												// 描画・DEFAULT
	void Draw_FadeAdd() const override;												// 描画・ADD

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const float m_Radius;		// 半径
	static const float m_Accelerator;	// 加速度

	bool m_HitFlag;			// 衝突済判定
	const int m_AtkDamage;	// 攻撃力
	VEC2F m_Pos;			// 位置
	VEC2F m_Pos_prev;		// 位置(1フレーム前)
	float m_Angle;			// 角度
	float m_Speed;			// 移動速度
	int m_Ricochet;			// 跳弾数
	bool m_ExplosionFlag;	// 爆発フラグ
};
