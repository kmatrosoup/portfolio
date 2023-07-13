// プリプロセス
#include "Player.h"
#include "DxLib.h"
#include "Game.h"
#include "InputMgr.h"
#include "BookMgr.h"
#include "EnemyMgr.h"
#include "EffectMgr.h"

// 定数の宣言
#define CURSOR_RADIUS				(80.0f)	// 本検知範囲
#define PICKUP_TIME					(30)	// 本を持つ時間
#define USE_TIME					(12)	// 本を使う時間
#define DAMAGE_RADIUS				(20.0f)	// 被ダメ範囲
#define HOLD_SWING_TIME				(40)	// 所持中の本の揺れ時間
#define HEAD_ANIMATION_TIME			(20)	// 頭のアニメーション時間
#define BODY_ANIMATION_TIME			(12)	// 体のアニメーション時間
#define GUIDE_ANIMATION_TIME		(30)	// 操作ガイドのアニメーション時間
#define SHOW_MOVEMENT_GUIDE_TIME	(300)	// 移動操作ガイドの表示時間
#define SHOW_HOLD_GUIDE_TIME		(300)	// 本取得ガイドの表示時間
#define SHOW_USE_GUIDE_TIME			(480)	// 本使用ガイドの表示時間

// 静的メンバ変数の宣言
cPlayer* cPlayer::m_pInstance(nullptr);
int cPlayer::m_Image_Head[2]{};
int cPlayer::m_Image_Body[3]{};
int cPlayer::m_Image_Grave(-1);
int cPlayer::m_Image_Cursor[13]{};
int cPlayer::m_Image_MovementGuide[2]{};
int cPlayer::m_Image_HoldGuide[2]{};
int cPlayer::m_Image_UseGuide[2]{};
int cPlayer::m_Sound_Pickup(-1);

// インスタンスを生成
void cPlayer::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cPlayer;

	LoadDivGraph("data\\player_head.png", 2, 2, 1, 24, 24, m_Image_Head);
	LoadDivGraph("data\\player_body.png", 3, 3, 1, 24, 24, m_Image_Body);
	m_Image_Grave = LoadGraph("data\\grave.png");
	LoadDivGraph("data\\cursor.png", 13, 13, 1, 20, 20, m_Image_Cursor);
	LoadDivGraph("data\\guide_movement.png", 2, 2, 1, 29, 30, m_Image_MovementGuide);
	LoadDivGraph("data\\guide_hold.png", 2, 2, 1, 15, 19, m_Image_HoldGuide);
	LoadDivGraph("data\\guide_use.png", 2, 2, 1, 15, 13, m_Image_UseGuide);
	m_Sound_Pickup = LoadSoundMem("data\\sound\\pickup.wav");
}

// インスタンスを解放
void cPlayer::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;

	for (int i = 0; i < 2; ++i)		DeleteGraph(m_Image_Head[i]);
	for (int i = 0; i < 3; ++i)		DeleteGraph(m_Image_Body[i]);
	for (int i = 0; i < 13; ++i)	DeleteGraph(m_Image_Cursor[i]);
	DeleteGraph(m_Image_Grave);
	for (int i = 0; i < 2; ++i)		DeleteGraph(m_Image_MovementGuide[i]);
	for (int i = 0; i < 2; ++i)		DeleteGraph(m_Image_HoldGuide[i]);
	for (int i = 0; i < 2; ++i)		DeleteGraph(m_Image_UseGuide[i]);
	DeleteSoundMem(m_Sound_Pickup);
}

// インスタンスを取得
cPlayer& cPlayer::GetInstance()
{
	return *m_pInstance;
}

