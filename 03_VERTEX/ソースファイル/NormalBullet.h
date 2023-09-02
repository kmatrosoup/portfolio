#pragma once

#include "BaseAtkobj.h"
class cPlayer;

class cNormalBullet : public cBaseAtkobj
{
	/* 公開関数 */
public:
	cNormalBullet(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet);	// コンストラクタ
	~cNormalBullet() override;															// デストラクタ
	void Update() override;																// 更新
	void Draw_Default() const override;													// 描画・DEFAULT

	bool Active() const override;	// アクティブ状態

	static void LoadResource();		// リソースの読み込み
	static void DeleteResource();	// リソースの解放

	/* 非公開変数 */
private:
	static const float m_Radius;	// 半径
	static const float m_Speed;		// 移動速度

	bool m_HitFlag;			// 衝突済判定
	const int m_AtkDamage;	// 攻撃力
	VEC2F m_Pos;			// 位置
	VEC2F m_Pos_prev;		// 位置(1フレーム前)
	float m_Angle;			// 角度
	int m_Ricochet;			// 跳弾数

	static bool m_LoadedFlag;			// リソースの読み込みフラグ
	static int m_Image_NormalBullet;	// 画像ハンドル・通常弾
};
