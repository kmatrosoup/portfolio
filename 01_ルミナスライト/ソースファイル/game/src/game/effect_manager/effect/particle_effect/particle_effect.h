#pragma once

#include "../effect.h"

class CParticleEffect : public IEffect
{
public:
	CParticleEffect(aqua::IGameObject* parent);
	~CParticleEffect() override = default;

	void Initialize(const aqua::CVector2& position);
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	static const float m_min_active_time;
	static const float m_max_active_time;
	static const float m_min_base_scale;
	static const float m_max_base_scale;
	static const float m_min_incremental_scale;
	static const float m_max_incremental_scale;

	aqua::CTimer m_ActiveTimer;
	aqua::CVector2 m_Position;
	aqua::CVector2 m_Velocity;
	float m_BaseScale;
	float m_IncrementalScale;

	aqua::CSprite m_ParticleSprite;
};
