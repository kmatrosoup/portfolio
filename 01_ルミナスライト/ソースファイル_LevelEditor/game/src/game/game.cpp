#include "game.h"
#include "back_ground/back_ground.h"
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
	, m_pBackGround(nullptr)
	, m_pField(nullptr)
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
	// ゲームオブジェクトの生成
	m_pBackGround = aqua::CreateGameObject<CBackGround>(this);
	m_pField = aqua::CreateGameObject<CField>(this);
	m_pSoundManager = aqua::CreateGameObject<CSoundManager>(this);
	m_pEffectManager = aqua::CreateGameObject<CEffectManager>(this);
	m_pGlowScreen = aqua::CreateGameObject<CGlowScreen>(this);

	// ゲームオブジェクトの初期化
	IGameObject::Initialize();

	// 背景色の設定
	m_pBackGround->paramR = 0.10f;
	m_pBackGround->paramG = 0.15f;
	m_pBackGround->paramB = 0.20f;

	// タイマーのセットアップ
	m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);

	// BGMの再生
	m_pSoundManager->Play(SOUND_ID::BGM);
}

/*
 *  更新
 */
void
CGame::
Update(void)
{
	// パーティクル生成の更新
	m_CreateParticleTimer.Update();
	if (m_CreateParticleTimer.Finished())
	{
		m_pEffectManager->CreateEffect_RandomParticle();
		m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);
	}

	// ゲームオブジェクトの更新
	IGameObject::Update();
}

/*
 *  描画
 */
void
CGame::
Draw(void)
{
	// 発光スクリーンへの書き込み
	m_pGlowScreen->Clear();
	m_pGlowScreen->Begin();
	m_pField->Draw_Lit();
	m_pEffectManager->Draw_Lit();
	m_pGlowScreen->End();

	// ゲームオブジェクトの描画
	IGameObject::Draw();
}
