#pragma once

#include "aqua.h"

class CLamp : public aqua::IGameObject
{
public:
	CLamp(IGameObject* parent);
	~CLamp() override = default;

	void Initialize(float pos_x, float anchor_y, bool lit_flag);
	void Update() override;
	void Draw() override;
	void Draw_Lit();
	void Finalize() override;

private:
	float m_PositionX;		// X座標
	float m_AngleVelocity;	// 角速度

	aqua::CSprite m_BaseSprite;	// フレームの画像
	aqua::CSprite m_LitSprite;	// 発光部分の画像
};
