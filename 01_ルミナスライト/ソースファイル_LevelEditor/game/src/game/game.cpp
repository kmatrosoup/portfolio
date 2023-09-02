#include "game.h"
#include "field/field.h"
#include "sound_manager/sound_manager.h"
#include "effect_manager/effect_manager.h"
#include "glow_screen/glow_screen.h"

const float	CGame::m_min_create_particle_time = 0.02f;
const float	CGame::m_max_create_particle_time = 0.08f;

/*
 *  コンストラクタ
 */
CGame::
CGame(aqua::IGameObject* parent)
	: aqua::IGameObject(parent, "Game")
	, m_pSoundManager(nullptr)
	, m_pEffectManager(nullptr)
	, m_pGlowScreen(nullptr)
{
}

/*
 *  初期化
 */
void
CGame::
Initialize(void)
{
	aqua::CreateGameObject<CField>(this);
	m_pSoundManager = aqua::CreateGameObject<CSoundManager>(this);
	m_pEffectManager = aqua::CreateGameObject<CEffectManager>(this);
	m_pGlowScreen = aqua::CreateGameObject<CGlowScreen>(this);

	IGameObject::Initialize();

	m_pSoundManager->Play(SOUND_ID::BGM);
	m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);
}

/*
 *  更新
 */
void
CGame::
Update(void)
{
	m_pGlowScreen->Clear();

	// パーティクル生成の更新
	m_CreateParticleTimer.Update();
	if (m_CreateParticleTimer.Finished())
	{
		m_pEffectManager->CreateEffect_RandomParticle();
		m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);
	}

	// グロー効果付与の描画
	m_pGlowScreen->Begin();
	{
		m_pEffectManager->DrawEffects();
	}
	m_pGlowScreen->End();

	IGameObject::Update();
}

/*
 *  描画
 */
void
CGame::
Draw(void)
{
	aqua::Clear(0xff102030);

	IGameObject::Draw();
	m_pEffectManager->DrawEffects();
}
