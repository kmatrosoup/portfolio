#include "game.h"
#include "common_data/common_data.h"
#include "sound_manager/sound_manager.h"
#include "scene_manager/scene_manager.h"
#include "effect_manager/effect_manager.h"
#include "glow_screen/glow_screen.h"

/*
 *  コンストラクタ
 */
CGame::
CGame(aqua::IGameObject* parent)
	: aqua::IGameObject(parent, "Game")
	, m_pCommonData(nullptr)
	, m_pSoundManager(nullptr)
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
	m_pCommonData = aqua::CreateGameObject<CCommonData>(this);
	m_pSoundManager = aqua::CreateGameObject<CSoundManager>(this);
	aqua::CreateGameObject<CSceneManager>(this);
	aqua::CreateGameObject<CEffectManager>(this);
	m_pGlowScreen = aqua::CreateGameObject<CGlowScreen>(this);

	IGameObject::Initialize();

	m_pSoundManager->Play(SOUND_ID::BGM);
}

/*
 *  更新
 */
void
CGame::
Update(void)
{
	m_pGlowScreen->Clear();

	IGameObject::Update();
}

/*
 *  描画
 */
void
CGame::
Draw(void)
{
	unsigned int bg_color =
		0xff000000 |
		((int)(m_pCommonData->GetBGParamR() * 255.0f) << 16) |
		((int)(m_pCommonData->GetBGParamG() * 255.0f) << 8) |
		((int)(m_pCommonData->GetBGParamB() * 255.0f) << 0);

	aqua::Clear(bg_color);

	IGameObject::Draw();
}
