#pragma once

#include "BaseAtkobj.h"
class cPlayer;

class cDeathBullet : public cBaseAtkobj
{
	/* 公開関数 */
public:
	cDeathBullet(cPlayer* Owner, VEC2F Pos, float Angle, int Ricochet);	// コンストラクタ
	~cDeathBullet() override;											// デストラクタ
	void Update() override;												// 更新
	void Draw_FadeAdd() const override;									// 描画・ADD

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const float m_Radius;	// 半径
	static const float m_Speed;		// 移動速度

	bool m_HitFlag;		// 衝突済判定
	VEC2F m_Pos;		// 位置
	VEC2F m_Pos_prev;	// 位置(1フレーム前)
	float m_Angle;		// 角度
	int m_Ricochet;		// 跳弾数
};
