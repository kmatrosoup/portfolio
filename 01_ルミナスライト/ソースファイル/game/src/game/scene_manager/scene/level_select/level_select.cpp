#include "level_select.h"
#include "../../../common_data/common_data.h"
#include "../../../back_ground/back_ground.h"
#include "../../../fade_screen/fade_screen.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../sound_manager/sound_manager.h"
#include "../../../lamp/lamp.h"
#include <fstream>

const aqua::CVector2	CLevelSelect::m_cancel_button_position = aqua::CVector2(40.0f, 40.0f);
const aqua::CVector2	CLevelSelect::m_cancel_button_size = aqua::CVector2(120.0f, 96.0f);
const aqua::CVector2	CLevelSelect::m_level_button_size = aqua::CVector2(160.0f, 160.0f);
const float				CLevelSelect::m_button_click_time = 0.4f;
const float				CLevelSelect::m_min_create_particle_time = 0.02f;
const float				CLevelSelect::m_max_create_particle_time = 0.08f;
const std::string		CLevelSelect::m_save_data_file = "data\\savedata.txt";

CLevelSelect::CLevelSelect(IGameObject* parent)
	: IScene(parent, "LevelSelect")
	, m_pCommonData(nullptr)
	, m_pBackGround(nullptr)
	, m_pFadeScreen(nullptr)
	, m_pEffectManager(nullptr)
	, m_pSoundManager(nullptr)
	, m_pLamps()
	, m_ClearedLevel(0)
	, m_ButtonCursor(-1)
	, m_ButtonCursor_prev(-1)
	, m_IsButtonClicked(false)
{
}

