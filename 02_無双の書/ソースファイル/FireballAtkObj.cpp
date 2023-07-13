// プリプロセス
#include "FireballAtkObj.h"
#include "DxLib.h"
#include "Easing.h"
#include "EnemyMgr.h"
#include "Game.h"
#include "EffectMgr.h"

// 定数の宣言
#define MAX_TIME			(90)
#define HIT_RADIUS			(40.0f)
#define PARTICLE_INTERVAL	(8)

// 静的メンバ変数の宣言
bool cFireballAtkObj::m_LoadedResourceFlag(false);
int cFireballAtkObj::m_Image_Ball(-1);
int cFireballAtkObj::m_Sound_Shot(-1);
int cFireballAtkObj::m_Sound_Bound(-1);

// リソースの読み込み
void cFireballAtkObj::LoadResource()
{
	if (m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = true;
	m_Image_Ball = LoadGraph("data\\fireball\\ball.png");
	m_Sound_Shot = LoadSoundMem("data\\sound\\fireball_shot.wav");
	m_Sound_Bound = LoadSoundMem("data\\sound\\bound.wav");
}

// リソースの解放
void cFireballAtkObj::DeleteResource()
{
	if (!m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = false;
	DeleteGraph(m_Image_Ball);
	DeleteSoundMem(m_Sound_Shot);
	DeleteSoundMem(m_Sound_Bound);
}

// コンストラクタ
cFireballAtkObj::cFireballAtkObj(VEC2F pos, bool inversion)
	: cBaseAtkObj(pos, inversion)
	, m_Angle(DX_PI_F / 180.0f * GetRand(359))
	, m_ParticleTimer(0)
{
	PlaySoundMem(m_Sound_Shot, DX_PLAYTYPE_BACK);
}

// 更新
void cFireballAtkObj::Update()
{
	// 移動処理
	if (!m_Inversion)
	{
		m_Pos += VEC2F(3.0f, -3.0f);
		if (m_Pos.x > (float)FLOOR_X2)
		{
			m_Pos.x = (float)FLOOR_X2;
			m_Inversion = true;
			cGame::GetInstance().ShakeEffect(20);
			PlaySoundMem(m_Sound_Bound, DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		m_Pos += VEC2F(-3.0f, -3.0f);
		if (m_Pos.x < (float)FLOOR_X1)
		{
			m_Pos.x = (float)FLOOR_X1;
			m_Inversion = false;
			cGame::GetInstance().ShakeEffect(20);
			PlaySoundMem(m_Sound_Bound, DX_PLAYTYPE_BACK);
		}
	}

	// 敵との衝突
	for (auto& i : cEnemyMgr::GetInstance().GetEnemyList())
	{
		float distance = VEC2F::Dist(m_Pos, i->GetPos());
		if (distance >= HIT_RADIUS + i->GetRadius())
			continue;
		if (std::find(m_pHitEnemy.begin(), m_pHitEnemy.end(), i) != m_pHitEnemy.end())
			continue;

		float angle = atan2f(i->GetPos().y - m_Pos.y, i->GetPos().x - m_Pos.x);
		i->AddMotion(VEC2F(cosf(angle), sinf(angle)) * 2.0f);
		i->TakenDamage(1);
		m_pHitEnemy.push_back(i);
		cGame::GetInstance().ShakeEffect(10);
	}

	// 回転
	m_Angle += 0.04f;

	// パーティクルの生成
	if ((++m_ParticleTimer %= PARTICLE_INTERVAL) == 0)
		cEffectMgr::GetInstance().CreateEffect(eEFFECT::PARTICLE, m_Pos, m_Inversion ? DX_PI_F / 4.0f : DX_PI_F / 4.0f * 3.0f, GetRand(300) / 100.0f);
}

// 描画
void cFireballAtkObj::Draw() const
{
	DrawRotaGraph(
		(int)m_Pos.x,
		(int)m_Pos.y,
		4.0, (double)m_Angle, m_Image_Ball, TRUE);
}

// アクティブ状態
bool cFireballAtkObj::IsActive() const
{
	return m_Pos.y > -200.0f;
}
