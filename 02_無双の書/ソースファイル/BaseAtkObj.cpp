// プリプロセス
#include "BaseAtkObj.h"

#include "FireballAtkObj.h"
#include "ExplosionAtkObj.h"

// 静的メンバ変数の宣言
bool cBaseAtkObj::m_LoadedResourceFlag(false);

// リソースの読み込み
void cBaseAtkObj::LoadResource()
{
	if (m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = true;
	cFireballAtkObj::LoadResource();
	cExplosionAtkObj::LoadResource();
}

// リソースの解放
void cBaseAtkObj::DeleteResource()
{
	if (!m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = false;
	cFireballAtkObj::DeleteResource();
	cExplosionAtkObj::DeleteResource();
}

// コンストラクタ
cBaseAtkObj::cBaseAtkObj(VEC2F pos, bool inversion)
	: m_Pos(pos)
	, m_Inversion(inversion)
{
}

// デストラクタ
cBaseAtkObj::~cBaseAtkObj()
{
}