// コンストラクタ
cPlayer::cPlayer()
	: m_Pos(FLOOR_X1 + 100.0f, FLOOR_Y2 - 60.0f)
	, m_Motion(0.0f, 0.0f)
	, m_InversionFlag(false)
	, m_pCursor(nullptr)
	, m_pHoldBook(nullptr)
	, m_PickupTimer(0)
	, m_UseTimer(0)
	, m_DeadFlag(false)
	, m_HoldSwingTimer(0)
	, m_HoldAngle(DX_PI_F / 2.0f)
	, m_HeadAnimationTimer(0)
	, m_BodyAnimationTimer(-1)
	, m_GuideAnimationTimer(0)
	, m_MovementGuide_Alpha(0)
	, m_ShowMovementGuideTimer(SHOW_MOVEMENT_GUIDE_TIME)
	, m_HoldGuide_Alpha(0)
	, m_ShowHoldGuideTimer(SHOW_HOLD_GUIDE_TIME)
	, m_UseGuide_Alpha(0)
	, m_ShowUseGuideTimer(SHOW_USE_GUIDE_TIME)
{
}

// デストラクタ
cPlayer::~cPlayer()
{
	if (m_pHoldBook)
		delete m_pHoldBook;
}

// 初期化
void cPlayer::Init()
{
	m_PickupTimer = 0;
	m_UseTimer = 0;
	m_DeadFlag = false;

	if (m_pHoldBook)
	{
		delete m_pHoldBook;
		m_pHoldBook = nullptr;
	}
}