void CLevelSelect::Initialize()
{
	// セーブファイルの読み込み
	LoadClearedLevel();

	// ゲームオブジェクトの取得
	m_pCommonData = (CCommonData*)aqua::FindGameObject("CommonData");
	m_pBackGround = (CBackGround*)aqua::FindGameObject("BackGround");
	m_pFadeScreen = (CFadeScreen*)aqua::FindGameObject("FadeScreen");
	m_pEffectManager = (CEffectManager*)aqua::FindGameObject("EffectManager");
	m_pSoundManager = (CSoundManager*)aqua::FindGameObject("SoundManager");

	// ランプの生成
	for (int i = 0; i < 12; ++i)
	{
		CLamp* lamp = aqua::CreateGameObject<CLamp>(this);
		lamp->Initialize(
			(float)aqua::Rand((int)aqua::GetWindowWidth() - 40, 40),
			(float)aqua::Rand(400, 0),
			i < m_ClearedLevel);
		m_pLamps.push_back(lamp);
	}

	// 画像の配置
	m_CancelButtonSprite.Create("data\\cancel_button.png");
	m_CancelButtonSprite.position = m_cancel_button_position;
	m_CancelButtonSprite.scale.x = m_cancel_button_size.x / m_CancelButtonSprite.GetTextureWidth();
	m_CancelButtonSprite.scale.y = m_cancel_button_size.y / m_CancelButtonSprite.GetTextureHeight();
	m_CancelButtonLitSprite.Create("data\\cancel_button_lit.ass");
	m_CancelButtonLitSprite.position = m_cancel_button_position;
	m_CancelButtonLitSprite.scale.x = m_cancel_button_size.x / m_CancelButtonLitSprite.GetFrameWidth();
	m_CancelButtonLitSprite.scale.y = m_cancel_button_size.y / m_CancelButtonLitSprite.GetFrameHeight();
	m_CancelButtonFrameSprite.Create("data\\cancel_button_frame.png");
	m_CancelButtonFrameSprite.position = m_cancel_button_position;
	m_CancelButtonFrameSprite.scale.x = m_cancel_button_size.x / m_CancelButtonFrameSprite.GetTextureWidth();
	m_CancelButtonFrameSprite.scale.y = m_cancel_button_size.y / m_CancelButtonFrameSprite.GetTextureHeight();
	m_CancelButtonFrameSprite.blend_mode = aqua::ALPHABLEND::ADD;
	m_CancelButtonFrameSprite.visible = false;
	m_CancelButtonEffectSprite.Create("data\\cancel_button_effect.png");
	m_CancelButtonEffectSprite.position = m_cancel_button_position;
	m_CancelButtonEffectSprite.scale.x = m_cancel_button_size.x / m_CancelButtonEffectSprite.GetTextureWidth();
	m_CancelButtonEffectSprite.scale.y = m_cancel_button_size.y / m_CancelButtonEffectSprite.GetTextureHeight();
	m_CancelButtonEffectSprite.blend_mode = aqua::ALPHABLEND::ADD;
	m_CancelButtonEffectSprite.visible = false;

	for (int i = 0; i < 12; ++i)
	{
		m_LevelButtonPosition[i].x = (float)aqua::GetWindowWidth() / 2.0f - 300.0f + 200.0f * (float)(i % 4) + 60.0f * (float)(1 - i / 4);
		m_LevelButtonPosition[i].y = (float)aqua::GetWindowHeight() - 540.0f + 200.0f * (float)(i / 4);

		m_LevelButtonSprite[i].Create("data\\level_button.ass");
		m_LevelButtonLitSprite[i].Create("data\\level_button_lit.ass");
		if (i < m_ClearedLevel)
		{
			m_LevelButtonSprite[i].Change("clear");
			m_LevelButtonLitSprite[i].Change("clear");
		}
		else if (i == m_ClearedLevel)
		{
			m_LevelButtonSprite[i].Change("unlock");
			m_LevelButtonLitSprite[i].Change("unlock");
		}
		else
		{
			m_LevelButtonSprite[i].Change("lock");
			m_LevelButtonLitSprite[i].Change("lock");
		}
		m_LevelButtonSprite[i].anchor.x = (float)m_LevelButtonSprite[i].GetFrameWidth() / 2.0f;
		m_LevelButtonSprite[i].anchor.y = (float)m_LevelButtonSprite[i].GetFrameHeight() / 2.0f;
		m_LevelButtonSprite[i].position = m_LevelButtonPosition[i] - m_LevelButtonSprite[i].anchor;
		m_LevelButtonSprite[i].scale.x = m_level_button_size.x / (float)m_LevelButtonSprite[i].GetFrameWidth();
		m_LevelButtonSprite[i].scale.y = m_level_button_size.y / (float)m_LevelButtonSprite[i].GetFrameHeight();
		m_LevelButtonLitSprite[i].anchor.x = (float)m_LevelButtonLitSprite[i].GetFrameWidth() / 2.0f;
		m_LevelButtonLitSprite[i].anchor.y = (float)m_LevelButtonLitSprite[i].GetFrameHeight() / 2.0f;
		m_LevelButtonLitSprite[i].position = m_LevelButtonPosition[i] - m_LevelButtonLitSprite[i].anchor;
		m_LevelButtonLitSprite[i].scale.x = m_level_button_size.x / (float)m_LevelButtonLitSprite[i].GetFrameWidth();
		m_LevelButtonLitSprite[i].scale.y = m_level_button_size.y / (float)m_LevelButtonLitSprite[i].GetFrameHeight();
	}
	m_LevelButtonFrameSprite.Create("data\\level_button_frame.png");
	m_LevelButtonFrameSprite.anchor.x = (float)m_LevelButtonFrameSprite.GetTextureWidth() / 2.0f;
	m_LevelButtonFrameSprite.anchor.y = (float)m_LevelButtonFrameSprite.GetTextureHeight() / 2.0f;
	m_LevelButtonFrameSprite.scale.x = m_level_button_size.x / (float)m_LevelButtonFrameSprite.GetTextureWidth();
	m_LevelButtonFrameSprite.scale.y = m_level_button_size.y / (float)m_LevelButtonFrameSprite.GetTextureHeight();
	m_LevelButtonFrameSprite.blend_mode = aqua::ALPHABLEND::ADD;
	m_LevelButtonFrameSprite.visible = false;
	m_LevelButtonEffectSprite.Create("data\\level_button_effect.png");
	m_LevelButtonEffectSprite.anchor.x = (float)m_LevelButtonEffectSprite.GetTextureWidth() / 2.0f;
	m_LevelButtonEffectSprite.anchor.y = (float)m_LevelButtonEffectSprite.GetTextureHeight() / 2.0f;
	m_LevelButtonEffectSprite.scale.x = m_level_button_size.x / (float)m_LevelButtonEffectSprite.GetTextureWidth();
	m_LevelButtonEffectSprite.scale.y = m_level_button_size.y / (float)m_LevelButtonEffectSprite.GetTextureHeight();
	m_LevelButtonEffectSprite.blend_mode = aqua::ALPHABLEND::ADD;
	m_LevelButtonEffectSprite.visible = false;

	// タイマーの初期化
	m_ButtonClickTimer.Setup(m_button_click_time);
	m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);

	// 背景色の初期化
	m_pBackGround->paramR = 0.10f;
	m_pBackGround->paramG = 0.15f;
	m_pBackGround->paramB = 0.20f;

	// フェード解除
	m_pFadeScreen->SetFade(false);
}

