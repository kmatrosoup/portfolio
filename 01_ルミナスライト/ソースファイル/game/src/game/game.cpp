#include "game.h"
#include "common_data/common_data.h"
#include "back_ground/back_ground.h"
#include "scene_manager/scene_manager.h"
#include "effect_manager/effect_manager.h"
#include "fade_screen/fade_screen.h"
#include "glow_screen/glow_screen.h"
#include "sound_manager/sound_manager.h"

/*
 *  コンストラクタ
 */
CGame::
CGame(aqua::IGameObject* parent)
	: aqua::IGameObject(parent, "Game")
	, m_pCommonData(nullptr)
	, m_pBackGround(nullptr)
	, m_pSceneManager(nullptr)
	, m_pEffectManager(nullptr)
	, m_pFadeScreen(nullptr)
	, m_pGlowScreen(nullptr)
	, m_pSoundManager(nullptr)
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
	m_pCommonData = aqua::CreateGameObject<CCommonData>(this);
	m_pBackGround = aqua::CreateGameObject<CBackGround>(this);
	m_pSceneManager = aqua::CreateGameObject<CSceneManager>(this);
	m_pEffectManager = aqua::CreateGameObject<CEffectManager>(this);
	m_pFadeScreen = aqua::CreateGameObject<CFadeScreen>(this);
	m_pGlowScreen = aqua::CreateGameObject<CGlowScreen>(this);
	m_pSoundManager = aqua::CreateGameObject<CSoundManager>(this);

	// ゲームオブジェクトの初期化
	IGameObject::Initialize();

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
	m_pSceneManager->Draw_Lit();
	m_pEffectManager->Draw_Lit();
	m_pFadeScreen->Draw_Lit();
	m_pGlowScreen->End();

	// ゲームオブジェクトの描画
	IGameObject::Draw();
}
