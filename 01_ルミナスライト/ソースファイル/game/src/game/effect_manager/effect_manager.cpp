#include "effect_manager.h"

#include "effect/particle_effect/particle_effect.h"
#include "effect/line_effect/line_effect.h"

CEffectManager::CEffectManager(aqua::IGameObject* parent)
	: IGameObject(parent, "EffectManager")
{
}

void CEffectManager::Draw_Lit()
{
	IGameObject::Draw();
}

void CEffectManager::CreateEffect_RandomParticle()
{
	CParticleEffect* particle = aqua::CreateGameObject<CParticleEffect>(this);
	particle->Initialize(aqua::CVector2(
		-100.0f + (float)aqua::Rand(aqua::GetWindowWidth() + 200),
		-100.0f + (float)aqua::Rand(aqua::GetWindowHeight() + 200)));
}

void CEffectManager::CreateEffect_RandomLine()
{
	CLineEffect* particle = aqua::CreateGameObject<CLineEffect>(this);
	particle->Initialize(-100.0f + (float)aqua::Rand(aqua::GetWindowHeight() + 200));
}

void CEffectManager::ClearEffects()
{
	for (auto& i : m_ChildObjectList)
	{
		i->Finalize();
		AQUA_SAFE_DELETE(i);
	}
	m_ChildObjectList.clear();
}
