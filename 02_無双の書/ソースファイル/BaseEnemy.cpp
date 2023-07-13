// プリプロセス
#include "BaseEnemy.h"
#include "DxLib.h"
#include "Game.h"
#include "EnemyMgr.h"
#include "EffectMgr.h"

// 定数の宣言
#define ANIMATION_TIME	(16)	// アニメーション時間

// 静的メンバ変数の宣言
bool cBaseEnemy::m_LoadedResourceFlag(false);
int cBaseEnemy::m_Image_Enemy[(int)eENEMY::MAX][2]{};
int cBaseEnemy::m_Sound_Hit(-1);
int cBaseEnemy::m_Sound_Dead(-1);

// リソースの読み込み
void cBaseEnemy::LoadResource()
{
	if (m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = true;
	LoadDivGraph("data\\slime.png", 2, 2, 1, 24, 24, m_Image_Enemy[(int)eENEMY::SLIME]);
	LoadDivGraph("data\\bat.png", 2, 2, 1, 24, 24, m_Image_Enemy[(int)eENEMY::BAT]);
	LoadDivGraph("data\\skull.png", 2, 2, 1, 24, 24, m_Image_Enemy[(int)eENEMY::SKULL]);
	m_Sound_Hit = LoadSoundMem("data\\sound\\hit.wav");
	m_Sound_Dead = LoadSoundMem("data\\sound\\enemy_dead.wav");
}

// リソースの解放
void cBaseEnemy::DeleteResource()
{
	if (!m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = false;
	for (int i = 0; i < (int)eENEMY::MAX; ++i)
		for (int j = 0; j < 2; ++j)
			DeleteGraph(m_Image_Enemy[i][j]);
	DeleteSoundMem(m_Sound_Hit);
	DeleteSoundMem(m_Sound_Dead);
}

// コンストラクタ
cBaseEnemy::cBaseEnemy(eENEMY enemy, int hp, float radius, float x)
	: m_OwnType(enemy)
	, m_HP(hp)
	, m_Pos(x, 40.0f)
	, m_Motion(0.0f, 0.0f)
	, m_InversionFlag(false)
	, m_Radius(radius)
	, m_AnimationTimer(0)
{
}

// デストラクタ
cBaseEnemy::~cBaseEnemy()
{
}

// 更新
void cBaseEnemy::Update()
{
	// 固有処理
	m_OwnProcess();

	// 移動処理
	m_Motion *= 0.98f;
	m_Pos += m_Motion;
	if (m_Motion.x < 0.0f && m_Pos.x < (float)FLOOR_X1)
	{
		m_Pos.x = (float)FLOOR_X1;
		m_Motion.x = 0.0f;
	}
	if (m_Motion.y < 0.0f && m_Pos.y < 0.0f)
	{
		m_Pos.y = 0.0f;
		m_Motion.y = 0.0f;
	}
	if (m_Motion.x > 0.0f && m_Pos.x > (float)FLOOR_X2)
	{
		m_Pos.x = (float)FLOOR_X2;
		m_Motion.x = 0.0f;
	}
	if (m_Motion.y > 0.0f && m_Pos.y > (float)FLOOR_Y2)
	{
		m_Pos.y = (float)FLOOR_Y2;
		m_Motion.y = 0.0f;
	}

	// アニメーション
	++m_AnimationTimer %= (ANIMATION_TIME * 2);
}

// 描画
void cBaseEnemy::Draw() const
{
	DrawRotaGraph(
		(int)m_Pos.x,
		(int)m_Pos.y,
		4.0, 0.0, m_Image_Enemy[(int)m_OwnType][m_AnimationTimer / ANIMATION_TIME], TRUE, m_InversionFlag);
}

// アクティブ状態
bool cBaseEnemy::IsActive() const
{
	return m_HP > 0;
}

// 座標を取得
VEC2F cBaseEnemy::GetPos() const
{
	return m_Pos;
}

// 半径を取得
float cBaseEnemy::GetRadius() const
{
	return m_Radius;
}

// 移動量を加算
void cBaseEnemy::AddMotion(VEC2F motion)
{
	m_Motion += motion;
}

// 被ダメージ処理
void cBaseEnemy::TakenDamage(int value)
{
	if ((m_HP -= value) <= 0)
	{
		m_AddScore();
		for (int i = 0; i < 8; ++i)
			cEffectMgr::GetInstance().CreateEffect(eEFFECT::PARTICLE, m_Pos, DX_PI_F / 180.0f * GetRand(359), GetRand(1000) / 100.0f);
		PlaySoundMem(m_Sound_Hit, DX_PLAYTYPE_BACK);
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			cEffectMgr::GetInstance().CreateEffect(eEFFECT::PARTICLE, m_Pos, DX_PI_F / 180.0f * GetRand(359), GetRand(400) / 100.0f);
		PlaySoundMem(m_Sound_Dead, DX_PLAYTYPE_BACK);
	}
}
