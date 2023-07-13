#pragma once

#include "BaseAtkObj.h"
#include <list>

class cBaseEnemy;

class cFireballAtkObj : public cBaseAtkObj
{
	// 公開関数
public:
	static void LoadResource();		// リソースの読み込み
	static void DeleteResource();	// リソースの解放

	cFireballAtkObj(VEC2F pos, bool inversion);	// コンストラクタ
	void Update() override;						// 更新
	void Draw() const override;					// 描画

	bool IsActive() const override;	// アクティブ状態

	// 非公開変数
private:
	static bool m_LoadedResourceFlag;
	static int m_Image_Ball;
	static int m_Sound_Shot;
	static int m_Sound_Bound;

	std::list<cBaseEnemy*> m_pHitEnemy;
	float m_Angle;
	int m_ParticleTimer;
};
