// プリプロセス
#include "SpawnEffect.h"
#include "DxLib.h"

// 定数の宣言
#define MAX_TIME	(15)

// 静的メンバ変数の宣言
bool cSpawnEffect::m_LoadedResourceFlag(false);
int cSpawnEffect::m_Image_SpawnEffect[5]{};

// リソースの読み込み
void cSpawnEffect::LoadResource()
{
	if (m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = true;
	LoadDivGraph("data\\effect_spawn.png", 5, 5, 1, 8, 8, m_Image_SpawnEffect);
}

// リソースの解放
void cSpawnEffect::DeleteResource()
{
	if (!m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = false;
	for (int i = 0; i < 5; ++i)	DeleteGraph(m_Image_SpawnEffect[i]);
}

// コンストラクタ
cSpawnEffect::cSpawnEffect(VEC2F pos)
	: cBaseEffect(pos)
	, m_Timer(0)
	, m_Angle(DX_PI / 180.0 * GetRand(359))
{
}

// 更新
void cSpawnEffect::Update()
{
	if (m_Timer < MAX_TIME)
		++m_Timer;
}

// 描画
void cSpawnEffect::Draw() const
{
	DrawRotaGraph(
		(int)m_Pos.x,
		(int)m_Pos.y,
		12.0, m_Angle, m_Image_SpawnEffect[m_Timer * 5 / MAX_TIME], TRUE);
}

// アクティブ状態
bool cSpawnEffect::IsActive() const
{
	return m_Timer < MAX_TIME;
}
