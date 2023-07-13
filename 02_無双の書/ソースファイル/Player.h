#pragma once

#include "VEC2.h"

class cBaseBook;

class cPlayer
{
	// 公開関数
public:
	static void CreateInstance();	// インスタンスを生成
	static void DestroyInstance();	// インスタンスを解放
	static cPlayer& GetInstance();	// インスタンスを取得

	void Init();		// 初期化
	void Update();		// 更新
	void Draw() const;	// 描画

	VEC2F GetPos() const;	// 座標を取得
	bool IsDead() const;	// 死亡判定

	// 非公開関数
private:
	cPlayer(const cPlayer&) = delete;
	cPlayer& operator=(const cPlayer&) = delete;
	cPlayer(cPlayer&&) = delete;
	cPlayer& operator=(cPlayer&&) = delete;

	cPlayer();		// コンストラクタ
	~cPlayer();		// デストラクタ

	// 非公開変数
private:
	static cPlayer* m_pInstance;
	static int m_Image_Head[2];
	static int m_Image_Body[3];
	static int m_Image_Grave;
	static int m_Image_Cursor[13];
	static int m_Image_MovementGuide[2];
	static int m_Image_HoldGuide[2];
	static int m_Image_UseGuide[2];
	static int m_Sound_Pickup;

	VEC2F m_Pos;
	VEC2F m_Motion;
	bool m_InversionFlag;
	cBaseBook* m_pCursor;
	cBaseBook* m_pHoldBook;
	int m_PickupTimer;
	int m_UseTimer;
	bool m_DeadFlag;
	int m_HoldSwingTimer;
	float m_HoldAngle;
	int m_HeadAnimationTimer;
	int m_BodyAnimationTimer;
	int m_GuideAnimationTimer;
	int m_MovementGuide_Alpha;
	int m_ShowMovementGuideTimer;
	int m_HoldGuide_Alpha;
	int m_ShowHoldGuideTimer;
	int m_UseGuide_Alpha;
	int m_ShowUseGuideTimer;
};
