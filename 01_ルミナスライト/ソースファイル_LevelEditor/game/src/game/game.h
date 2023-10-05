#pragma once

#include "aqua.h"

class CBackGround;
class CField;
class CSoundManager;
class CEffectManager;
class CGlowScreen;

class CGame
	: public aqua::IGameObject
{
public:
	CGame(aqua::IGameObject* parent);
	~CGame(void) = default;

	void Initialize(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	static const float m_min_create_particle_time;
	static const float m_max_create_particle_time;

	CBackGround* m_pBackGround;
	CField* m_pField;
	CSoundManager* m_pSoundManager;
	CEffectManager* m_pEffectManager;
	CGlowScreen* m_pGlowScreen;

	aqua::CTimer m_CreateParticleTimer;
};
