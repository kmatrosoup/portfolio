// プリプロセス
#include "Game.h"
#include "DxLib.h"
#include "InputMgr.h"
#include "Player.h"
#include "EnemyMgr.h"
#include "BookMgr.h"
#include "AtkObjMgr.h"
#include "Score.h"
#include "EffectMgr.h"
#include <sstream>

#include "FireballBook.h"

// 定数の宣言
#define BOOK_INTERVAL			(240)		// 本の出現間隔
#define ADD_ENEMYTYPE_TIME		(60 * 15)	// 敵の出現間隔
#define RESTART_ANIMATION_TIME	(30)		// リスタートのアニメーション時間
#define CAN_RESTART_TIME		(30)		// リスタート可能時間

// 静的メンバ変数の宣言
cGame* cGame::m_pInstance(nullptr);
int cGame::m_Image_Title(-1);
int cGame::m_Image_Floor(-1);
int cGame::m_Image_ScoreText(-1);
int cGame::m_Image_ScoreNum[10]{};
int cGame::m_Image_Restart[2]{};
int cGame::m_Screen_Draw(-1);
int cGame::m_Screen_Overlay(-1);
int cGame::m_Sound_Start(-1);
int cGame::m_Sound_Decide(-1);
int cGame::m_Sound_PlayerDead(-1);
int cGame::m_Sound_BGM(-1);

// インスタンスを生成
void cGame::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cGame;

	m_Image_Title = LoadGraph("data\\title.png");
	m_Image_Floor = LoadGraph("data\\floor.png");
	m_Image_ScoreText = LoadGraph("data\\score_text.png");
	LoadDivGraph("data\\score_num.png", 10, 10, 1, 3, 6, m_Image_ScoreNum);
	LoadDivGraph("data\\restart.png", 2, 2, 1, 23, 25, m_Image_Restart);
	m_Screen_Draw = MakeScreen(Sw, Sh);
	m_Screen_Overlay = MakeScreen(Sw, Sh);
	m_Sound_Start = LoadSoundMem("data\\sound\\start.wav");
	m_Sound_Decide = LoadSoundMem("data\\sound\\decide.wav");
	m_Sound_PlayerDead = LoadSoundMem("data\\sound\\player_dead.wav");
	m_Sound_BGM = LoadSoundMem("data\\sound\\bgm.mp3");
}

// インスタンスを解放
void cGame::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;

	DeleteGraph(m_Image_Title);
	DeleteGraph(m_Image_Floor);
	DeleteGraph(m_Image_ScoreText);
	for (int i = 0; i < 10; ++i)	DeleteGraph(m_Image_ScoreNum[i]);
	for (int i = 0; i < 2; ++i)		DeleteGraph(m_Image_Restart[i]);
	DeleteGraph(m_Screen_Draw);
	DeleteGraph(m_Screen_Overlay);
	DeleteSoundMem(m_Sound_Start);
	DeleteSoundMem(m_Sound_Decide);
	DeleteSoundMem(m_Sound_PlayerDead);
	DeleteSoundMem(m_Sound_BGM);
}

// インスタンスを取得
cGame& cGame::GetInstance()
{
	return *m_pInstance;
}

// コンストラクタ
cGame::cGame()
	: m_Phase(ePHASE::TITLE)
	, m_BookTimer(0)
	, m_EnemyTimer(0)
	, m_Overlay_R(0.0f)
	, m_Overlay_G(0.0f)
	, m_Overlay_B(0.0f)
	, m_ColorAngle(0.0f)
	, m_ShakeTimer(0)
	, m_ShakeAngle(0.0f)
	, m_CanRestartTimer(0)
	, m_RestartAnimationTimer(0)
{
	// インスタンスを生成
	cPlayer::CreateInstance();
	cEnemyMgr::CreateInstance();
	cBookMgr::CreateInstance();
	cAtkObjMgr::CreateInstance();
	cScore::CreateInstance();
	cEffectMgr::CreateInstance();

	// 初期化
	m_InitGame();
}

// デストラクタ
cGame::~cGame()
{
	// インスタンスを解放
	cPlayer::DestroyInstance();
	cEnemyMgr::DestroyInstance();
	cBookMgr::DestroyInstance();
	cAtkObjMgr::DestroyInstance();
	cScore::DestroyInstance();
	cEffectMgr::DestroyInstance();
}

