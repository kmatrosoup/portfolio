#pragma once

#include "VEC2.h"

enum class eEFFECT
{
	SPAWN,
	FOOT,
	PARTICLE,

	MAX
};

class cBaseEffect
{
	// 公開関数
public:
	static void LoadResource();		// リソースの読み込み
	static void DeleteResource();	// リソースの解放

	cBaseEffect(VEC2F pos);			// コンストラクタ
	~cBaseEffect();					// デストラクタ
	virtual void Update() = 0;		// 更新
	virtual void Draw() const = 0;	// 描画

	virtual bool IsActive() const = 0;	// アクティブ状態

	// 保護変数
protected:
	VEC2F m_Pos;

	// 非公開変数
private:
	static bool m_LoadedResourceFlag;
};
