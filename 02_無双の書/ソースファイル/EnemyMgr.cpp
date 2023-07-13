// プリプロセス
#include "EnemyMgr.h"
#include "DxLib.h"
#include "Game.h"
#include "EffectMgr.h"

#include "SlimeEnemy.h"
#include "BatEnemy.h"
#include "SkullEnemy.h"

// 静的メンバ変数の宣言
cEnemyMgr* cEnemyMgr::m_pInstance(nullptr);

// インスタンスを生成
void cEnemyMgr::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cEnemyMgr;
}

// インスタンスを解放
void cEnemyMgr::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

// インスタンスを取得
cEnemyMgr& cEnemyMgr::GetInstance()
{
	return *m_pInstance;
}

// コンストラクタ
cEnemyMgr::cEnemyMgr()
{
	cBaseEnemy::LoadResource();
}

// デストラクタ
cEnemyMgr::~cEnemyMgr()
{
	cBaseEnemy::DeleteResource();

	for (auto& i : m_pEnemyList)
		delete i;
}

// 更新
void cEnemyMgr::Update()
{
	// 敵同士の衝突
	for (auto& i : m_pEnemyList)
	{
		for (auto& j : m_pEnemyList)
		{
			if (i == j)
				continue;

			float distance = VEC2F::Dist(i->GetPos(), j->GetPos());
			float i_radius = i->GetRadius();
			float j_radius = j->GetRadius();
			if (distance >= i_radius + j_radius)
				continue;

			float angle = atan2f(j->GetPos().y - i->GetPos().y, j->GetPos().x - i->GetPos().x);
			i->AddMotion(VEC2F(cosf(angle), sinf(angle)) * (distance - (i_radius + j_radius)) / (i_radius + j_radius));
		}
	}

	// 更新
	for (auto i = m_pEnemyList.begin(); i != m_pEnemyList.end();)
	{
		(*i)->Update();

		if ((*i)->IsActive())
			++i;
		else
		{
			delete (*i);
			i = m_pEnemyList.erase(i);
		}
	}
}

// 描画
void cEnemyMgr::Draw() const
{
	for (const auto& i : m_pEnemyList)
		i->Draw();
}

// 敵のリストを取得
std::list<cBaseEnemy*>& cEnemyMgr::GetEnemyList()
{
	return m_pEnemyList;
}

// 敵を生成
void cEnemyMgr::CreateEnemy(eENEMY enemy)
{
	if (enemy == eENEMY::MAX)
		enemy = (eENEMY)GetRand((int)eENEMY::MAX - 1);

	cBaseEnemy* enemy_instance = nullptr;

	switch (enemy)
	{
	case eENEMY::SLIME:	enemy_instance = new cSlimeEnemy((float)(FLOOR_X1 + GetRand(FLOOR_X2 - FLOOR_X1)));	break;
	case eENEMY::BAT:	enemy_instance = new cBatEnemy((float)(FLOOR_X1 + GetRand(FLOOR_X2 - FLOOR_X1)));	break;
	case eENEMY::SKULL:	enemy_instance = new cSkullEnemy((float)(FLOOR_X1 + GetRand(FLOOR_X2 - FLOOR_X1)));	break;
	}

	if (enemy_instance)
	{
		cEffectMgr::GetInstance().CreateEffect(eEFFECT::SPAWN, enemy_instance->GetPos());
		m_pEnemyList.push_back(enemy_instance);
	}
}

// 全ての敵を削除
void cEnemyMgr::ClearAllEnemy()
{
	for (auto& i : m_pEnemyList)
		delete i;
	m_pEnemyList.clear();
}
