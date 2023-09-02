#pragma once

#include "BaseAtkobj.h"
class cPlayer;

class cOrb : public cBaseAtkobj
{
	/* 公開関数 */
public:
	cOrb(cPlayer* Owner, int AtkDamage, int id);	// コンストラクタ
	~cOrb() override;								// デストラクタ
	void Update() override;							// 更新
	void Draw_Default() const override;				// 描画・DEFAULT
	void Draw_FadeAdd() const override;				// 描画・ADD

	bool Active() const override;	// アクティブ状態

	/* 非公開変数 */
private:
	static const float m_Radius;		// 半径
	static const float m_Speed;			// 移動速度
	static const float m_Distance;		// 所有者との距離
	static const int m_MaxHomingTime;	// ホーミング最大時間

	bool m_HitFlag;			// 衝突済判定
	const int m_AtkDamage;	// 攻撃力
	VEC2F m_Pos;			// 位置
	VEC2F m_Pos_prev;		// 位置(1フレーム前)
	float m_Angle;			// 角度
	bool m_FollowFlag;		// 追従フラグ
	int m_FollowID;			// 追従ID
	int m_HomingTimer;		// ホーミング経過時間
};
