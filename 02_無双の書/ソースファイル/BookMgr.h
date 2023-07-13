#pragma once

#include "BaseBook.h"
#include <list>

class cBookMgr
{
	// 公開関数
public:
	static void CreateInstance();		// インスタンスを生成
	static void DestroyInstance();		// インスタンスを解放
	static cBookMgr& GetInstance();	// インスタンスを取得

	void Update();		// 更新
	void Draw() const;	// 描画

	std::list<cBaseBook*>& GetBookList();	// 本のリストを取得
	void CreateBook(eSPELL spell);			// 本を生成
	void AddBook(cBaseBook* book);			// 本をリストに追加
	void RemoveBook(cBaseBook* book);		// 本をリストから削除
	bool ExistBook(cBaseBook* book);		// 本が存在するか判定
	void ClearAllBook();					// 全ての本を削除

	// 非公開関数
private:
	cBookMgr(const cBookMgr&) = delete;
	cBookMgr& operator=(const cBookMgr&) = delete;
	cBookMgr(cBookMgr&&) = delete;
	cBookMgr& operator=(cBookMgr&&) = delete;

	cBookMgr();		// コンストラクタ
	~cBookMgr();	// デストラクタ

	// 非公開変数
private:
	static cBookMgr* m_pInstance;

	std::list<cBaseBook*> m_pBookList;
};
