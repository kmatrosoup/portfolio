#pragma once

#include "BaseAtkObj.h"
#include <list>

class cAtkObjMgr
{
	// 公開関数
public:
	static void CreateInstance();		// インスタンスを生成
	static void DestroyInstance();		// インスタンスを解放
	static cAtkObjMgr& GetInstance();	// インスタンスを取得

	void Update();		// 更新
	void Draw() const;	// 描画

	void CreateAtkObj(eATK_OBJ atk_obj, VEC2F pos, bool inversion);	// 攻撃オブジェクトを生成
	void ClearAllAtkObj();											// 全ての攻撃オブジェクトを削除

	// 非公開関数
private:
	cAtkObjMgr(const cAtkObjMgr&) = delete;
	cAtkObjMgr& operator=(const cAtkObjMgr&) = delete;
	cAtkObjMgr(cAtkObjMgr&&) = delete;
	cAtkObjMgr& operator=(cAtkObjMgr&&) = delete;

	cAtkObjMgr();		// コンストラクタ
	~cAtkObjMgr();		// デストラクタ

	// 非公開変数
private:
	static cAtkObjMgr* m_pInstance;

	std::list<cBaseAtkObj*> m_pAtkObjList;
};
