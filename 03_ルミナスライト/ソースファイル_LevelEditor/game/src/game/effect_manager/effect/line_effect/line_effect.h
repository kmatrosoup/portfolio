#pragma once

#include "../effect.h"

class CLineEffect : public IEffect
{
public:
	CLineEffect(aqua::IGameObject* parent);
	~CLineEffect() override = default;

	void Initialize(float pos_y);
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	static const float m_max_wait_time;
	static const float m_min_active_time;
	static const float m_max_active_time;
	static const float m_min_base_thickness;
	static const float m_max_base_thickness;

	aqua::CTimer m_WaitTimer;
	aqua::CTimer m_ActiveTimer;
	float m_PositionY;
	float m_BaseThickness;

	aqua::CLinePrimitive m_LinePrimitive;
};
