//////////////////////
//   プリプロセス   //
//////////////////////
#include "Game.h"
#include "DxLib.h"
#include "System.h"
#include "SceneMgr.h"
#include "CommonData.h"
#include "SoundData.h"
#include "BgmData.h"
#include "Result.h"
#include "Easing.h"
#include <algorithm>

#include "Field.h"
#include "Player.h"
#include "AtkobjMgr.h"
#include "EffectMgr.h"
using namespace std;

#include "Input.h"

////////////////////
//   定数の定義   //
////////////////////

#define HP_GAUGE_NUM	(10)	// HPゲージの表示数

#define MARGIN_W		(10)	// 余白・横
#define MARGIN_H		(10)	// 余白・縦
#define SKILL_FRAME_W	(3)		// スキルフレーム・横
#define SKILL_FRAME_H	(3)		// スキルフレーム・縦
#define SKILL_W			(72)	// スキルアイコン・横
#define SKILL_H			(72)	// スキルアイコン・縦
#define HP_FRAME_W		(2)		// HPフレーム・横
#define HP_FRAME_H		(2)		// HPフレーム・縦
#define HP_W			(30)	// HPゲージ・横
#define HP_H			(16)	// HPゲージ・縦
#define BULLET_MARGIN_H	(5)		// 弾数余白・縦
#define BULLET_W		(42)	// 弾数ゲージ・横
#define BULLET_H		(10)	// 弾数ゲージ・縦

#define MAX_START_TIME		(120)		// 開始時間
#define MAX_TIME_LIMIT		(60 * 120)	// 制限時間
#define MAX_END_TIME		(180)		// 終了時間
#define MINE_INTERVAL		(60)		// サドンデス時の地雷発生間隔
#define START_FLASH_TIME	(60)		// 開始時の演出時間
#define MAX_RUNSKILL_TIME	(18)		// スキル発動の演出時間
#define SHAKE_INTERVAL		(3)			// 振動演出の揺れの間隔

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 公開変数 */
const VEC2& cGame::ScreenDif(m_ScreenDif);		// 画面のオフセット
vector<cPlayer*>& cGame::pVecPlayer(m_pPlayer);	// プレイヤーリスト

/* 非公開変数 */
VEC2 cGame::m_ScreenDif(0, 0);		// 画面のオフセット
vector<cPlayer*> cGame::m_pPlayer;	// プレイヤーリスト
int cGame::m_ShakeTimer(0);			// 振動演出の経過時間
float cGame::m_ShakeAngle(0.0f);	// 振動演出の方向

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cGame::cGame()
	: m_Timer(-MAX_START_TIME)
	, m_FinishBattleFlag(false)
	, m_SpawnMineTimer(0)
{
	// リソースの読み込み
	m_Image_IconFrame = LoadGraph("data\\image\\icon\\icon_frame.png");
	m_Image_SkillIcon_None = LoadGraph("data\\image\\icon\\skill_none.png");
	LoadDivGraph("data\\image\\game\\countdown.png", 4, 1, 4, 11, 8, m_Image_CountDown);
	LoadDivGraph("data\\image\\game\\ready.png", 4, 1, 4, 59, 20, m_Image_Ready);
	LoadDivGraph("data\\image\\game\\fight.png", 4, 1, 4, 59, 20, m_Image_Fight);
	cPlayer::LoadResource();

	m_Screen_SkillCoolTime = MakeScreen(24, 24, FALSE);
	SetDrawScreen(m_Screen_SkillCoolTime);
	ClearDrawScreen();
	cSceneMgr::GetInstance().ChangeScreen(eSCREEN::DEFAULT);

	// ゲームオブジェクトの初期化
	cField::Init(cCommonData::FieldID);
	for (int i = 0; i < 2 + cCommonData::Player4Flag * 2; ++i)
		m_pPlayer.push_back(new cPlayer(i, cCommonData::PlayerInitData[i]));
	cAtkobjMgr::CreateInstance();
	cEffectMgr::CreateInstance();

	// 初期化処理
	for (int i = 0; i < 4; i++)
		cCommonData::Ranking[i] = -1;

	m_SkillEffectTimer.assign(2 + (long long)cCommonData::Player4Flag * 2, 0);
	m_SkillEffectPos.assign(2 + (long long)cCommonData::Player4Flag * 2, VEC2::ZERO);
	for (auto& i : m_pPlayer)
		i->SetPos(cField::GetPlayerStartPos(i->PlayerID));

	m_ShakeTimer = 0;
	m_ShakeAngle = 0.0f;

	// BGMの再生
	cBgmData::GetInstance().Play(eBGM::BATTLE, false);
}

