#pragma once

#include "BaseEnemy.h"
#include <list>

class cEnemyMgr
{
	// 公開関数
public:
	static void CreateInstance();		// インスタンスを生成
	static void DestroyInstance();		// インスタンスを解放
	static cEnemyMgr& GetInstance();	// インスタンスを取得

	void Update();		// 更新
	void Draw() const;	// 描画

	std::list<cBaseEnemy*>& GetEnemyList();	// 敵のリストを取得
	void CreateEnemy(eENEMY enemy);			// 敵を生成
	void ClearAllEnemy();					// 全ての敵を削除

	// 非公開関数
private:
	cEnemyMgr(const cEnemyMgr&) = delete;
	cEnemyMgr& operator=(const cEnemyMgr&) = delete;
	cEnemyMgr(cEnemyMgr&&) = delete;
	cEnemyMgr& operator=(cEnemyMgr&&) = delete;

	cEnemyMgr();	// コンストラクタ
	~cEnemyMgr();	// デストラクタ

	// 非公開変数
private:
	static cEnemyMgr* m_pInstance;

	std::list<cBaseEnemy*> m_pEnemyList;
};