void CLevelSelect::Update()
{
	// カーソルの更新
	m_ButtonCursor_prev = m_ButtonCursor;
	if (!m_IsButtonClicked)
	{
		m_ButtonCursor = -1;
		aqua::CVector2 mpos = aqua::mouse::GetCursorPos();

		if (mpos.x >= m_cancel_button_position.x &&
			mpos.y >= m_cancel_button_position.y &&
			mpos.x < m_cancel_button_position.x + m_cancel_button_size.x &&
			mpos.y < m_cancel_button_position.y + m_cancel_button_size.y)
			m_ButtonCursor = 0;

		if (m_ButtonCursor == -1)
			for (int i = 0; i < 12; ++i)
				if (abs(m_LevelButtonPosition[i].x - mpos.x) < m_level_button_size.x / 2.0f &&
					abs(m_LevelButtonPosition[i].y - mpos.y) < m_level_button_size.y / 2.0f)
				{
					m_ButtonCursor = i + 1;
					break;
				}

		if (m_ButtonCursor != -1 && m_ButtonCursor != m_ButtonCursor_prev)
			m_pSoundManager->Play(SOUND_ID::BUTTON_CURSOR_SE);
	}

	// クリック時の処理
	if (!m_IsButtonClicked && m_ButtonCursor != -1 && aqua::mouse::Trigger(aqua::mouse::BUTTON_ID::LEFT))
	{
		if (m_ButtonCursor == 0)
		{
			m_IsButtonClicked = true;
			m_CancelButtonEffectSprite.visible = true;
			m_pSoundManager->Play(SOUND_ID::BUTTON_CLICK_SE);
		}
		else if (m_ButtonCursor <= m_ClearedLevel + 1)
		{
			m_IsButtonClicked = true;
			m_pCommonData->selectedLevel = m_ButtonCursor;
			m_LevelButtonEffectSprite.position = m_LevelButtonFrameSprite.position;
			m_LevelButtonEffectSprite.visible = true;
			m_pSoundManager->Play(SOUND_ID::BUTTON_CLICK_SE);
		}
		else
		{
			m_pSoundManager->Play(SOUND_ID::BUTTON_ERROR_SE);
		}
	}

	// ボタンクリック後の処理
	if (m_IsButtonClicked)
	{
		m_ButtonClickTimer.Update();
		if (m_ButtonClickTimer.Finished())
			m_pFadeScreen->SetFade(true);
	}
	m_CancelButtonEffectSprite.color.alpha = (unsigned char)(max(1.0f - m_ButtonClickTimer.GetTime(), 0.0f) * 255.0f);
	m_LevelButtonEffectSprite.color.alpha = (unsigned char)(max(1.0f - m_ButtonClickTimer.GetTime(), 0.0f) * 255.0f);

	// ボタン演出後の処理
	if (m_pFadeScreen->IsFinish())
	{
		if (m_ButtonCursor == 0)
			ChangeScene(SCENE_ID::TITLE);
		else if (m_ButtonCursor >= 1 && m_ButtonCursor <= 12)
			ChangeScene(SCENE_ID::GAME_MAIN);
	}

	// キャンセルボタンの更新
	if (m_ButtonCursor == 0)
		m_CancelButtonLitSprite.Update();
	else
		m_CancelButtonLitSprite.ResetAnimationFrame();

	// レベルボタンの更新
	for (int i = 0; i < 12; ++i)
	{
		if (m_ButtonCursor == i + 1)
			m_LevelButtonLitSprite[i].Update();
		else
			m_LevelButtonLitSprite[i].ResetAnimationFrame();
	}

	// カーソルフレームの更新
	m_CancelButtonFrameSprite.visible = (m_ButtonCursor == 0);
	if (m_ButtonCursor >= 1)
		m_LevelButtonFrameSprite.position = m_LevelButtonPosition[m_ButtonCursor - 1] - m_LevelButtonFrameSprite.anchor;
	m_LevelButtonFrameSprite.visible = (m_ButtonCursor >= 1);

	// ランプの更新
	for (auto elem : m_pLamps)
		elem->Update();

	// パーティクル生成の更新
	m_CreateParticleTimer.Update();
	if (m_CreateParticleTimer.Finished())
	{
		m_pEffectManager->CreateEffect_RandomParticle();
		m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);
	}
}

