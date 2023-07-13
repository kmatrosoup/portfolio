// プリプロセス
#include "AtkObjMgr.h"

#include "FireballAtkObj.h"
#include "ExplosionAtkObj.h"

// 静的メンバ変数の宣言
cAtkObjMgr* cAtkObjMgr::m_pInstance(nullptr);

// インスタンスを生成
void cAtkObjMgr::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cAtkObjMgr;
}

// インスタンスを解放
void cAtkObjMgr::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

// インスタンスを取得
cAtkObjMgr& cAtkObjMgr::GetInstance()
{
	return *m_pInstance;
}

// コンストラクタ
cAtkObjMgr::cAtkObjMgr()
{
	cBaseAtkObj::LoadResource();
}

// デストラクタ
cAtkObjMgr::~cAtkObjMgr()
{
	cBaseAtkObj::DeleteResource();

	for (auto& i : m_pAtkObjList)
		delete i;
}

// 更新
void cAtkObjMgr::Update()
{
	for (auto i = m_pAtkObjList.begin(); i != m_pAtkObjList.end();)
	{
		(*i)->Update();

		if ((*i)->IsActive())
			++i;
		else
		{
			delete (*i);
			i = m_pAtkObjList.erase(i);
		}
	}
}

// 描画
void cAtkObjMgr::Draw() const
{
	for (const auto& i : m_pAtkObjList)
		i->Draw();
}

// 攻撃オブジェクトを生成
void cAtkObjMgr::CreateAtkObj(eATK_OBJ atk_obj, VEC2F pos, bool inversion)
{
	switch (atk_obj)
	{
	case eATK_OBJ::FIREBALL:	m_pAtkObjList.push_back(new cFireballAtkObj(pos, inversion));	break;
	case eATK_OBJ::EXPLOSION:	m_pAtkObjList.push_back(new cExplosionAtkObj(pos));				break;
	}
}

// 全ての攻撃オブジェクトを削除
void cAtkObjMgr::ClearAllAtkObj()
{
	for (auto& i : m_pAtkObjList)
		delete i;
	m_pAtkObjList.clear();
}
