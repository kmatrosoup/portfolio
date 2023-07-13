// プリプロセス
#include "ParticleEffect.h"
#include "DxLib.h"
#include <cmath>

// 定数の宣言
#define MAX_TIME	(24)

// 静的メンバ変数の宣言
bool cParticleEffect::m_LoadedResourceFlag(false);
int cParticleEffect::m_Image_ParticleEffect[4]{};

// リソースの読み込み
void cParticleEffect::LoadResource()
{
	if (m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = true;
	LoadDivGraph("data\\effect_particle.png", 4, 4, 1, 8, 8, m_Image_ParticleEffect);
}

// リソースの解放
void cParticleEffect::DeleteResource()
{
	if (!m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = false;
	for (int i = 0; i < 4; ++i)	DeleteGraph(m_Image_ParticleEffect[i]);
}

// コンストラクタ
cParticleEffect::cParticleEffect(VEC2F pos, float angle, float speed)
	: cBaseEffect(pos)
	, m_Timer(0)
	, m_Angle(DX_PI / 180.0 * GetRand(359))
	, m_Motion(VEC2F(cosf(angle), sinf(angle))* speed)
{
}

// 更新
void cParticleEffect::Update()
{
	if (m_Timer < MAX_TIME)
		++m_Timer;

	m_Motion *= 0.96f;
	m_Pos += m_Motion;
}

// 描画
void cParticleEffect::Draw() const
{
	DrawRotaGraph(
		(int)m_Pos.x,
		(int)m_Pos.y,
		4.0, m_Angle, m_Image_ParticleEffect[m_Timer * 4 / MAX_TIME], TRUE);
}

// アクティブ状態
bool cParticleEffect::IsActive() const
{
	return m_Timer < MAX_TIME;
}
