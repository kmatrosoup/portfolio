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
	static const float m_min_active_time;		// 最低生存時間
	static const float m_max_active_time;		// 最大生存時間
	static const float m_min_base_scale;		// 最低元スケール
	static const float m_max_base_scale;		// 最大元スケール
	static const float m_min_incremental_scale;	// 最低スケール加算値
	static const float m_max_incremental_scale;	// 最大スケール加算値

	aqua::CTimer m_ActiveTimer;	// 生存時間
	aqua::CVector2 m_Position;	// 座標
	aqua::CVector2 m_Velocity;	// 移動量
	float m_BaseScale;			// 元スケール
	float m_IncrementalScale;	// スケール加算値

	aqua::CSprite m_ParticleSprite;	// 粒子エフェクトの画像
};