void CLevelSelect::Draw()
{
	// ランプ
	for (auto elem : m_pLamps)
		elem->Draw();

	// 戻るボタン
	m_CancelButtonSprite.color = 0xffffffff;
	m_CancelButtonSprite.Draw();
	m_CancelButtonLitSprite.Draw();
	m_CancelButtonFrameSprite.Draw();
	m_CancelButtonEffectSprite.Draw();

	// レベルボタン
	for (int i = 0; i < 12; ++i)
	{
		m_LevelButtonSprite[i].color = 0xffffffff;
		m_LevelButtonSprite[i].Draw();
		m_LevelButtonLitSprite[i].Draw();
	}
	m_LevelButtonFrameSprite.Draw();
	m_LevelButtonEffectSprite.Draw();
}

void CLevelSelect::Draw_Lit()
{
	// ランプ
	for (auto elem : m_pLamps)
		elem->Draw_Lit();

	// 戻るボタン
	m_CancelButtonSprite.color = 0xff000000;
	m_CancelButtonSprite.Draw();
	m_CancelButtonLitSprite.Draw();
	m_CancelButtonEffectSprite.Draw();

	// レベルボタン
	for (int i = 0; i < 12; ++i)
	{
		m_LevelButtonSprite[i].color = 0xff000000;
		m_LevelButtonSprite[i].Draw();
		m_LevelButtonLitSprite[i].Draw();
	}
	m_LevelButtonEffectSprite.Draw();
}

void CLevelSelect::Finalize()
{
	m_CancelButtonSprite.Delete();
	m_CancelButtonLitSprite.Delete();
	m_CancelButtonFrameSprite.Delete();
	m_CancelButtonEffectSprite.Delete();
	for (int i = 0; i < 12; ++i)
	{
		m_LevelButtonSprite[i].Delete();
		m_LevelButtonLitSprite[i].Delete();
	}
	m_LevelButtonFrameSprite.Delete();
	m_LevelButtonEffectSprite.Delete();

	IGameObject::Finalize();
}

void CLevelSelect::LoadClearedLevel()
{
	std::ifstream ifs(m_save_data_file);

	// セーブデータが存在する場合
	if (!ifs.fail())
	{
		ifs >> m_ClearedLevel;
	}

	// セーブデータが存在しない場合
	else
	{
		m_ClearedLevel = 0;
	}

	// ファイルを閉じる
	ifs.close();
}
