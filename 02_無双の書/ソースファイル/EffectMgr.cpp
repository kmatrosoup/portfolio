// プリプロセス
#include "EffectMgr.h"
#include "DxLib.h"
#include "Game.h"

#include "SpawnEffect.h"
#include "FootEffect.h"
#include "ParticleEffect.h"

// 静的メンバ変数の宣言
cEffectMgr* cEffectMgr::m_pInstance(nullptr);

// インスタンスを生成
void cEffectMgr::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cEffectMgr;
}

// インスタンスを解放
void cEffectMgr::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

// インスタンスを取得
cEffectMgr& cEffectMgr::GetInstance()
{
	return *m_pInstance;
}

// コンストラクタ
cEffectMgr::cEffectMgr()
{
	cBaseEffect::LoadResource();
}

// デストラクタ
cEffectMgr::~cEffectMgr()
{
	cBaseEffect::DeleteResource();

	for (auto& i : m_pEffectList)
		delete i;
}

// 更新
void cEffectMgr::Update()
{
	for (auto i = m_pEffectList.begin(); i != m_pEffectList.end();)
	{
		(*i)->Update();

		if ((*i)->IsActive())
			++i;
		else
		{
			delete (*i);
			i = m_pEffectList.erase(i);
		}
	}
}

// 描画
void cEffectMgr::Draw() const
{
	for (const auto& i : m_pEffectList)
		i->Draw();
}

// エフェクトを生成
void cEffectMgr::CreateEffect(eEFFECT effect, VEC2F pos, float angle, float speed)
{
	cBaseEffect* effect_instance = nullptr;

	switch (effect)
	{
	case eEFFECT::SPAWN:	effect_instance = new cSpawnEffect(pos);					break;
	case eEFFECT::FOOT:		effect_instance = new cFootEffect(pos);						break;
	case eEFFECT::PARTICLE:	effect_instance = new cParticleEffect(pos, angle, speed);	break;
	}

	if (effect_instance)
		m_pEffectList.push_back(effect_instance);
}
