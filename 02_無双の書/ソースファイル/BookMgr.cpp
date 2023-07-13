// プリプロセス
#include "BookMgr.h"
#include "DxLib.h"
#include "Game.h"
#include "EffectMgr.h"

#include "FireballBook.h"
#include "ExplosionBook.h"

// 静的メンバ変数の宣言
cBookMgr* cBookMgr::m_pInstance(nullptr);

// インスタンスを生成
void cBookMgr::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cBookMgr;
}

// インスタンスを解放
void cBookMgr::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

// インスタンスを取得
cBookMgr& cBookMgr::GetInstance()
{
	return *m_pInstance;
}

// コンストラクタ
cBookMgr::cBookMgr()
{
	cBaseBook::LoadResource();
}

// デストラクタ
cBookMgr::~cBookMgr()
{
	cBaseBook::DeleteResource();

	for (auto& i : m_pBookList)
		delete i;
}

// 更新
void cBookMgr::Update()
{
	for (auto i = m_pBookList.begin(); i != m_pBookList.end();)
	{
		(*i)->Update();

		if ((*i)->IsActive())
			++i;
		else
		{
			delete (*i);
			i = m_pBookList.erase(i);
		}
	}
}

// 描画
void cBookMgr::Draw() const
{
	for (const auto& i : m_pBookList)
		if (i->GetActiveTime() > 120 || (i->GetActiveTime() / 2) % 2 == 0)
			i->Draw();
}

// 本のリストを取得
std::list<cBaseBook*>& cBookMgr::GetBookList()
{
	return m_pBookList;
}

// 本を生成
void cBookMgr::CreateBook(eSPELL spell)
{
	if (spell == eSPELL::MAX)
		spell = (eSPELL)(GetRand((int)eSPELL::MAX - 1));

	cBaseBook* book_instance = nullptr;

	switch (spell)
	{
	case eSPELL::FIRE_BALL:
		book_instance = new cFireballBook(
			VEC2F((float)(FLOOR_X1 + GetRand(FLOOR_X2 - FLOOR_X1)), (float)(FLOOR_Y1 + GetRand(FLOOR_Y2 - FLOOR_Y1))),
			VEC2F(0.0f, 0.0f),
			DX_PI_F / 180.0f * GetRand(359),
			false);
		break;
	case eSPELL::EXPLOSION:
		book_instance = new cExplosionBook(
			VEC2F((float)(FLOOR_X1 + GetRand(FLOOR_X2 - FLOOR_X1)), (float)(FLOOR_Y1 + GetRand(FLOOR_Y2 - FLOOR_Y1))),
			VEC2F(0.0f, 0.0f),
			DX_PI_F / 180.0f * GetRand(359),
			false);
		break;
	}

	if (book_instance)
	{
		cEffectMgr::GetInstance().CreateEffect(eEFFECT::SPAWN, book_instance->GetPos());
		m_pBookList.push_back(book_instance);
	}
}

// 本をリストに追加
void cBookMgr::AddBook(cBaseBook* book)
{
	m_pBookList.push_back(book);
}

// 本をリストから削除
void cBookMgr::RemoveBook(cBaseBook* book)
{
	std::list<cBaseBook*>::iterator itr = std::find(m_pBookList.begin(), m_pBookList.end(), book);
	if (itr != m_pBookList.end())
		m_pBookList.erase(itr);
}

// 本が存在するか判定
bool cBookMgr::ExistBook(cBaseBook* book)
{
	return std::find(m_pBookList.begin(), m_pBookList.end(), book) != m_pBookList.end();
}

// 全ての本を削除
void cBookMgr::ClearAllBook()
{
	for (auto& i : m_pBookList)
		delete i;
	m_pBookList.clear();
}