/* デストラクタ */
cGame::~cGame()
{
	// リソースの解放
	DeleteGraph(m_Image_IconFrame);
	DeleteGraph(m_Image_SkillIcon_None);
	for (int i = 0; i < 4; ++i)	DeleteGraph(m_Image_CountDown[i]);
	for (int i = 0; i < 4; ++i)	DeleteGraph(m_Image_Ready[i]);
	for (int i = 0; i < 4; ++i)	DeleteGraph(m_Image_Fight[i]);
	DeleteGraph(m_Screen_SkillCoolTime);
	cPlayer::DeleteResource();

	// ゲームオブジェクトの終了
	cEffectMgr::DestroyInstance();
	cAtkobjMgr::DestroyInstance();
	for (auto& i : m_pPlayer) {
		delete i;
		i = nullptr;
	}
	m_pPlayer.clear();
	cField::Fin();

	// ランキングが空いていたら0で埋める
	for (int i = 0; i < 4; ++i)
		if (cCommonData::Ranking[i] == -1)
			cCommonData::Ranking[i] = 0;
}

/* 更新 */
cBaseScene* cGame::Update()
{
	if (Input::Keyboard::In(KEY_INPUT_Z)) {
		return new cResult();
	}

	// タイマー制御
	if (m_Timer < MAX_TIME_LIMIT || m_FinishBattleFlag)
		++m_Timer;

	// 地雷の生成
	if (m_Timer >= MAX_TIME_LIMIT && !m_FinishBattleFlag) {
		if ((++m_SpawnMineTimer %= MINE_INTERVAL) == 0) {
			VEC2F pos;
			while (true) {
				pos = VEC2F(GetRand(cField::GetCenterPos().x * 20) * 0.1f, GetRand(cField::GetCenterPos().y * 20) * 0.1f);
				if (cField::InWall(pos))
					continue;
				m_SpawnMinePos.push_back({ pos, 60 });
				break;
			}
		}
	}
	for (auto i = m_SpawnMinePos.begin(); i != m_SpawnMinePos.end();) {
		if (--i->second < 0) {
			cAtkobjMgr::GetInstance().Create_MineTrap(nullptr, i->first, VEC2F::ZERO);
			cEffectMgr::GetInstance().Create_Circle(i->first, 0.0f, 20.0f, 0xffffff);
			i = m_SpawnMinePos.erase(i);
		}
		else {
			++i;
		}
	}

	// 勝敗の判定
	if (!m_FinishBattleFlag) {
		if ((int)count_if(m_pPlayer.begin(), m_pPlayer.end(), [](const cPlayer* p) { return !p->IsDead(); }) <= 1) {
			if (cCommonData::Ranking[0] == -1) {
				for (auto& i : m_pPlayer) {
					if (!i->IsDead()) {
						cCommonData::Ranking[0] = i->PlayerID;
						break;
					}
				}
			}
			m_Timer = MAX_TIME_LIMIT;
			m_FinishBattleFlag = true;
		}
	}
	else {
		if (++m_Timer >= MAX_TIME_LIMIT + MAX_END_TIME)
			return new cResult();
	}

	// ゲームオブジェクトの更新
	cField::Update();
	if (m_Timer >= 0)
		for (auto& i : m_pPlayer)
			i->Update();
	cAtkobjMgr::GetInstance().Update();
	cEffectMgr::GetInstance().Update();

	// 画面の揺れ
	if (m_ShakeTimer > 0)
		if (--m_ShakeTimer % SHAKE_INTERVAL == 0)
			m_ShakeAngle += DX_PI_F / 2.0f + DX_PI_F * (GetRand(10000) / 10000.0f);

	// 描画位置のズレ
	m_ScreenDif = SSize / 2 - cField::GetCenterPos();
	if (m_ShakeTimer > 0)
		m_ScreenDif += VEC2((int)roundf(cosf(m_ShakeAngle) * (m_ShakeTimer + 1) / 8.0f), (int)roundf(sinf(m_ShakeAngle) * (m_ShakeTimer + 1) / 8.0f));

	// スキル使用演出
	for (auto& i : m_SkillEffectTimer)
		if (i > 0)
			--i;
	for (auto& i : m_pPlayer)
		if (i->CheckRunSkill()) {
			m_SkillEffectTimer[i->PlayerID] = MAX_RUNSKILL_TIME;
			m_SkillEffectPos[i->PlayerID] = i->Pos;
			//cSoundData::GetInstance().Play(eSOUND::RUN_SKILL);
		}

	return this;
}

