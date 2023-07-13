#pragma once

#include "BaseEffect.h"
#include <list>

class cEffectMgr
{
	// 公開関数
public:
	static void CreateInstance();		// インスタンスを生成
	static void DestroyInstance();		// インスタンスを解放
	static cEffectMgr& GetInstance();	// インスタンスを取得

	void Update();		// 更新
	void Draw() const;	// 描画

	void CreateEffect(eEFFECT effect, VEC2F pos, float angle = 0.0f, float speed = 0.0f);	// エフェクトを生成

	// 非公開関数
private:
	cEffectMgr(const cEffectMgr&) = delete;
	cEffectMgr& operator=(const cEffectMgr&) = delete;
	cEffectMgr(cEffectMgr&&) = delete;
	cEffectMgr& operator=(cEffectMgr&&) = delete;

	cEffectMgr();		// コンストラクタ
	~cEffectMgr();	// デストラクタ

	// 非公開変数
private:
	static cEffectMgr* m_pInstance;

	std::list<cBaseEffect*> m_pEffectList;
};