// 更新
void cGame::Update()
{
	if (CheckSoundMem(m_Sound_BGM) == 0)
	{
		ChangeVolumeSoundMem(128, m_Sound_BGM);
		PlaySoundMem(m_Sound_BGM, DX_PLAYTYPE_LOOP);
	}

	switch (m_Phase)
	{
	case ePHASE::TITLE:
	{
		// 本を拾うとゲーム開始
		if (cBookMgr::GetInstance().GetBookList().empty())
		{
			PlaySoundMem(m_Sound_Start, DX_PLAYTYPE_BACK);
			m_Phase = ePHASE::PLAYING;
		}
	}
	break;

	case ePHASE::PLAYING:
	{
		// 本の出現
		if ((++m_BookTimer %= BOOK_INTERVAL) == 0)
			cBookMgr::GetInstance().CreateBook(eSPELL::MAX);

		// 敵の出現
		if (--m_EnemyTimer <= 0)
		{
			int type_num = m_GameTimer / ADD_ENEMYTYPE_TIME;
			(type_num < (int)eENEMY::MAX) ?
				cEnemyMgr::GetInstance().CreateEnemy((eENEMY)GetRand(type_num)) :
				cEnemyMgr::GetInstance().CreateEnemy(eENEMY::MAX);
			m_EnemyTimer = 200000 / (m_GameTimer + 1500) + 1;
		}

		// プレイヤーがダメージを受けるとゲームオーバー
		if (cPlayer::GetInstance().IsDead())
		{
			cEnemyMgr::GetInstance().ClearAllEnemy();
			cBookMgr::GetInstance().ClearAllBook();
			cAtkObjMgr::GetInstance().ClearAllAtkObj();
			m_RestartAnimationTimer = 0;
			m_CanRestartTimer = 0;
			PlaySoundMem(m_Sound_PlayerDead, DX_PLAYTYPE_BACK);
			m_Phase = ePHASE::RESULT;
		}

		// 時間経過
		++m_GameTimer;

		// 時間経過でオーバーレイの効果色変更
		m_ColorAngle = DX_PI_F * 2.0f * ((m_GameTimer / (60 * 15)) % 12) / 12.0f;
	}
	break;

	case ePHASE::RESULT:
	{
		// アニメーション
		++m_RestartAnimationTimer %= (RESTART_ANIMATION_TIME * 2);

		// リスタート
		m_CanRestartTimer = min(m_CanRestartTimer + 1, CAN_RESTART_TIME);
		if (m_CanRestartTimer == CAN_RESTART_TIME && InputMgr::In(eINPUT::DECIDE))
		{
			m_InitGame();
			PlaySoundMem(m_Sound_Decide, DX_PLAYTYPE_BACK);
			m_Phase = ePHASE::TITLE;
		}
	}
	break;
	}

	// プレイヤーを更新
	cPlayer::GetInstance().Update();

	// 敵を更新
	cEnemyMgr::GetInstance().Update();

	// 本を更新
	cBookMgr::GetInstance().Update();

	// 攻撃オブジェクトを更新
	cAtkObjMgr::GetInstance().Update();

	// エフェクトを更新
	cEffectMgr::GetInstance().Update();

	// オーバーレイの効果色
	m_Overlay_R += (0.4f + m_GetColorParam(m_ColorAngle + DX_PI_F * 2.0f / 3.0f * 0.0f) * 0.4f - m_Overlay_R) * 0.02f;
	m_Overlay_G += (0.4f + m_GetColorParam(m_ColorAngle + DX_PI_F * 2.0f / 3.0f * 1.0f) * 0.4f - m_Overlay_G) * 0.02f;
	m_Overlay_B += (0.4f + m_GetColorParam(m_ColorAngle + DX_PI_F * 2.0f / 3.0f * 2.0f) * 0.4f - m_Overlay_B) * 0.02f;

	// 画面の振動を減衰
	if (m_ShakeTimer > 0)
		--m_ShakeTimer;
}

