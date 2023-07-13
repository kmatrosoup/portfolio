#pragma once

#include "BaseEffect.h"

class cSpawnEffect : public cBaseEffect
{
	// 公開関数
public:
	static void LoadResource();		// リソースの読み込み
	static void DeleteResource();	// リソースの解放

	cSpawnEffect(VEC2F pos);	// コンストラクタ
	void Update() override;		// 更新
	void Draw() const override;	// 描画

	bool IsActive() const override;	// アクティブ状態

	// 非公開変数
private:
	static bool m_LoadedResourceFlag;
	static int m_Image_SpawnEffect[5];

	int m_Timer;
	double m_Angle;
};
