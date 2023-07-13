#pragma once

#include "VEC2.h"

enum class eSPELL
{
	FIRE_BALL,
	EXPLOSION,

	MAX
};

class cBaseBook
{
	// 公開関数
public:
	static void LoadResource();		// リソースの読み込み
	static void DeleteResource();	// リソースの解放

	cBaseBook(eSPELL spell, VEC2F pos, VEC2F motion, float angle, int count, bool no_erase_flag);	// コンストラクタ
	~cBaseBook();																					// デストラクタ
	void Update();																					// 更新
	void Draw() const;																				// 描画

	void RunMagic();	// 魔法を発動
	bool IsActive();	// アクティブ状態

	VEC2F GetPos() const;				// 座標を取得
	void SetPos(VEC2F pos);				// 座標を設定
	VEC2F GetMotion() const;			// 移動量を取得
	void SetMotion(VEC2F motion);		// 移動量を設定
	float GetAngle() const;				// 角度を取得
	void SetAngle(float angle);			// 角度を設定
	bool GetInversion() const;			// 向きを取得
	void SetInversion(bool inversion);	// 向きを設定
	int GetActiveTime() const;			// アクティブ時間を取得

	// 保護関数
protected:
	virtual void m_RunMagicProcess() = 0;	// 発動時の処理

	// 保護変数
protected:
	const eSPELL m_Spell;
	VEC2F m_Pos;
	VEC2F m_Motion;
	float m_Angle;
	bool m_InversionFlag;

	// 非公開変数
private:
	static bool m_LoadedResourceFlag;
	static int m_Image_Book[];

	int m_RestCount;
	int m_ActiveTimer;
	const bool m_NoEraseFlag;
};
