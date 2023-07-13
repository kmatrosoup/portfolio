// プリプロセス
#include "ExplosionAtkObj.h"
#include "DxLib.h"
#include "Easing.h"
#include "EnemyMgr.h"
#include "Game.h"
#include "EffectMgr.h"

// 定数の宣言
#define MAX_TIME	(90)
#define HIT_RADIUS	(220.0f)

// 静的メンバ変数の宣言
bool cExplosionAtkObj::m_LoadedResourceFlag(false);
int cExplosionAtkObj::m_Image_Inner(-1);
int cExplosionAtkObj::m_Image_Outer(-1);
int cExplosionAtkObj::m_Sound_Explosion(-1);
int cExplosionAtkObj::m_Sound_ExplosionReady(-1);

// リソースの読み込み
void cExplosionAtkObj::LoadResource()
{
	if (m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = true;
	m_Image_Inner = LoadGraph("data\\explosion\\inner.png");
	m_Image_Outer = LoadGraph("data\\explosion\\outer.png");
	m_Sound_Explosion = LoadSoundMem("data\\sound\\explosion.wav");
	m_Sound_ExplosionReady = LoadSoundMem("data\\sound\\explosion_ready.wav");
}

// リソースの解放
void cExplosionAtkObj::DeleteResource()
{
	if (!m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = false;
	DeleteGraph(m_Image_Inner);
	DeleteGraph(m_Image_Outer);
	DeleteSoundMem(m_Sound_Explosion);
	DeleteSoundMem(m_Sound_ExplosionReady);
}

// コンストラクタ
cExplosionAtkObj::cExplosionAtkObj(VEC2F pos)
	: cBaseAtkObj(pos, false)
	, m_Timer(0)
{
	PlaySoundMem(m_Sound_ExplosionReady, DX_PLAYTYPE_BACK);
}

// 更新
void cExplosionAtkObj::Update()
{
	++m_Timer;

	if (m_Timer == 60)
	{
		for (auto& i : cEnemyMgr::GetInstance().GetEnemyList())
		{
			float distance = VEC2F::Dist(m_Pos, i->GetPos());
			if (distance >= HIT_RADIUS + i->GetRadius())
				continue;

			float angle = atan2f(i->GetPos().y - m_Pos.y, i->GetPos().x - m_Pos.x);
			i->TakenDamage(2);
			i->AddMotion(VEC2F(cosf(angle), sinf(angle)) * ((HIT_RADIUS + i->GetRadius()) - distance) / (HIT_RADIUS + i->GetRadius()) * 16.0f);
		}

		cGame::GetInstance().ShakeEffect(60);
		for (int i = 0; i < 30; ++i)
		{
			float angle = DX_PI_F / 180.0f * GetRand(359);
			cEffectMgr::GetInstance().CreateEffect(eEFFECT::PARTICLE, m_Pos + VEC2F(cosf(angle), sinf(angle)) * HIT_RADIUS, angle, GetRand(1000) / 100.0f);
		}
		PlaySoundMem(m_Sound_Explosion, DX_PLAYTYPE_BACK);
	}
}

// 描画
void cExplosionAtkObj::Draw() const
{
	if (m_Timer < 30)		SetDrawBlendMode(DX_BLENDMODE_ADD, (int)(Easing::Pow2::In(m_Timer / 30.0f) * 128.0f));
	else if (m_Timer < 60)	SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
	else					SetDrawBlendMode(DX_BLENDMODE_ADD, (int)((1.0f - Easing::Pow2::Out((m_Timer - 60) / 30.0f)) * 255.0f));
	DrawRotaGraph(
		(int)m_Pos.x,
		(int)m_Pos.y,
		4.0, m_Timer / 50.0, m_Image_Inner, TRUE);
	DrawRotaGraph(
		(int)m_Pos.x,
		(int)m_Pos.y,
		4.0, -m_Timer / 50.0, m_Image_Outer, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// アクティブ状態
bool cExplosionAtkObj::IsActive() const
{
	return m_Timer < MAX_TIME;
}
