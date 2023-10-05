#pragma once

#include "aqua.h"

class CEffectManager : public aqua::IGameObject
{
public:
	CEffectManager(aqua::IGameObject* parent);
	~CEffectManager() override = default;

	void Draw_Lit();

	void CreateEffect_RandomParticle();	// 粒子エフェクトを生成
	void CreateEffect_RandomLine();		// 横線エフェクトを生成
	void ClearEffects();				// 全エフェクトを削除

private:
};
