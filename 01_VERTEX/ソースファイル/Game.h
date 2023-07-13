#pragma once

#include "BaseScene.h"
#include "VEC2.h"
#include <vector>
#include <list>
#include <utility>

class cPlayer;
struct sPlayerInitData;

class cGame : public cBaseScene
{
	/* 公開関数 */
public:
	cGame();						// コンストラクタ
	~cGame() override;				// デストラクタ
	cBaseScene* Update() override;	// 更新
	void Draw() const override;		// 描画

	static void ShakeEffect(int time);						// 地震エフェクト
	static cPlayer* GetNearestOtherPlayer(cPlayer* self);	// 最寄りの他プレイヤーを取得

	/* 非公開関数 */
private:
	cGame(const cGame&) = delete;
	cGame& operator=(const cGame&) = delete;
	cGame(cGame&&) = delete;
	cGame& operator=(cGame&&) = delete;

	/* 公開変数 */
public:
	static const VEC2& ScreenDif;				// 画面のオフセット
	static std::vector<cPlayer*>& pVecPlayer;	// プレイヤーリスト

	/* 非公開変数 */
private:
	static VEC2 m_ScreenDif;					// 画面のオフセット
	static std::vector<cPlayer*> m_pPlayer;		// プレイヤーリスト
	static int m_ShakeTimer;					// 振動演出の経過時間
	static float m_ShakeAngle;					// 振動演出の方向

	std::vector<int> m_SkillEffectTimer;				// スキル発動演出の経過時間
	std::vector<VEC2> m_SkillEffectPos;					// スキル発動演出の座標
	int m_Timer;										// 経過時間
	bool m_FinishBattleFlag;							// 戦闘終了判定
	int m_SpawnMineTimer;								// サドンデスの地雷出現時間
	std::list<std::pair<VEC2F, int>> m_SpawnMinePos;	// サドンデスの地雷出現場所

	int m_Image_IconFrame;		// 画像ハンドル・アイコンフレーム
	int m_Image_SkillIcon_None;	// 画像ハンドル・スキルアイコン・無し
	int m_Image_CountDown[4];	// 画像ハンドル・カウントダウン
	int m_Image_Ready[4];		// 画像ハンドル・READY
	int m_Image_Fight[4];		// 画像ハンドル・FIGHT
	int m_Screen_SkillCoolTime;	// スクリーンハンドル・スキルのクールタイム
};
