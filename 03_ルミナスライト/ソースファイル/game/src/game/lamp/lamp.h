#pragma once

#include "aqua.h"

class CGlowScreen;

class CLamp : public aqua::IGameObject
{
public:
	CLamp(IGameObject* parent);
	~CLamp() override = default;

	void Initialize(float pos_x, float anchor_y, bool lit_flag);
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	CGlowScreen* m_pGlowScreen;

	float m_PositionX;
	float m_AngleVelocity;

	aqua::CSprite m_BaseSprite;
	aqua::CSprite m_LitSprite;
};