// 更新
void cPlayer::Update()
{
	{
		int inputX = (InputMgr::Keep(eINPUT::RIGHT) - InputMgr::Keep(eINPUT::LEFT));
		int inputY = (InputMgr::Keep(eINPUT::DOWN) - InputMgr::Keep(eINPUT::UP));

		// 移動操作ガイドの表示設定
		(inputX || inputY) ?
			m_ShowMovementGuideTimer = 0 :
			m_ShowMovementGuideTimer = min(m_ShowMovementGuideTimer + 1, SHOW_MOVEMENT_GUIDE_TIME);

		if (m_DeadFlag)
			inputX = inputY = 0;

		// 移動量計算
		float move_speed = 0.8f;
		if (m_PickupTimer > 0)
			move_speed *= 0.2f;
		m_Motion.x += inputX * move_speed;
		m_Motion.y += inputY * move_speed;
		m_Motion *= 0.85f;

		// 向きの制御
		switch (inputX)
		{
		case -1:	m_InversionFlag = true;		break;
		case 1:		m_InversionFlag = false;	break;
		}

		// 腕振り
		(inputX || inputY) ?
			++m_HoldSwingTimer %= HOLD_SWING_TIME :
			m_HoldSwingTimer = 0;

		// 体のアニメーション
		(inputX || inputY) ?
			++m_BodyAnimationTimer %= (BODY_ANIMATION_TIME * 2) :
			m_BodyAnimationTimer = -1;

		// 足跡のエフェクト
		if (m_BodyAnimationTimer == 0)
			cEffectMgr::GetInstance().CreateEffect(eEFFECT::FOOT, m_Pos);
	}

	// プレイヤー移動処理
	m_Pos += m_Motion;
	if (m_Motion.x < 0.0f && m_Pos.x < (float)FLOOR_X1)
	{
		m_Pos.x = (float)FLOOR_X1;
		m_Motion.x = 0.0f;
	}
	if (m_Motion.x > 0.0f && m_Pos.x > (float)FLOOR_X2)
	{
		m_Pos.x = (float)FLOOR_X2;
		m_Motion.x = 0.0f;
	}
	if (m_Motion.y < 0.0f && m_Pos.y < (float)FLOOR_Y1)
	{
		m_Pos.y = (float)FLOOR_Y1;
		m_Motion.y = 0.0f;
	}
	if (m_Motion.y > 0.0f && m_Pos.y > (float)FLOOR_Y2)
	{
		m_Pos.y = (float)FLOOR_Y2;
		m_Motion.y = 0.0f;
	}

	if (m_PickupTimer == 0)
	{
		// 範囲内にある最寄りの本にカーソルを合わせる
		m_pCursor = nullptr;
		float min_distance = CURSOR_RADIUS;
		for (auto& i : cBookMgr::GetInstance().GetBookList())
		{
			float distance = VEC2F::Dist(m_Pos, i->GetPos());
			if (min_distance > distance)
			{
				min_distance = distance;
				m_pCursor = i;
			}
		}

		// 本を選択
		if (m_pCursor && InputMgr::In(eINPUT::DECIDE))
			++m_PickupTimer;
	}
	else
	{
		if (!cBookMgr::GetInstance().ExistBook(m_pCursor))
		{
			m_pCursor = nullptr;
			m_PickupTimer = 0;
		}
		else
		{
			if (InputMgr::Keep(eINPUT::DECIDE))
			{
				// 本を拾う
				if (++m_PickupTimer >= PICKUP_TIME)
				{
					// 既に本を持っていたら捨てる
					if (m_pHoldBook)
					{
						m_pHoldBook;
						m_pHoldBook->SetPos(m_Pos);
						float angle = DX_PI_F / 180.0f * GetRand(359);
						m_pHoldBook->SetMotion(VEC2F(cosf(angle), sinf(angle)) * (float)(10 + GetRand(5)));
						cBookMgr::GetInstance().AddBook(m_pHoldBook);
						m_pHoldBook = nullptr;
					}
					cBookMgr::GetInstance().RemoveBook(m_pCursor);
					m_pHoldBook = m_pCursor;
					m_pCursor = nullptr;
					m_PickupTimer = 0;
					PlaySoundMem(m_Sound_Pickup, DX_PLAYTYPE_BACK);
				}
			}
			else
			{
				m_PickupTimer = 0;
			}
		}
	}

	// 所持中の本の処理
	if (m_pHoldBook)
	{
		float target_angle = 0.6f - sinf(DX_PI_F * 2.0f * m_HoldSwingTimer / HOLD_SWING_TIME) * 0.5f;
		if (m_InversionFlag)
			target_angle = DX_PI_F - target_angle;
		m_HoldAngle += (target_angle - m_HoldAngle) * 0.4f;
		m_pHoldBook->SetPos(m_Pos + VEC2F(cosf(m_HoldAngle), sinf(m_HoldAngle)) * 20.0f + VEC2F(0.0f, -20.0f));
		m_pHoldBook->SetAngle(m_HoldAngle + DX_PI_F * m_InversionFlag);
		m_pHoldBook->SetInversion(m_InversionFlag);

		// 本を使用
		if (InputMgr::Keep(eINPUT::DECIDE))
		{
			if (InputMgr::In(eINPUT::DECIDE))
				++m_UseTimer;
			else if (m_UseTimer > 0 && m_UseTimer < USE_TIME)
				++m_UseTimer;
		}
		if (InputMgr::Out(eINPUT::DECIDE) && m_UseTimer > 0)
		{
			if (m_UseTimer < USE_TIME)
			{
				m_pHoldBook->RunMagic();
				if (!m_pHoldBook->IsActive())
				{
					delete m_pHoldBook;
					m_pHoldBook = nullptr;
				}
				m_ShowUseGuideTimer = 0;
			}
			m_UseTimer = 0;
		}
	}
	else
		m_UseTimer = 0;

	// 敵との衝突
	if (!m_DeadFlag)
		for (auto& i : cEnemyMgr::GetInstance().GetEnemyList())
			if (VEC2F::Dist(m_Pos, i->GetPos()) < DAMAGE_RADIUS + i->GetRadius())
			{
				m_DeadFlag = true;
				m_Motion = VEC2F();
				m_pCursor = nullptr;
				if (m_pHoldBook)
				{
					delete m_pHoldBook;
					m_pHoldBook = nullptr;
				}
				cGame::GetInstance().ShakeEffect(80);
				for (int i = 0; i < 10; ++i)
				{
					float angle = DX_PI_F / 180.0f * GetRand(359);
					cEffectMgr::GetInstance().CreateEffect(eEFFECT::PARTICLE, m_Pos + VEC2F(cosf(angle), sinf(angle)) * 20.0f, angle, GetRand(2000) / 100.0f);
				}
			}

	// 頭のアニメーション
	++m_HeadAnimationTimer %= (HEAD_ANIMATION_TIME * 2);

	// ガイドアニメーション
	++m_GuideAnimationTimer %= (GUIDE_ANIMATION_TIME * 2);

	// 移動操作ガイド
	(m_ShowMovementGuideTimer == SHOW_MOVEMENT_GUIDE_TIME) ?
		m_MovementGuide_Alpha = min(m_MovementGuide_Alpha + 15, 255) :
		m_MovementGuide_Alpha = max(m_MovementGuide_Alpha - 15, 0);
	if (m_DeadFlag)
		m_MovementGuide_Alpha = 0;

	// 本取得ガイド
	(!m_pHoldBook) ?
		m_ShowHoldGuideTimer = min(m_ShowHoldGuideTimer + 1, SHOW_HOLD_GUIDE_TIME) :
		m_ShowHoldGuideTimer = 0;
	(m_ShowHoldGuideTimer == SHOW_HOLD_GUIDE_TIME) ?
		m_HoldGuide_Alpha = min(m_HoldGuide_Alpha + 15, 255) :
		m_HoldGuide_Alpha = max(m_HoldGuide_Alpha - 15, 0);
	if (!m_pCursor)
		m_HoldGuide_Alpha = 0;

	// 本使用ガイド
	m_ShowUseGuideTimer = min(m_ShowUseGuideTimer + 1, SHOW_USE_GUIDE_TIME);
	if (!m_pHoldBook)
		m_ShowUseGuideTimer = min(m_ShowUseGuideTimer, SHOW_USE_GUIDE_TIME - 1);
	(m_ShowUseGuideTimer == SHOW_USE_GUIDE_TIME) ?
		m_UseGuide_Alpha = min(m_UseGuide_Alpha + 15, 255) :
		m_UseGuide_Alpha = max(m_UseGuide_Alpha - 15, 0);
}

