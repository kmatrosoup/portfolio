#include "level_select.h"
#include "../../../common_data/common_data.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../sound_manager/sound_manager.h"
#include "../../../glow_screen/glow_screen.h"
#include "../../../lamp/lamp.h"
#include <fstream>

const aqua::CVector2	CLevelSelect::m_cancel_button_position = aqua::CVector2(40.0f, 40.0f);
const aqua::CVector2	CLevelSelect::m_cancel_button_size = aqua::CVector2(120.0f, 96.0f);
const aqua::CVector2	CLevelSelect::m_level_button_size = aqua::CVector2(160.0f, 160.0f);
const float				CLevelSelect::m_button_click_time = 0.4f;
const int				CLevelSelect::m_cancel_button_frame_num = 4;
const float				CLevelSelect::m_cancel_button_animation_time = 0.6f;
const int				CLevelSelect::m_level_button_frame_num = 4;
const float				CLevelSelect::m_level_button_animation_time = 0.6f;
const float				CLevelSelect::m_change_scene_time = 0.4f;
const float				CLevelSelect::m_min_create_particle_time = 0.02f;
const float				CLevelSelect::m_max_create_particle_time = 0.08f;

CLevelSelect::CLevelSelect(IGameObject* parent)
	: IScene(parent, "LevelSelect")
	, m_pCommonData(nullptr)
	, m_pEffectManager(nullptr)
	, m_pSoundManager(nullptr)
	, m_pGlowScreen(nullptr)
	, m_ClearedLevel(0)
	, m_ButtonCursor(-1)
	, m_ButtonCursor_prev(-1)
	, m_ButtonClickedFlag(false)
	, m_ChangeSceneFlag(false)
{
}

void CLevelSelect::Initialize()
{
	// セーブファイルの読み込み
	LoadClearedLevel();

	// ゲームオブジェクトの取得
	m_pCommonData = (CCommonData*)aqua::FindGameObject("CommonData");
	m_pEffectManager = (CEffectManager*)aqua::FindGameObject("EffectManager");
	m_pSoundManager = (CSoundManager*)aqua::FindGameObject("SoundManager");
	m_pGlowScreen = (CGlowScreen*)aqua::FindGameObject("GlowScreen");

	// ゲームオブジェクトの生成
	for (int i = 0; i < 12; ++i)
	{
		CLamp* lamp = aqua::CreateGameObject<CLamp>(this);
		lamp->Initialize(
			(float)aqua::Rand((int)aqua::GetWindowWidth() - 40, 40),
			(float)aqua::Rand(400, 0),
			i < m_ClearedLevel);
	}
	IGameObject::Initialize();

	// 画像の配置
	m_CancelButtonSprite.Create("data\\cancel_button.png");
	m_CancelButtonSprite.position = m_cancel_button_position;
	m_CancelButtonSprite.scale.x = m_cancel_button_size.x / m_CancelButtonSprite.GetTextureWidth();
	m_CancelButtonSprite.scale.y = m_cancel_button_size.y / m_CancelButtonSprite.GetTextureHeight();
	m_CancelButtonLitSprite.Create("data\\cancel_button_lit.png");
	m_CancelButtonLitSprite.position = m_cancel_button_position;
	m_CancelButtonLitSprite.scale.x = m_cancel_button_size.x / m_CancelButtonLitSprite.GetTextureWidth();
	m_CancelButtonLitSprite.scale.y = m_cancel_button_size.y / m_CancelButtonLitSprite.GetTextureHeight() * m_cancel_button_frame_num;
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

		m_LevelButtonSprite[i].Create("data\\level_button.png");
		m_LevelButtonLitSprite[i].Create("data\\level_button_lit.png");
		if (i < m_ClearedLevel)
		{
			m_LevelButtonSprite[i].rect.left = 80;
			m_LevelButtonLitSprite[i].rect.left = 80;
		}
		else if (i == m_ClearedLevel)
		{
			m_LevelButtonSprite[i].rect.left = 40;
			m_LevelButtonLitSprite[i].rect.left = 40;
		}
		else
		{
			m_LevelButtonSprite[i].rect.left = 0;
			m_LevelButtonLitSprite[i].rect.left = 0;
		}
		m_LevelButtonSprite[i].rect.right = m_LevelButtonSprite[i].rect.left + 40;
		m_LevelButtonSprite[i].anchor.x = (float)m_LevelButtonSprite[i].GetTextureWidth() / 2.0f / 3.0f;
		m_LevelButtonSprite[i].anchor.y = (float)m_LevelButtonSprite[i].GetTextureHeight() / 2.0f;
		m_LevelButtonSprite[i].position = m_LevelButtonPosition[i] - m_LevelButtonSprite[i].anchor;
		m_LevelButtonSprite[i].scale.x = m_level_button_size.x / (float)m_LevelButtonSprite[i].GetTextureWidth() * 3.0f;
		m_LevelButtonSprite[i].scale.y = m_level_button_size.y / (float)m_LevelButtonSprite[i].GetTextureHeight();
		m_LevelButtonLitSprite[i].rect.right = m_LevelButtonLitSprite[i].rect.left + 40;
		m_LevelButtonLitSprite[i].anchor.x = (float)m_LevelButtonLitSprite[i].GetTextureWidth() / 2.0f / 3.0f;
		m_LevelButtonLitSprite[i].anchor.y = (float)m_LevelButtonLitSprite[i].GetTextureHeight() / 2.0f / m_level_button_frame_num;
		m_LevelButtonLitSprite[i].position = m_LevelButtonPosition[i] - m_LevelButtonLitSprite[i].anchor;
		m_LevelButtonLitSprite[i].scale.x = m_level_button_size.x / (float)m_LevelButtonLitSprite[i].GetTextureWidth() * 3.0f;
		m_LevelButtonLitSprite[i].scale.y = m_level_button_size.y / (float)m_LevelButtonLitSprite[i].GetTextureHeight() * m_level_button_frame_num;
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

	m_ChangeSceneEffectSprite.Create("data\\scene_change_effect.png");
	m_ChangeSceneEffectSprite.scale = aqua::CVector2::ONE * 40.0f;
	m_ChangeSceneEffectSurface.Create((int)aqua::GetWindowWidth(), (int)aqua::GetWindowHeight(), true);
	m_ChangeSceneEffectSprite_Dest.Create(m_ChangeSceneEffectSurface);

	m_ButtonClickTimer.Setup(m_button_click_time);
	m_CancelButtonTimer.Setup(m_cancel_button_animation_time);
	for (int i = 0; i < 12; ++i)
		m_LevelButtonTimer[i].Setup(m_level_button_animation_time);
	m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);
	m_ChangeSceneInTimer.Setup(m_change_scene_time);
	m_ChangeSceneOutTimer.Setup(m_change_scene_time);
}

