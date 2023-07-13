#include "particle_effect.h"
#include <algorithm>

const float CParticleEffect::m_min_active_time = 2.0f;
const float CParticleEffect::m_max_active_time = 4.0f;
const float CParticleEffect::m_min_base_scale = 0.3f;
const float CParticleEffect::m_max_base_scale = 0.6f;
const float CParticleEffect::m_min_incremental_scale = 0.5f;
const float CParticleEffect::m_max_incremental_scale = 3.0f;

CParticleEffect::CParticleEffect(aqua::IGameObject* parent)
	: IEffect(parent, "ParticleEffect")
	, m_BaseScale(0.0f)
	, m_IncrementalScale(0.0f)
{
}

void CParticleEffect::Initialize(const aqua::CVector2& position)
{
	m_Position = position;

	m_ActiveTimer.Setup(m_min_active_time + (m_max_active_time - m_min_active_time) * aqua::Rand(1000) / 1000.0f);

	m_BaseScale = m_min_base_scale + (m_max_base_scale - m_min_base_scale) * aqua::Rand(1000) / 1000.0f;
	m_IncrementalScale = m_min_incremental_scale + (m_max_incremental_scale - m_min_incremental_scale) * aqua::Rand(1000) / 1000.0f;

	m_ParticleSprite.Create("data\\particle_effect.png");
	m_ParticleSprite.anchor.x = m_ParticleSprite.GetTextureWidth() / 2.0f / 2.0f;
	m_ParticleSprite.anchor.y = m_ParticleSprite.GetTextureHeight() / 2.0f;
	m_ParticleSprite.rect.left = m_ParticleSprite.GetTextureWidth() / 2 * aqua::Rand(1);
	m_ParticleSprite.rect.right = m_ParticleSprite.rect.left + m_ParticleSprite.GetTextureWidth() / 2;
	m_ParticleSprite.color.red = (unsigned char)aqua::Rand(140, 80);
	m_ParticleSprite.color.green = (unsigned char)aqua::Rand(200, 120);
	m_ParticleSprite.color.blue = (unsigned char)((int)m_ParticleSprite.color.green + aqua::Rand(20, -60));
	m_ParticleSprite.blend_mode = aqua::ALPHABLEND::ADD;
}

void CParticleEffect::Update()
{
	m_Velocity.x += 0.05f - 0.1f * (aqua::Rand(1000) / 1000.0f);
	m_Velocity.y -= 0.01f + 0.01f * (aqua::Rand(1000) / 1000.0f);
	m_Velocity *= 0.98f;
	m_Position += m_Velocity;

	m_ParticleSprite.color.alpha = (unsigned char)(std::clamp(min(m_ActiveTimer.GetTime() * 8.0f, (m_ActiveTimer.GetLimit() - m_ActiveTimer.GetTime()) * 0.4f), 0.0f, 1.0f) * 255.0f);
	m_ParticleSprite.scale.x = m_ParticleSprite.scale.y = m_BaseScale + m_IncrementalScale * (1.0f - m_ActiveTimer.GetTime() / m_ActiveTimer.GetLimit());
	m_ParticleSprite.position = m_Position - m_ParticleSprite.anchor * m_ParticleSprite.scale;

	m_ActiveTimer.Update();
	if (m_ActiveTimer.Finished())
		DeleteObject();
}

void CParticleEffect::Draw()
{
	m_ParticleSprite.Draw();
}

void CParticleEffect::Finalize()
{
	m_ParticleSprite.Delete();
}
