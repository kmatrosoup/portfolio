#pragma once

#include "System.h"
#include "BaseEffect.h"
#include <list>

/* パーティクルの種類 */
enum class ePARTICLE
{
	LINE,			// 線
	RHOMBUS,		// ひし形
	RING_IN,		// リング・内側
	RING_OUT,		// リング・外側
	MUZZLE_FLASH,	// マズルフラッシュ
	SHIELD,			// シールド
	AURA,			// オーラ
	CONFETTI_1,		// 紙吹雪1
	CONFETTI_2,		// 紙吹雪2
	CONFETTI_3,		// 紙吹雪3

	MAX
};

/* エフェクト管理クラス */
class cEffectMgr
{
	/* 公開関数 */
public:
	static void CreateInstance();		// インスタンスの生成
	static void DestroyInstance();		// インスタンスの解放
	static cEffectMgr& GetInstance();	// インスタンスの取得

	void Update();		// 更新
	void Draw() const;	// 描画

	void Create_Line(VEC2F Pos, float Angle, float Motion, unsigned int Color);							// エフェクトを生成・線
	void Create_Circle(VEC2F Pos, float Radius, float Motion, unsigned int Color);						// エフェクトを生成・円
	void Create_MuzzleFlash(VEC2F Pos, float Scale, float Angle);										// エフェクトを生成・マズルフラッシュ
	void Create_Aura(VEC2F Pos, float Scale, float Angle, float Motion, unsigned int Color);			// エフェクトを生成・オーラ
	void Create_Confetti(VEC2F Pos, float Angle, float Motion, unsigned int Color);						// エフェクトを生成・紙吹雪
	void Create_Dead(VEC2F Pos, unsigned int Color);													// エフェクトを生成・死亡
	void Create_NormalBullet_Trajectory(VEC2F Pos, float Angle, unsigned int Color, double MaxScale);	// エフェクトを生成・通常弾の軌跡
	void Create_DeathBullet_Hit(VEC2F Pos);																// エフェクトを生成・デスの被弾
	int	GetImageHandle(ePARTICLE particle) const;														// パーティクルの画像ハンドルを取得

	/* 非公開関数 */
private:
	::DISALLOW_COPY_CONSTRUCTOR(cEffectMgr);
	::DISALLOW_MOVE_CONSTRUCTOR(cEffectMgr);

	cEffectMgr();	// コンストラクタ
	~cEffectMgr();	// デストラクタ

	/* 非公開変数 */
private:
	static cEffectMgr* m_pInstance;	// インスタンスのポインタ

	std::list<cBaseEffect*> m_pEffectList;		// エフェクトリスト
	std::list<cBaseEffect*> m_pNewEffectList;	// 新規エフェクトリスト

	int m_Image_Particle[(int)ePARTICLE::MAX];	// 画像ハンドル・パーティクル
};
