#pragma once

#include "BaseAtkObj.h"

class cExplosionAtkObj : public cBaseAtkObj
{
	// 公開関数
public:
	static void LoadResource();		// リソースの読み込み
	static void DeleteResource();	// リソースの解放

	cExplosionAtkObj(VEC2F pos);	// コンストラクタ
	void Update() override;			// 更新
	void Draw() const override;		// 描画

	bool IsActive() const override;	// アクティブ状態

	// 非公開変数
private:
	static bool m_LoadedResourceFlag;
	static int m_Image_Inner;
	static int m_Image_Outer;
	static int m_Sound_Explosion;
	static int m_Sound_ExplosionReady;

	int m_Timer;
};