/* 描画 */
void cGame::Draw() const
{
	// ゲームオブジェクトの描画
	cField::Draw();
	cAtkobjMgr::GetInstance().Draw();
	{
		std::list<cPlayer*> draw_order;
		for (int i = 0; i < 2 + cCommonData::Player4Flag * 2; ++i)
			draw_order.push_back(m_pPlayer[i]);
		draw_order.sort(&cPlayer::Comp);

		cSceneMgr::GetInstance().ChangeScreen(eSCREEN::FADE_ADD);
		for (const auto& i : draw_order) {
			i->Draw_FadeAdd();
		}
		cSceneMgr::GetInstance().ChangeScreen(eSCREEN::DEFAULT);
		for (const auto& i : draw_order) {
			i->Draw_Default();
		}
	}
	cEffectMgr::GetInstance().Draw();

	// 地雷の生成場所
	for (const auto& i : m_SpawnMinePos) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - i.second * 200 / 60);
		DrawCircle(
			m_ScreenDif.x + (int)i.first.x,
			m_ScreenDif.y + (int)i.first.y,
			2 + (60 - i.second) / 5, 0x000000, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 1Pステータス
	{
		// HP
		DrawBox(
			MARGIN_W + SKILL_FRAME_W + SKILL_W + SKILL_FRAME_W,
			MARGIN_H,
			MARGIN_W + SKILL_FRAME_W + SKILL_W + SKILL_FRAME_W + HP_FRAME_W + (HP_W + HP_FRAME_W) * HP_GAUGE_NUM,
			MARGIN_H + HP_FRAME_H + HP_H + HP_FRAME_H,
			0xffffffff, TRUE);
		for (int i = 0; i < HP_GAUGE_NUM; ++i) {
			int hp_num = m_pPlayer[0]->HP / HP_GAUGE_NUM + (m_pPlayer[0]->HP % HP_GAUGE_NUM > i);
			DrawBox(
				MARGIN_W + SKILL_FRAME_W + SKILL_W + SKILL_FRAME_W + HP_FRAME_W + i * 32,
				MARGIN_H + HP_FRAME_H,
				MARGIN_W + SKILL_FRAME_W + SKILL_W + SKILL_FRAME_W + HP_FRAME_W + HP_W + i * 32,
				MARGIN_H + HP_FRAME_H + HP_H,
				(hp_num == 0) ? 0x000000 : (hp_num == 1) ? 0x00ff00 : (hp_num == 2) ? 0xffd000 : (hp_num == 3) ? 0xe02040 : 0xe0e0e0, TRUE);
		}

		// 弾数
		for (int i = 0; i < m_pPlayer[0]->MaxBulletNum; ++i) {
			((m_pPlayer[0]->MaxBulletNum - i) <= m_pPlayer[0]->BulletNum) ?
				SetDrawAddColor(0, 0, 0) :
				SetDrawAddColor(-13, -92, -70);
			DrawBox(
				MARGIN_W,
				MARGIN_H + SKILL_FRAME_H + SKILL_H + SKILL_FRAME_H + BULLET_MARGIN_H + i * BULLET_H,
				MARGIN_W + BULLET_W,
				MARGIN_H + SKILL_FRAME_H + SKILL_H + SKILL_FRAME_H + BULLET_MARGIN_H + BULLET_H + i * BULLET_H,
				0x0def80, TRUE);
		}
		SetDrawAddColor(0, 0, 0);

		// コア
		for (int i = 0; i < cPlayer::MaxCoreNum; ++i)
			DrawCircle(
				94 + i * 24,
				40,
				8, 0xffffff, i < m_pPlayer[0]->CoreNum);

		// スキルアイコン
		SetDrawAddColor(160, 0, 0);
		DrawExtendGraph(
			MARGIN_W,
			MARGIN_H,
			MARGIN_W + SKILL_FRAME_W + SKILL_W + SKILL_FRAME_W,
			MARGIN_H + SKILL_FRAME_H + SKILL_H + SKILL_FRAME_H,
			m_Image_IconFrame, FALSE);
		SetDrawAddColor(0, 0, 0);
		(m_pPlayer[0]->Skill != eSKILL::MAX) ?
			DrawExtendGraph(
				MARGIN_W + SKILL_FRAME_W,
				MARGIN_H + SKILL_FRAME_H,
				MARGIN_W + SKILL_FRAME_W + SKILL_W,
				MARGIN_H + SKILL_FRAME_H + SKILL_H,
				cCommonData::GetIcon_Skill(m_pPlayer[0]->Skill), TRUE) :
			DrawExtendGraph(
				MARGIN_W + SKILL_FRAME_W,
				MARGIN_H + SKILL_FRAME_H,
				MARGIN_W + SKILL_FRAME_W + SKILL_W,
				MARGIN_H + SKILL_FRAME_H + SKILL_H,
				m_Image_SkillIcon_None, TRUE);

		if (m_pPlayer[0]->Skill != eSKILL::MAX && m_pPlayer[0]->SkillCoolTime > 0)
		{
			float angle = DX_PI_F * 2.0f * m_pPlayer[0]->SkillCoolTime / m_pPlayer[0]->MaxSkillCoolTime;
			SetDrawScreen(m_Screen_SkillCoolTime);
			ClearDrawScreen();
			if (angle > DX_PI_F / 2.0f * 4.0f)	DrawBox(12, 0, 24, 12, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 3.0f)	DrawBox(12, 12, 24, 24, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 2.0f)	DrawBox(0, 12, 12, 24, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 1.0f)	DrawBox(0, 0, 12, 12, 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 4.0f && angle > DX_PI_F / 2.0f * 3.0f)
				DrawTriangle(12, 12, 36, 12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 3.0f && angle > DX_PI_F / 2.0f * 2.0f)
				DrawTriangle(12, 12, 12, 36, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 2.0f && angle > DX_PI_F / 2.0f * 1.0f)
				DrawTriangle(12, 12, -12, 12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 1.0f && angle > DX_PI_F / 2.0f * 0.0f)
				DrawTriangle(12, 12, 12, -12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			cSceneMgr::GetInstance().ChangeScreen(eSCREEN::DEFAULT);

			SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
			DrawExtendGraph(
				MARGIN_W + SKILL_FRAME_W,
				MARGIN_H + SKILL_FRAME_H,
				MARGIN_W + SKILL_FRAME_W + SKILL_W,
				MARGIN_H + SKILL_FRAME_H + SKILL_H,
				m_Screen_SkillCoolTime, FALSE);
			SetDrawBlendMode(DX_BLENDMODE_ADD, (int)(m_SkillEffectTimer[0] * 255.0f / MAX_RUNSKILL_TIME));
			DrawBox(
				MARGIN_W + SKILL_FRAME_W - (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[0] / MAX_RUNSKILL_TIME) * 30.0f),
				MARGIN_H + SKILL_FRAME_H - (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[0] / MAX_RUNSKILL_TIME) * 30.0f),
				MARGIN_W + SKILL_FRAME_W + SKILL_W + (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[0] / MAX_RUNSKILL_TIME) * 30.0f),
				MARGIN_H + SKILL_FRAME_H + SKILL_H + (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[0] / MAX_RUNSKILL_TIME) * 30.0f),
				0xffffff, FALSE);
			DrawRotaGraph(
				m_ScreenDif.x + m_SkillEffectPos[0].x,
				m_ScreenDif.y + m_SkillEffectPos[0].y,
				1.0 + ((long long)MAX_RUNSKILL_TIME - m_SkillEffectTimer[0]) / 5.0, 0.0, cCommonData::GetIcon_Skill(m_pPlayer[0]->Skill), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	// 2Pステータス
	{
		// HP
		DrawBox(
			Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - SKILL_FRAME_W - HP_FRAME_W - (HP_W + HP_FRAME_W) * HP_GAUGE_NUM,
			Sh - MARGIN_H - HP_FRAME_H - HP_H - HP_FRAME_H,
			Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - SKILL_FRAME_W,
			Sh - MARGIN_H,
			0xffffffff, TRUE);
		for (int i = 0; i < HP_GAUGE_NUM; ++i) {
			int hp_num = m_pPlayer[1]->HP / HP_GAUGE_NUM + (m_pPlayer[1]->HP % HP_GAUGE_NUM > i);
			DrawBox(
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - SKILL_FRAME_W - HP_FRAME_W - HP_W - i * 32,
				Sh - MARGIN_H - HP_FRAME_H - HP_H,
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - SKILL_FRAME_W - HP_FRAME_W - i * 32,
				Sh - MARGIN_H - HP_FRAME_H,
				(hp_num == 0) ? 0x000000 : (hp_num == 1) ? 0x00ff00 : (hp_num == 2) ? 0xffd000 : (hp_num == 3) ? 0xe02040 : 0xe0e0e0, TRUE);
		}

		// 弾数
		for (int i = 0; i < m_pPlayer[1]->MaxBulletNum; ++i) {
			((m_pPlayer[1]->MaxBulletNum - i) <= m_pPlayer[1]->BulletNum) ?
				SetDrawAddColor(0, 0, 0) :
				SetDrawAddColor(-13, -92, -70);
			DrawBox(
				Sw - MARGIN_W - BULLET_W,
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H - SKILL_FRAME_H - BULLET_MARGIN_H - BULLET_H - i * BULLET_H,
				Sw - MARGIN_W,
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H - SKILL_FRAME_H - BULLET_MARGIN_H - i * BULLET_H,
				0x0def80, TRUE);
		}
		SetDrawAddColor(0, 0, 0);

		// コア
		for (int i = 0; i < cPlayer::MaxCoreNum; ++i)
			DrawCircle(Sw - 94 - i * 24, Sh - 40, 8, 0xffffff, i < m_pPlayer[1]->CoreNum);

		// スキルアイコン
		SetDrawAddColor(0, 0, 160);
		DrawExtendGraph(
			Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - SKILL_FRAME_W,
			Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H - SKILL_FRAME_H,
			Sw - MARGIN_W,
			Sh - MARGIN_H,
			m_Image_IconFrame, FALSE);
		SetDrawAddColor(0, 0, 0);
		(m_pPlayer[1]->Skill != eSKILL::MAX) ?
			DrawExtendGraph(
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W,
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H,
				Sw - MARGIN_W - SKILL_FRAME_W,
				Sh - MARGIN_H - SKILL_FRAME_H,
				cCommonData::GetIcon_Skill(m_pPlayer[1]->Skill), TRUE) :
			DrawExtendGraph(
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W,
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H,
				Sw - MARGIN_W - SKILL_FRAME_W,
				Sh - MARGIN_H - SKILL_FRAME_H,
				m_Image_SkillIcon_None, TRUE);

		if (m_pPlayer[1]->Skill != eSKILL::MAX && m_pPlayer[1]->SkillCoolTime > 0) {
			float angle = DX_PI_F * 2.0f * m_pPlayer[1]->SkillCoolTime / m_pPlayer[1]->MaxSkillCoolTime;
			SetDrawScreen(m_Screen_SkillCoolTime);
			ClearDrawScreen();
			if (angle > DX_PI_F / 2.0f * 4.0f)	DrawBox(12, 0, 24, 12, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 3.0f)	DrawBox(12, 12, 24, 24, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 2.0f)	DrawBox(0, 12, 12, 24, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 1.0f)	DrawBox(0, 0, 12, 12, 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 4.0f && angle > DX_PI_F / 2.0f * 3.0f)
				DrawTriangle(12, 12, 36, 12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 3.0f && angle > DX_PI_F / 2.0f * 2.0f)
				DrawTriangle(12, 12, 12, 36, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 2.0f && angle > DX_PI_F / 2.0f * 1.0f)
				DrawTriangle(12, 12, -12, 12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 1.0f && angle > DX_PI_F / 2.0f * 0.0f)
				DrawTriangle(12, 12, 12, -12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			cSceneMgr::GetInstance().ChangeScreen(eSCREEN::DEFAULT);

			SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
			DrawExtendGraph(
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W,
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H,
				Sw - MARGIN_W - SKILL_FRAME_W,
				Sh - MARGIN_H - SKILL_FRAME_H,
				m_Screen_SkillCoolTime, FALSE);
			SetDrawBlendMode(DX_BLENDMODE_ADD, (int)(m_SkillEffectTimer[1] * 255.0f / MAX_RUNSKILL_TIME));
			DrawBox(
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[1] / MAX_RUNSKILL_TIME) * 30.0f),
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H - (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[1] / MAX_RUNSKILL_TIME) * 30.0f),
				Sw - MARGIN_W - SKILL_FRAME_W + (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[1] / MAX_RUNSKILL_TIME) * 30.0f),
				Sh - MARGIN_H - SKILL_FRAME_H + (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[1] / MAX_RUNSKILL_TIME) * 30.0f),
				0xffffff, FALSE);
			DrawRotaGraph(
				m_ScreenDif.x + m_SkillEffectPos[1].x,
				m_ScreenDif.y + m_SkillEffectPos[1].y,
				1.0 + ((long long)MAX_RUNSKILL_TIME - m_SkillEffectTimer[1]) / 5.0, 0.0, cCommonData::GetIcon_Skill(m_pPlayer[1]->Skill), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	// 3Pステータス
	if (cCommonData::Player4Flag)
	{
		// HP
		DrawBox(
			Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - SKILL_FRAME_W - HP_FRAME_W - (HP_W + HP_FRAME_W) * HP_GAUGE_NUM,
			MARGIN_H,
			Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - SKILL_FRAME_W,
			MARGIN_H + HP_FRAME_H + HP_H + HP_FRAME_H,
			0xffffffff, TRUE);
		for (int i = 0; i < HP_GAUGE_NUM; ++i) {
			int hp_num = m_pPlayer[2]->HP / HP_GAUGE_NUM + (m_pPlayer[2]->HP % HP_GAUGE_NUM > i);
			DrawBox(
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - SKILL_FRAME_W - HP_FRAME_W - HP_W - i * 32,
				MARGIN_H + HP_FRAME_H,
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - SKILL_FRAME_W - HP_FRAME_W - i * 32,
				MARGIN_H + HP_FRAME_H + HP_H,
				(hp_num == 0) ? 0x000000 : (hp_num == 1) ? 0x00ff00 : (hp_num == 2) ? 0xffd000 : (hp_num == 3) ? 0xe02040 : 0xe0e0e0, TRUE);
		}

		// 弾数
		for (int i = 0; i < m_pPlayer[2]->MaxBulletNum; ++i) {
			((m_pPlayer[2]->MaxBulletNum - i) <= m_pPlayer[2]->BulletNum) ?
				SetDrawAddColor(0, 0, 0) :
				SetDrawAddColor(-13, -92, -70);
			DrawBox(
				Sw - MARGIN_W - BULLET_W,
				MARGIN_H + SKILL_FRAME_H + SKILL_H + SKILL_FRAME_H + BULLET_MARGIN_H + i * BULLET_H,
				Sw - MARGIN_W,
				MARGIN_H + SKILL_FRAME_H + SKILL_H + SKILL_FRAME_H + BULLET_MARGIN_H + BULLET_H + i * BULLET_H,
				0x0def80, TRUE);
		}
		SetDrawAddColor(0, 0, 0);

		// コア
		for (int i = 0; i < cPlayer::MaxCoreNum; ++i)
			DrawCircle(Sw - 94 - i * 24, 40, 8, 0xffffff, i < m_pPlayer[2]->CoreNum);

		// スキルアイコン
		SetDrawAddColor(0, 160, 0);
		DrawExtendGraph(
			Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - SKILL_FRAME_W,
			MARGIN_H,
			Sw - MARGIN_W,
			MARGIN_H + SKILL_FRAME_H + SKILL_H + SKILL_FRAME_H,
			m_Image_IconFrame, FALSE);
		SetDrawAddColor(0, 0, 0);
		(m_pPlayer[2]->Skill != eSKILL::MAX) ?
			DrawExtendGraph(
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W,
				MARGIN_H + SKILL_FRAME_H,
				Sw - MARGIN_W - SKILL_FRAME_W,
				MARGIN_H + SKILL_FRAME_H + SKILL_H,
				cCommonData::GetIcon_Skill(m_pPlayer[2]->Skill), TRUE) :
			DrawExtendGraph(
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W,
				MARGIN_H + SKILL_FRAME_H,
				Sw - MARGIN_W - SKILL_FRAME_W,
				MARGIN_H + SKILL_FRAME_H + SKILL_H,
				m_Image_SkillIcon_None, TRUE);

		if (m_pPlayer[2]->Skill != eSKILL::MAX && m_pPlayer[2]->SkillCoolTime > 0) {
			float angle = DX_PI_F * 2.0f * m_pPlayer[2]->SkillCoolTime / m_pPlayer[2]->MaxSkillCoolTime;
			SetDrawScreen(m_Screen_SkillCoolTime);
			ClearDrawScreen();
			if (angle > DX_PI_F / 2.0f * 4.0f)	DrawBox(12, 0, 24, 12, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 3.0f)	DrawBox(12, 12, 24, 24, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 2.0f)	DrawBox(0, 12, 12, 24, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 1.0f)	DrawBox(0, 0, 12, 12, 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 4.0f && angle > DX_PI_F / 2.0f * 3.0f)
				DrawTriangle(12, 12, 36, 12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 3.0f && angle > DX_PI_F / 2.0f * 2.0f)
				DrawTriangle(12, 12, 12, 36, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 2.0f && angle > DX_PI_F / 2.0f * 1.0f)
				DrawTriangle(12, 12, -12, 12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 1.0f && angle > DX_PI_F / 2.0f * 0.0f)
				DrawTriangle(12, 12, 12, -12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			cSceneMgr::GetInstance().ChangeScreen(eSCREEN::DEFAULT);

			SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
			DrawExtendGraph(
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W,
				MARGIN_H + SKILL_FRAME_H,
				Sw - MARGIN_W - SKILL_FRAME_W,
				MARGIN_H + SKILL_FRAME_H + SKILL_H,
				m_Screen_SkillCoolTime, FALSE);
			SetDrawBlendMode(DX_BLENDMODE_ADD, (int)(m_SkillEffectTimer[2] * 255.0f / MAX_RUNSKILL_TIME));
			DrawBox(
				Sw - MARGIN_W - SKILL_FRAME_W - SKILL_W - (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[2] / MAX_RUNSKILL_TIME) * 30.0f),
				MARGIN_H + SKILL_FRAME_H - (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[2] / MAX_RUNSKILL_TIME) * 30.0f),
				Sw - MARGIN_W - SKILL_FRAME_W + (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[2] / MAX_RUNSKILL_TIME) * 30.0f),
				MARGIN_H + SKILL_FRAME_H + SKILL_H + (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[2] / MAX_RUNSKILL_TIME) * 30.0f),
				0xffffff, FALSE);
			DrawRotaGraph(
				m_ScreenDif.x + m_SkillEffectPos[2].x,
				m_ScreenDif.y + m_SkillEffectPos[2].y,
				1.0 + ((long long)MAX_RUNSKILL_TIME - m_SkillEffectTimer[2]) / 5.0, 0.0, cCommonData::GetIcon_Skill(m_pPlayer[2]->Skill), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	// 4Pステータス
	if (cCommonData::Player4Flag)
	{
		// HP
		DrawBox(
			MARGIN_W + SKILL_FRAME_W + SKILL_W + SKILL_FRAME_W,
			Sh - MARGIN_H - HP_FRAME_H - HP_H - HP_FRAME_H,
			MARGIN_W + SKILL_FRAME_W + SKILL_W + SKILL_FRAME_W + HP_FRAME_W + (HP_W + HP_FRAME_W) * HP_GAUGE_NUM,
			Sh - MARGIN_H,
			0xffffffff, TRUE);
		for (int i = 0; i < HP_GAUGE_NUM; ++i) {
			int hp_num = m_pPlayer[3]->HP / HP_GAUGE_NUM + (m_pPlayer[3]->HP % HP_GAUGE_NUM > i);
			DrawBox(
				MARGIN_W + SKILL_FRAME_W + SKILL_W + SKILL_FRAME_W + HP_FRAME_W + i * 32,
				Sh - MARGIN_H - HP_FRAME_H - HP_H,
				MARGIN_W + SKILL_FRAME_W + SKILL_W + SKILL_FRAME_W + HP_FRAME_W + HP_W + i * 32,
				Sh - MARGIN_H - HP_FRAME_H,
				(hp_num == 0) ? 0x000000 : (hp_num == 1) ? 0x00ff00 : (hp_num == 2) ? 0xffd000 : (hp_num == 3) ? 0xe02040 : 0xe0e0e0, TRUE);
		}

		// 弾数
		for (int i = 0; i < m_pPlayer[3]->MaxBulletNum; ++i) {
			((m_pPlayer[3]->MaxBulletNum - i) <= m_pPlayer[3]->BulletNum) ?
				SetDrawAddColor(0, 0, 0) :
				SetDrawAddColor(-13, -92, -70);
			DrawBox(
				MARGIN_W,
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H - SKILL_FRAME_H - BULLET_MARGIN_H - BULLET_H - i * BULLET_H,
				MARGIN_W + BULLET_W,
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H - SKILL_FRAME_H - BULLET_MARGIN_H - i * BULLET_H,
				0x0def80, TRUE);
		}
		SetDrawAddColor(0, 0, 0);

		// コア
		for (int i = 0; i < cPlayer::MaxCoreNum; ++i)
			DrawCircle(94 + i * 24, Sh - 40, 8, 0xffffff, i < m_pPlayer[3]->CoreNum);

		// スキルアイコン
		SetDrawAddColor(160, 160, 0);
		DrawExtendGraph(
			MARGIN_W,
			Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H - SKILL_FRAME_H,
			MARGIN_W + SKILL_FRAME_W + SKILL_W + SKILL_FRAME_W,
			Sh - MARGIN_H,
			m_Image_IconFrame, FALSE);
		SetDrawAddColor(0, 0, 0);
		(m_pPlayer[3]->Skill != eSKILL::MAX) ?
			DrawExtendGraph(
				MARGIN_W + SKILL_FRAME_W,
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H,
				MARGIN_W + SKILL_FRAME_W + SKILL_W,
				Sh - MARGIN_H - SKILL_FRAME_H,
				cCommonData::GetIcon_Skill(m_pPlayer[3]->Skill), TRUE) :
			DrawExtendGraph(
				MARGIN_W + SKILL_FRAME_W,
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H,
				MARGIN_W + SKILL_FRAME_W + SKILL_W,
				Sh - MARGIN_H - SKILL_FRAME_H,
				m_Image_SkillIcon_None, TRUE);

		if (m_pPlayer[3]->Skill != eSKILL::MAX && m_pPlayer[3]->SkillCoolTime > 0) {
			float angle = DX_PI_F * 2.0f * m_pPlayer[3]->SkillCoolTime / m_pPlayer[3]->MaxSkillCoolTime;
			SetDrawScreen(m_Screen_SkillCoolTime);
			ClearDrawScreen();
			if (angle > DX_PI_F / 2.0f * 4.0f)	DrawBox(12, 0, 24, 12, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 3.0f)	DrawBox(12, 12, 24, 24, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 2.0f)	DrawBox(0, 12, 12, 24, 0xa0a0a0, TRUE);
			if (angle > DX_PI_F / 2.0f * 1.0f)	DrawBox(0, 0, 12, 12, 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 4.0f && angle > DX_PI_F / 2.0f * 3.0f)
				DrawTriangle(12, 12, 36, 12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 3.0f && angle > DX_PI_F / 2.0f * 2.0f)
				DrawTriangle(12, 12, 12, 36, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 2.0f && angle > DX_PI_F / 2.0f * 1.0f)
				DrawTriangle(12, 12, -12, 12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			if (angle <= DX_PI_F / 2.0f * 1.0f && angle > DX_PI_F / 2.0f * 0.0f)
				DrawTriangle(12, 12, 12, -12, 12 + (int)(cosf(-DX_PI_F / 2.0f - angle) * 24.0f), 12 + (int)(sinf(-DX_PI_F / 2.0f - angle) * 24.0f), 0xa0a0a0, TRUE);
			cSceneMgr::GetInstance().ChangeScreen(eSCREEN::DEFAULT);

			SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
			DrawExtendGraph(
				MARGIN_W + SKILL_FRAME_W,
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H,
				MARGIN_W + SKILL_FRAME_W + SKILL_W,
				Sh - MARGIN_H - SKILL_FRAME_H,
				m_Screen_SkillCoolTime, FALSE);
			SetDrawBlendMode(DX_BLENDMODE_ADD, (int)(m_SkillEffectTimer[3] * 255.0f / MAX_RUNSKILL_TIME));
			DrawBox(
				MARGIN_W + SKILL_FRAME_W - (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[3] / MAX_RUNSKILL_TIME) * 30.0f),
				Sh - MARGIN_H - SKILL_FRAME_H - SKILL_H - (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[3] / MAX_RUNSKILL_TIME) * 30.0f),
				MARGIN_W + SKILL_FRAME_W + SKILL_W + (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[3] / MAX_RUNSKILL_TIME) * 30.0f),
				Sh - MARGIN_H - SKILL_FRAME_H + (int)(cosf(DX_PI_F / 2.0f * m_SkillEffectTimer[3] / MAX_RUNSKILL_TIME) * 30.0f),
				0xffffff, FALSE);
			DrawRotaGraph(
				m_ScreenDif.x + m_SkillEffectPos[3].x,
				m_ScreenDif.y + m_SkillEffectPos[3].y,
				1.0 + ((long long)MAX_RUNSKILL_TIME - m_SkillEffectTimer[3]) / 5.0, 0.0, cCommonData::GetIcon_Skill(m_pPlayer[3]->Skill), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	// 制限時間
	if (m_Timer > 0) {
		if (m_Timer < MAX_TIME_LIMIT)
			DrawFormatString(Sw / 2 - 8, 16, 0xffffff, "%d", (MAX_TIME_LIMIT - m_Timer + 59) / 60);
		else if (!m_FinishBattleFlag)
			DrawFormatString(Sw / 2 - 52, 16, 0xff0000, "-SuddenDeath-");
	}

	// 開始演出
	if (m_Timer <= -START_FLASH_TIME) {
		int param = 255 - (MAX_START_TIME + m_Timer) * 255 / START_FLASH_TIME;
		param = __max(param, 0);
		SetDrawBlendMode(DX_BLENDMODE_ADD, param);
		DrawBox(0, 0, Sw, Sh, 0xffffff, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (m_Timer >= -90 && m_Timer < -50) {
		int img_id;
		if (m_Timer < -80)		img_id = (int)floorf((m_Timer + 90) / 10.0f * 3.0f);
		else if (m_Timer < -60)	img_id = 3;
		else					img_id = 3 - (int)ceilf((m_Timer + 60) / 10.0f * 3.0f);
		DrawRotaGraph(Sw / 2 - 120, Sh / 2, 2.0, 0.0, m_Image_CountDown[img_id], TRUE);
		DrawRotaGraph(Sw / 2 + 120, Sh / 2, 2.0, 0.0, m_Image_CountDown[img_id], TRUE);
	}
	if (m_Timer >= -60 && m_Timer < -20) {
		int img_id;
		if (m_Timer < -50)		img_id = (int)floorf((m_Timer + 60) / 10.0f * 3.0f);
		else if (m_Timer < -30)	img_id = 3;
		else					img_id = 3 - (int)ceilf((m_Timer + 30) / 10.0f * 3.0f);
		DrawRotaGraph(Sw / 2 - 96, Sh / 2, 2.0, 0.0, m_Image_CountDown[img_id], TRUE);
		DrawRotaGraph(Sw / 2 + 96, Sh / 2, 2.0, 0.0, m_Image_CountDown[img_id], TRUE);
	}
	if (m_Timer >= -30 && m_Timer < 10) {
		int img_id;
		if (m_Timer < -20)		img_id = (int)floorf((m_Timer + 30) / 10.0f * 3.0f);
		else if (m_Timer < 0)	img_id = 3;
		else					img_id = 3 - (int)ceilf((m_Timer) / 10.0f * 3.0f);
		DrawRotaGraph(Sw / 2 - 72, Sh / 2, 2.0, 0.0, m_Image_CountDown[img_id], TRUE);
		DrawRotaGraph(Sw / 2 + 72, Sh / 2, 2.0, 0.0, m_Image_CountDown[img_id], TRUE);
	}
	if (m_Timer >= -90 && m_Timer < 0) {
		int img_id;
		if (m_Timer < -80)	img_id = (int)floorf((m_Timer + 90) / 10.0f * 3.0f);
		else				img_id = 3;
		DrawRotaGraph(Sw / 2, Sh / 2, 2.0, 0.0, m_Image_Ready[img_id], TRUE);
	}
	if (m_Timer >= 0 && m_Timer < 70) {
		int img_id;
		if (m_Timer < 60)	img_id = 3;
		else				img_id = 3 - (int)ceilf((m_Timer - 60) / 10.0f * 3.0f);
		DrawRotaGraph(Sw / 2, Sh / 2, 2.0, 0.0, m_Image_Fight[img_id], TRUE);
	}
	if (m_Timer == -90 || m_Timer == -60 || m_Timer == -30)
		cSoundData::GetInstance().Play(eSOUND::COWNTDOWN);
	if (m_Timer == 0) {
		cEffectMgr::GetInstance().Create_Dead(VEC2F(Sw / 2.0f, Sh / 2.0f) - m_ScreenDif, 0xbbddff);
		cSoundData::GetInstance().Play(eSOUND::DESTROY);
	}

	DrawString(0, 0, "[Game]", 0xffffff);
}

/* 地震エフェクト */
void cGame::ShakeEffect(int time)
{
	if (time < 0) {
		m_ShakeTimer = 0;
		return;
	}

	m_ShakeTimer = time;
	m_ShakeAngle += DX_PI_F / 2.0f + DX_PI_F * (GetRand(10000) / 10000.0f);

	for (int i = 0; i < 2 + cCommonData::Player4Flag * 2; ++i)
		if ((int)cCommonData::InputDevice[i] < 16)
			StartJoypadVibration((int)cCommonData::InputDevice[i] + 1, 1000, time * 6);
}

/* 最寄りの他プレイヤーを取得 */
cPlayer* cGame::GetNearestOtherPlayer(cPlayer* self)
{
	if (!self)
		return nullptr;

	cPlayer* target = nullptr;
	float min_distance;

	for (auto& i : m_pPlayer) {
		if (i == self)
			continue;
		if (i->IsRepair() || i->IsDead())
			continue;

		if (target) {
			float distance = VEC2F::Dist(self->Pos, i->Pos);
			if (min_distance > distance) {
				min_distance = distance;
				target = i;
			}
		}
		else {
			min_distance = VEC2F::Dist(self->Pos, i->Pos);
			target = i;
		}
	}

	return target;
}