// 描画
void cGame::Draw() const
{
	SetDrawScreen(m_Screen_Draw);
	ClearDrawScreen();

	// 移動可能範囲
	DrawRotaGraph(
		(FLOOR_X1 + FLOOR_X2) / 2,
		(FLOOR_Y1 + FLOOR_Y2) / 2,
		10.0, 0.0, m_Image_Floor, TRUE);

	// タイトル
	if (m_Phase == ePHASE::TITLE)
		DrawRotaGraph(Sw / 2, Sh / 2 - 160, 8.0, 0.0, m_Image_Title, TRUE);

	// 本
	cBookMgr::GetInstance().Draw();

	// 敵
	cEnemyMgr::GetInstance().Draw();

	// プレイヤー
	cPlayer::GetInstance().Draw();

	// 攻撃オブジェクト
	cAtkObjMgr::GetInstance().Draw();

	// エフェクト
	cEffectMgr::GetInstance().Draw();

	// スコア
	if (m_Phase == ePHASE::PLAYING)
		m_DrawScore(VEC2(20, 20), 6);
	if (m_Phase == ePHASE::RESULT)
		m_DrawScore(VEC2(Sw / 2 - 180, Sh / 2 - 180), 8);

	// リスタート
	if (m_Phase == ePHASE::RESULT)
		DrawRotaGraph(Sw / 2, Sh / 2 - 40, 4.0, 0.0, m_Image_Restart[m_RestartAnimationTimer / RESTART_ANIMATION_TIME], TRUE);

	// オーバーレイブレンド
	SetDrawScreen(m_Screen_Overlay);
	DrawBox(0, 0, Sw, Sh, (int)(m_Overlay_R * 255.0f) << 16 | (int)(m_Overlay_G * 255.0f) << 8 | (int)(m_Overlay_B * 255.0f) << 0, TRUE);
	GraphBlend(m_Screen_Draw, m_Screen_Overlay, 255, DX_GRAPH_BLEND_OVERLAY);

	// 画面の振動値
	VEC2 screen_dif = VEC2(VEC2F(cosf(m_ShakeAngle), sinf(m_ShakeAngle)) * sinf((float)m_ShakeTimer * 0.5f) * (float)m_ShakeTimer * 0.2f);

	// 画面に描画
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(screen_dif.x, screen_dif.y, m_Screen_Draw, FALSE);
}

// 画面の揺れ効果
void cGame::ShakeEffect(int time)
{
	m_ShakeTimer = __max(m_ShakeTimer, time);
	m_ShakeAngle = DX_PI_F / 180.0f * GetRand(359);
}

// ゲームの初期化
void cGame::m_InitGame()
{
	// データの初期化
	m_BookTimer = 0;
	m_EnemyTimer = 0;
	m_GameTimer = 0;
	m_ColorAngle = 0.0f;
	cPlayer::GetInstance().Init();
	cScore::GetInstance().Init();

	// 中心に本を配置
	cBaseBook* book = new cFireballBook(
		VEC2F((FLOOR_X1 + FLOOR_X2) / 2.0f, (FLOOR_Y1 + FLOOR_Y2) / 2.0f),
		VEC2F(0.0f, 0.0f),
		0.0f,
		true);
	cBookMgr::GetInstance().AddBook(book);
}

// 角度から彩度を取得
float cGame::m_GetColorParam(float angle) const
{
	while (angle < 0.0f)			angle += DX_PI_F * 2.0f;
	while (angle >= DX_PI_F * 2.0f)	angle -= DX_PI_F * 2.0f;

	if (angle < DX_PI_F * 2.0f / 6.0f)
		return 1.0f;
	angle -= DX_PI_F * 2.0f / 6.0f;

	if (angle < DX_PI_F * 2.0f / 6.0f)
		return 1.0f - (angle / (DX_PI_F * 2.0f / 6.0f));
	angle -= DX_PI_F * 2.0f / 6.0f;

	if (angle < DX_PI_F * 2.0f / 6.0f)
		return 0.0f;
	angle -= DX_PI_F * 2.0f / 6.0f;

	if (angle < DX_PI_F * 2.0f / 6.0f)
		return 0.0f;
	angle -= DX_PI_F * 2.0f / 6.0f;

	if (angle < DX_PI_F * 2.0f / 6.0f)
		return angle / (DX_PI_F * 2.0f / 6.0f);

	return 1.0f;
}

// スコアの描画
void cGame::m_DrawScore(VEC2 pos, int scale) const
{
	std::ostringstream oss;
	oss << cScore::GetInstance().GetScore();

	DrawExtendGraph(
		pos.x,
		pos.y,
		pos.x + 19 * scale,
		pos.y + 6 * scale,
		m_Image_ScoreText, TRUE);

	for (int i = 0; i < (int)oss.str().length(); ++i)
	{
		DrawExtendGraph(
			pos.x + 24 * scale + 4 * scale * i,
			pos.y,
			pos.x + 24 * scale + 4 * scale * i + 3 * scale,
			pos.y + 6 * scale,
			m_Image_ScoreNum[oss.str()[i] - '0'], TRUE);
	}
}
