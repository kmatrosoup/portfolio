#pragma once

#include "aqua.h"

class CEffectManager : public aqua::IGameObject
{
public:
	CEffectManager(aqua::IGameObject* parent);
	~CEffectManager() override = default;

	void Draw() override;

	void CreateEffect_RandomParticle();
	void CreateEffect_RandomLine();
	void ClearEffects();
	void DrawEffects() const;

private:
};
