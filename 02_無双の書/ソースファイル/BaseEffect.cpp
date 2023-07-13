// プリプロセス
#include "BaseEffect.h"
#include "DxLib.h"
#include "Game.h"

#include "SpawnEffect.h"
#include "FootEffect.h"
#include "ParticleEffect.h"

// 静的メンバ変数の宣言
bool cBaseEffect::m_LoadedResourceFlag(false);

// リソースの読み込み
void cBaseEffect::LoadResource()
{
	if (m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = true;
	cSpawnEffect::LoadResource();
	cFootEffect::LoadResource();
	cParticleEffect::LoadResource();
}

// リソースの解放
void cBaseEffect::DeleteResource()
{
	if (!m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = false;
	cSpawnEffect::DeleteResource();
	cFootEffect::DeleteResource();
	cParticleEffect::DeleteResource();
}

// コンストラクタ
cBaseEffect::cBaseEffect(VEC2F pos)
	: m_Pos(pos)
{
}

// デストラクタ
cBaseEffect::~cBaseEffect()
{
}
