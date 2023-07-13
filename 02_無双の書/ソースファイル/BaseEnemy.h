#pragma once

#include "VEC2.h"

enum class eENEMY
{
	SLIME,
	BAT,
	SKULL,

	MAX
};

class cBaseEnemy
{
	// 公開関数
public:
	static void LoadResource();		// リソースの読み込み
	static void DeleteResource();	// リソースの解放

	cBaseEnemy(eENEMY enemy, int hp, float radius, float x);	// コンストラクタ
	~cBaseEnemy();												// デストラクタ
	void Update();												// 更新
	void Draw() const;											// 描画

	bool IsActive() const;	// アクティブ状態

	VEC2F GetPos() const;			// 座標を取得
	float GetRadius() const;		// 半径を取得
	void AddMotion(VEC2F motion);	// 移動量を加算
	void TakenDamage(int value);	// 被ダメージ処理

	// 保護関数
protected:
	virtual void m_OwnProcess() = 0;	// 固有処理
	virtual void m_AddScore() = 0;		// スコア加算

	// 保護変数
protected:
	const eENEMY m_OwnType;
	int m_HP;
	VEC2F m_Pos;
	VEC2F m_Motion;
	bool m_InversionFlag;
	const float m_Radius;
	int m_AnimationTimer;

	// 非公開変数
private:
	static bool m_LoadedResourceFlag;
	static int m_Image_Enemy[(int)eENEMY::MAX][2];
	static int m_Sound_Hit;
	static int m_Sound_Dead;
};