void CLevelSelect::Update()
{
	IGameObject::Update();

	// カーソルの更新
	m_ButtonCursor_prev = m_ButtonCursor;
	if (!m_ButtonClickedFlag)
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
	if (!m_ButtonClickedFlag && m_ButtonCursor != -1 && aqua::mouse::Trigger(aqua::mouse::BUTTON_ID::LEFT))
	{
		if (m_ButtonCursor == 0)
		{
			m_ButtonClickedFlag = true;
			m_CancelButtonEffectSprite.visible = true;
			m_pSoundManager->Play(SOUND_ID::BUTTON_CLICK_SE);
		}
		else if (m_ButtonCursor <= m_ClearedLevel + 1)
		{
			m_ButtonClickedFlag = true;
			m_pCommonData->SetSelectedLevel(m_ButtonCursor);
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
	if (m_ButtonClickedFlag)
	{
		m_ButtonClickTimer.Update();
		if (!m_ChangeSceneFlag && m_ButtonClickTimer.Finished())
			m_ChangeSceneFlag = true;
	}
	m_CancelButtonEffectSprite.color.alpha = (unsigned char)(max(1.0f - m_ButtonClickTimer.GetTime(), 0.0f) * 255.0f);
	m_LevelButtonEffectSprite.color.alpha = (unsigned char)(max(1.0f - m_ButtonClickTimer.GetTime(), 0.0f) * 255.0f);

	// シーンインタイマーの更新
	if (!m_ChangeSceneInTimer.Finished())
	{
		m_ChangeSceneInTimer.Update();
		if (m_ChangeSceneInTimer.Finished())
			m_ChangeSceneInTimer.SetTime(m_change_scene_time);
	}

	// シーンアウトタイマーの更新
	if (m_ChangeSceneFlag)
	{
		m_ChangeSceneOutTimer.Update();
		if (m_ChangeSceneOutTimer.Finished())
			m_ChangeSceneOutTimer.SetTime(m_change_scene_time);

		if (m_ChangeSceneOutTimer.Finished())
		{
			if (m_ButtonCursor == 0)
				ChangeScene(SCENE_ID::TITLE);
			else if (m_ButtonCursor >= 1 && m_ButtonCursor <= 12)
				ChangeScene(SCENE_ID::GAME_MAIN);
		}
	}

	// キャンセルボタンの更新
	if (m_ButtonCursor == 0)
	{
		m_CancelButtonTimer.Update();
		if (m_CancelButtonTimer.Finished())
			m_CancelButtonTimer.Reset();
	}
	else
		m_CancelButtonTimer.Reset();
	m_CancelButtonLitSprite.rect.top = (int)(m_CancelButtonTimer.GetTime() / m_cancel_button_animation_time * m_cancel_button_frame_num) * (m_CancelButtonLitSprite.GetTextureHeight() / m_cancel_button_frame_num);
	m_CancelButtonLitSprite.rect.bottom = m_CancelButtonLitSprite.rect.top + m_CancelButtonLitSprite.GetTextureHeight() / m_cancel_button_frame_num;

	// レベルボタンの更新
	for (int i = 0; i < 12; ++i)
	{
		if (m_ButtonCursor == i + 1)
		{
			m_LevelButtonTimer[i].Update();
			if (m_LevelButtonTimer[i].Finished())
				m_LevelButtonTimer[i].Reset();
		}
		else
			m_LevelButtonTimer[i].Reset();
		m_LevelButtonLitSprite[i].rect.top = (int)(m_LevelButtonTimer[i].GetTime() / m_level_button_animation_time * m_level_button_frame_num) * (m_LevelButtonLitSprite[i].GetTextureHeight() / m_level_button_frame_num);
		m_LevelButtonLitSprite[i].rect.bottom = m_LevelButtonLitSprite[i].rect.top + m_LevelButtonLitSprite[i].GetTextureHeight() / m_level_button_frame_num;
	}

	// カーソルフレームの更新
	m_CancelButtonFrameSprite.visible = (m_ButtonCursor == 0);
	if (m_ButtonCursor >= 1)
		m_LevelButtonFrameSprite.position = m_LevelButtonPosition[m_ButtonCursor - 1] - m_LevelButtonFrameSprite.anchor;
	m_LevelButtonFrameSprite.visible = (m_ButtonCursor >= 1);

	// パーティクル生成の更新
	m_CreateParticleTimer.Update();
	if (m_CreateParticleTimer.Finished())
	{
		m_pEffectManager->CreateEffect_RandomParticle();
		m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);
	}

	// シーン切り替え演出の更新
	m_ChangeSceneEffectSurface.Begin();
	aqua::Clear(0x00000000);
	m_ChangeSceneEffectSprite.Draw();
	m_ChangeSceneEffectSurface.End();
	DxLib::GraphFilter(m_ChangeSceneEffectSurface.GetTexture().GetResourceHandle(), DX_GRAPH_FILTER_TWO_COLOR,
		(int)(max(1.0f - m_ChangeSceneInTimer.GetTime() / m_change_scene_time, m_ChangeSceneOutTimer.GetTime() / m_ChangeSceneOutTimer.GetLimit()) * 255.0f),
		0xff000000,
		255,
		0xff000000,
		0);

	// グロー効果付与の描画
	m_pGlowScreen->Begin();
	{
		m_CancelButtonSprite.color = 0xff000000;
		m_CancelButtonSprite.blend_mode = aqua::ALPHABLEND::SUB;
		m_CancelButtonSprite.Draw();
		for (int i = 0; i < 12; ++i)
		{
			m_LevelButtonSprite[i].color = 0xff000000;
			m_LevelButtonSprite[i].blend_mode = aqua::ALPHABLEND::SUB;
			m_LevelButtonSprite[i].Draw();
		}

		m_CancelButtonLitSprite.Draw();
		m_CancelButtonFrameSprite.Draw();
		m_CancelButtonEffectSprite.Draw();
		for (int i = 0; i < 12; ++i)
			m_LevelButtonLitSprite[i].Draw();
		m_LevelButtonFrameSprite.Draw();
		m_LevelButtonEffectSprite.Draw();
		m_pEffectManager->DrawEffects();
		m_ChangeSceneEffectSprite_Dest.Draw();
	}
	m_pGlowScreen->End();

	// 背景色の更新
	m_pCommonData->SetBGParamR(0.10f);
	m_pCommonData->SetBGParamG(0.15f);
	m_pCommonData->SetBGParamB(0.20f);
}

void CLevelSelect::Draw()
{
	IGameObject::Draw();

	m_CancelButtonSprite.color = 0xffffffff;
	m_CancelButtonSprite.blend_mode = aqua::ALPHABLEND::ALPHA;
	m_CancelButtonSprite.Draw();
	m_CancelButtonLitSprite.Draw();
	m_CancelButtonFrameSprite.Draw();
	m_CancelButtonEffectSprite.Draw();
	for (int i = 0; i < 12; ++i)
	{
		m_LevelButtonSprite[i].color = 0xffffffff;
		m_LevelButtonSprite[i].blend_mode = aqua::ALPHABLEND::ALPHA;
		m_LevelButtonSprite[i].Draw();
		m_LevelButtonLitSprite[i].Draw();
	}
	m_LevelButtonFrameSprite.Draw();
	m_LevelButtonEffectSprite.Draw();
	m_pEffectManager->DrawEffects();
	m_ChangeSceneEffectSprite_Dest.Draw();
}

void CLevelSelect::Finalize()
{
	IGameObject::Finalize();

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
	m_ChangeSceneEffectSprite.Delete();
	m_ChangeSceneEffectSurface.Delete();
	m_ChangeSceneEffectSprite_Dest.Delete();
}

void CLevelSelect::LoadClearedLevel()
{
	std::ifstream ifs(m_pCommonData->GetSaveDataFileName());

	// セーブデータが存在する場合
	if (!ifs.fail())
		ifs >> m_ClearedLevel;

	// セーブデータが存在しない場合
	else
		m_ClearedLevel = 0;

	// ファイルを閉じる
	ifs.close();
}