// 描画
void cPlayer::Draw() const
{
	// プレイヤー
	if (!m_DeadFlag)
	{
		DrawRotaGraph(
			(int)m_Pos.x,
			(int)m_Pos.y - 40,
			4.0, 0.0, m_Image_Body[(m_BodyAnimationTimer == -1) ? 0 : (m_BodyAnimationTimer / BODY_ANIMATION_TIME) + 1], TRUE, m_InversionFlag);
		DrawRotaGraph(
			(int)m_Pos.x,
			(int)m_Pos.y - 40,
			4.0, 0.0, m_Image_Head[m_HeadAnimationTimer / HEAD_ANIMATION_TIME], TRUE, m_InversionFlag);
	}

	// 墓
	else
	{
		DrawRotaGraph(
			(int)m_Pos.x,
			(int)m_Pos.y - 40,
			4.0, 0.0, m_Image_Grave, TRUE);
	}

	// 所持中の本
	if (m_pHoldBook)
		m_pHoldBook->Draw();

	// カーソル
	if (m_pCursor)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		if (m_PickupTimer == 0)
			DrawRotaGraph(
				(int)m_pCursor->GetPos().x,
				(int)m_pCursor->GetPos().y,
				4.0, 0.0, m_Image_Cursor[0], TRUE);
		else
			DrawRotaGraph(
				(int)m_pCursor->GetPos().x,
				(int)m_pCursor->GetPos().y,
				4.0, 0.0, m_Image_Cursor[m_PickupTimer * 12 / PICKUP_TIME + 1], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// 操作ガイド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_MovementGuide_Alpha);
	DrawRotaGraph(
		(int)m_Pos.x,
		(int)m_Pos.y - 160,
		4.0, 0.0, m_Image_MovementGuide[m_GuideAnimationTimer / GUIDE_ANIMATION_TIME], TRUE);

	// 本取得ガイド
	if (m_pCursor)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_HoldGuide_Alpha);
		DrawRotaGraph(
			(int)m_pCursor->GetPos().x,
			(int)m_pCursor->GetPos().y - 160,
			4.0, 0.0, m_Image_HoldGuide[m_GuideAnimationTimer / GUIDE_ANIMATION_TIME], TRUE);
	}

	// 本使用ガイド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_UseGuide_Alpha);
	DrawRotaGraph(
		(int)m_Pos.x,
		(int)m_Pos.y + 60,
		4.0, 0.0, m_Image_UseGuide[m_GuideAnimationTimer / GUIDE_ANIMATION_TIME], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 座標を取得
VEC2F cPlayer::GetPos() const
{
	return m_Pos;
}

// 死亡判定
bool cPlayer::IsDead() const
{
	return m_DeadFlag;
}
