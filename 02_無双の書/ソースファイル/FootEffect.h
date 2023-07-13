#pragma once

#include "BaseEffect.h"

class cFootEffect : public cBaseEffect
{
	// 公開関数
public:
	static void LoadResource();		// リソースの読み込み
	static void DeleteResource();	// リソースの解放

	cFootEffect(VEC2F pos);	// コンストラクタ
	void Update() override;		// 更新
	void Draw() const override;	// 描画

	bool IsActive() const override;	// アクティブ状態

	// 非公開変数
private:
	static bool m_LoadedResourceFlag;
	static int m_Image_FootEffect[3];

	int m_Timer;
	double m_Angle;
};
