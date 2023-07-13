#pragma once

#include "System.h"
#include "VEC2.h"

// 定数の宣言
#define FLOOR_X1	(80)	// 移動可能範囲の左座標
#define FLOOR_Y1	(550)	// 移動可能範囲の上座標
#define FLOOR_X2	(520)	// 移動可能範囲の右座標
#define FLOOR_Y2	(750)	// 移動可能範囲の下座標

class cGame
{
	// 列挙定数
private:
	enum class ePHASE
	{
		TITLE,
		PLAYING,
		RESULT,
	};

	// 公開関数
public:
	static void CreateInstance();	// インスタンスを生成
	static void DestroyInstance();	// インスタンスを解放
	static cGame& GetInstance();	// インスタンスを取得

	void Update();		// 更新
	void Draw() const;	// 描画

	void ShakeEffect(int time);					// 画面の振動演出

	// 非公開関数
private:
	cGame(const cGame&) = delete;
	cGame& operator=(const cGame&) = delete;
	cGame(cGame&&) = delete;
	cGame& operator=(cGame&&) = delete;

	cGame();	// コンストラクタ
	~cGame();	// デストラクタ

	void m_InitGame();								// ゲームの初期化
	float m_GetColorParam(float angle) const;		// 角度から彩度を取得
	void m_DrawScore(VEC2 pos, int scale) const;	// スコアの描画

	// 非公開変数
private:
	static cGame* m_pInstance;
	static int m_Image_Title;
	static int m_Image_Floor;
	static int m_Image_ScoreText;
	static int m_Image_ScoreNum[10];
	static int m_Image_Restart[2];
	static int m_Screen_Draw;
	static int m_Screen_Overlay;
	static int m_Sound_Start;
	static int m_Sound_Decide;
	static int m_Sound_PlayerDead;
	static int m_Sound_BGM;

	ePHASE m_Phase;
	int m_BookTimer;
	int m_EnemyTimer;
	int m_GameTimer;
	float m_Overlay_R;
	float m_Overlay_G;
	float m_Overlay_B;
	float m_ColorAngle;
	int m_ShakeTimer;
	float m_ShakeAngle;
	int m_CanRestartTimer;
	int m_RestartAnimationTimer;
};
