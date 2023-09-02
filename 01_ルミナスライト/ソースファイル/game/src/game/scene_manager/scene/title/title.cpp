#include "title.h"
#include "../../../common_data/common_data.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../sound_manager/sound_manager.h"
#include "../../../glow_screen/glow_screen.h"

const int			CTitle::m_start_button_pos_y = 520;
const int			CTitle::m_exit_button_pos_y = 640;
const int			CTitle::m_button_height = 80;
const float			CTitle::m_min_part_time = 0.8f;
const float			CTitle::m_max_part_time = 1.6f;
const float			CTitle::m_spot_light_time = 1.18f;
const float			CTitle::m_button_click_time = 0.4f;
const float			CTitle::m_min_create_particle_time = 0.02f;
const float			CTitle::m_max_create_particle_time = 0.08f;
const float			CTitle::m_change_scene_time = 0.4f;
const unsigned int	CTitle::m_logo_color_table[7] =
{
	0xff802030,
	0xff602070,
	0xff106080,
	0xff603010,
	0xff309020,
	0xff606020,
	0xff202090,
};

CTitle::CTitle(IGameObject* parent)
	: IScene(parent, "Title")
	, m_pCommonData(nullptr)
	, m_pEffectManager(nullptr)
	, m_pSoundManager(nullptr)
	, m_pGlowScreen(nullptr)
	, m_ButtonCursor(-1)
	, m_ButtonCursor_prev(-1)
	, m_ButtonClickedFlag(false)
	, m_ChangeSceneFlag(false)
{
}

void CTitle::Initialize()
{
	Finalize();

	// ゲームオブジェクトの取得
	m_pCommonData = (CCommonData*)aqua::FindGameObject("CommonData");
	m_pEffectManager = (CEffectManager*)aqua::FindGameObject("EffectManager");
	m_pSoundManager = (CSoundManager*)aqua::FindGameObject("SoundManager");
	m_pGlowScreen = (CGlowScreen*)aqua::FindGameObject("GlowScreen");

	// リソースの読み込み
	m_LogoBackSprite.Create("data\\title_logo_back.png");
	m_LogoBackSprite.scale = aqua::CVector2::ONE * 8.0f;
	m_LogoBackSprite.position.x = (float)aqua::GetWindowWidth() / 2.0f - (float)m_LogoBackSprite.GetTextureWidth() * m_LogoBackSprite.scale.x / 2.0f;
	m_LogoBackSprite.position.y = 160.0f;

	for (int i = 0; i < 7; ++i)
	{
		m_LogoPartSprite[i].Create(std::string("data\\title_logo_") + std::to_string(i) + ".png");
		m_LogoPartSprite[i].scale = m_LogoBackSprite.scale;
		m_LogoPartSprite[i].position = m_LogoBackSprite.position;
		m_LogoPartSprite[i].visible = false;
	}

	m_StartButtonSprite.Create("data\\title_start.png");
	m_StartButtonSprite.scale = aqua::CVector2::ONE * 4.0f;
	m_StartButtonSprite.position.y = (float)m_start_button_pos_y;
	m_StartButtonSprite.visible = false;
	m_StartButtonInvSprite.Create("data\\title_start_inv.png");
	m_StartButtonInvSprite.scale = aqua::CVector2::ONE * 4.0f;
	m_StartButtonInvSprite.position.y = (float)m_start_button_pos_y;
	m_StartButtonInvSprite.rect.right = 0;

	m_ExitButtonSprite.Create("data\\title_exit.png");
	m_ExitButtonSprite.scale = aqua::CVector2::ONE * 4.0f;
	m_ExitButtonSprite.position.y = (float)m_exit_button_pos_y;
	m_ExitButtonSprite.visible = false;
	m_ExitButtonInvSprite.Create("data\\title_exit_inv.png");
	m_ExitButtonInvSprite.scale = aqua::CVector2::ONE * 4.0f;
	m_ExitButtonInvSprite.position.y = (float)m_exit_button_pos_y;
	m_ExitButtonInvSprite.rect.right = 0;

	m_SpotLightSprite.Create("data\\spot_light.png");
	m_SpotLightSprite.scale = aqua::CVector2::ONE * 4.0f;
	m_SpotLightSprite.blend_mode = aqua::ALPHABLEND::ADD;
	m_SpotLightSprite.visible = false;

	m_ChangeSceneEffectSprite.Create("data\\scene_change_effect.png");
	m_ChangeSceneEffectSprite.scale = aqua::CVector2::ONE * 40.0f;
	m_ChangeSceneEffectSurface.Create((int)aqua::GetWindowWidth(), (int)aqua::GetWindowHeight(), true);
	m_ChangeSceneEffectSprite_Dest.Create(m_ChangeSceneEffectSurface);

	// タイマーの設定
	for (int i = 0; i < 7; ++i)
		m_LogoPartTimer[i].Setup(m_min_part_time + (m_max_part_time - m_min_part_time) * aqua::Rand(1000) / 1000.0f);

	m_SpotLightTimer.Setup(m_spot_light_time);
	m_ButtonClickTimer.Setup(m_button_click_time);
	m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);
	m_ChangeSceneTimer.Setup(m_change_scene_time);

	// 開始処理
	m_pCommonData->SetBGParamR(0.0f);
	m_pCommonData->SetBGParamG(0.0f);
	m_pCommonData->SetBGParamB(0.0f);
	m_pEffectManager->ClearEffects();
	m_pSoundManager->Play(SOUND_ID::LIGHTUP_SE);
}

void CTitle::Update()
{
	// カーソルの更新
	m_ButtonCursor_prev = m_ButtonCursor;
	if (m_SpotLightTimer.Finished() && !m_ButtonClickedFlag)
	{
		aqua::CPoint mpos = aqua::mouse::GetCursorPos();
		m_ButtonCursor = -1;
		if (mpos.x >= 0 && mpos.x < (int)aqua::GetWindowWidth())
		{
			if (mpos.y >= m_start_button_pos_y && mpos.y < m_start_button_pos_y + m_button_height)	m_ButtonCursor = 0;
			if (mpos.y >= m_exit_button_pos_y && mpos.y < m_exit_button_pos_y + m_button_height)	m_ButtonCursor = 1;
		}

		if (m_ButtonCursor != -1 && m_ButtonCursor != m_ButtonCursor_prev)
			m_pSoundManager->Play(SOUND_ID::BUTTON_CURSOR_SE);
	}

	// クリック時の処理
	if (aqua::mouse::Trigger(aqua::mouse::BUTTON_ID::LEFT) && !m_ButtonClickedFlag)
		if (m_ButtonCursor != -1)
		{
			m_ButtonClickedFlag = true;
			m_pSoundManager->Play(SOUND_ID::BUTTON_CLICK_SE);
		}

	// ボタンクリック後の処理
	if (m_ButtonClickedFlag)
	{
		m_ButtonClickTimer.Update();
		if (m_ButtonClickTimer.Finished())
			m_ChangeSceneFlag = true;

		switch (m_ButtonCursor)
		{
		case 0:
			((int)(m_ButtonClickTimer.GetTime() * 16.0f) % 2 == 0) ?
				m_StartButtonInvSprite.blend_mode = aqua::ALPHABLEND::ADD :
				m_StartButtonInvSprite.blend_mode = aqua::ALPHABLEND::ALPHA;
		case 1:
			((int)(m_ButtonClickTimer.GetTime() * 16.0f) % 2 == 0) ?
				m_ExitButtonInvSprite.blend_mode = aqua::ALPHABLEND::ADD :
				m_ExitButtonInvSprite.blend_mode = aqua::ALPHABLEND::ALPHA;
		}
	}

	// ボタン演出後の処理
	if (m_ChangeSceneFlag)
	{
		m_ChangeSceneTimer.Update();
		if (m_ChangeSceneTimer.Finished())
			m_ChangeSceneTimer.SetTime(m_change_scene_time);

		if (m_ChangeSceneTimer.Finished())
		{
			switch (m_ButtonCursor)
			{
			case 0:
				ChangeScene(SCENE_ID::LEVEL_SELECT);
				break;
			case 1:
				aqua::ExitGame();
				break;
			}
		}
	}

	// パーティクル生成の更新
	m_CreateParticleTimer.Update();
	if (m_CreateParticleTimer.Finished())
	{
		m_pEffectManager->CreateEffect_RandomParticle();
		m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);
	}

	// 描画関係
	for (int i = 0; i < 7; ++i)
	{
		m_LogoPartTimer[i].Update();

		if (m_LogoPartTimer[i].Finished())
			m_LogoPartTimer[i].SetTime(m_LogoPartTimer[i].GetLimit());

		if (m_LogoPartTimer[i].GetLimit() - m_LogoPartTimer[i].GetTime() > m_min_part_time)
			m_LogoPartSprite[i].visible = false;
		else if (m_LogoPartTimer[i].GetLimit() - m_LogoPartTimer[i].GetTime() > m_min_part_time - 0.3f)
			m_LogoPartSprite[i].visible = ((int)(m_LogoPartTimer[i].GetTime() * 15.0f) % 2 == 0);
		else
			m_LogoPartSprite[i].visible = true;
	}

	if (!m_SpotLightTimer.Finished())
	{
		m_SpotLightTimer.Update();
		if (m_SpotLightTimer.GetTime() < 0.22f)			m_SpotLightSprite.visible = false;
		else if (m_SpotLightTimer.GetTime() < 0.25f)	m_SpotLightSprite.visible = true;
		else if (m_SpotLightTimer.GetTime() < 0.45f)	m_SpotLightSprite.visible = false;
		else if (m_SpotLightTimer.GetTime() < 0.47f)	m_SpotLightSprite.visible = true;
		else if (m_SpotLightTimer.GetTime() < 0.6f)		m_SpotLightSprite.visible = false;
		else if (m_SpotLightTimer.GetTime() < 0.63f)	m_SpotLightSprite.visible = true;
		else if (m_SpotLightTimer.GetTime() < 0.75f)	m_SpotLightSprite.visible = false;
		else if (m_SpotLightTimer.GetTime() < 0.77f)	m_SpotLightSprite.visible = true;
		else											m_SpotLightSprite.visible = false;

		if (m_SpotLightTimer.Finished())
		{
			m_SpotLightTimer.SetTime(m_spot_light_time);
			m_SpotLightSprite.visible = true;
			m_StartButtonSprite.visible = true;
			m_ExitButtonSprite.visible = true;
		}
	}

	m_ButtonCursor == 0 ?
		m_StartButtonInvSprite.rect.right += (int)ceilf(((float)m_StartButtonInvSprite.GetTextureWidth() + 20.0f - m_StartButtonInvSprite.rect.right) * 0.15f) :
		m_StartButtonInvSprite.rect.right += (int)floorf((-20.0f - m_StartButtonInvSprite.rect.right) * 0.1f);

	m_ButtonCursor == 1 ?
		m_ExitButtonInvSprite.rect.right += (int)ceilf(((float)m_ExitButtonInvSprite.GetTextureWidth() + 20.0f - m_ExitButtonInvSprite.rect.right) * 0.15f) :
		m_ExitButtonInvSprite.rect.right += (int)floorf((-20.0f - m_ExitButtonInvSprite.rect.right) * 0.1f);

	// シーン切り替え演出の更新
	m_ChangeSceneEffectSurface.Begin();
	aqua::Clear(0x00000000);
	m_ChangeSceneEffectSprite.Draw();
	m_ChangeSceneEffectSurface.End();
	DxLib::GraphFilter(m_ChangeSceneEffectSurface.GetTexture().GetResourceHandle(), DX_GRAPH_FILTER_TWO_COLOR,
		(int)(m_ChangeSceneTimer.GetTime() / m_change_scene_time * 255.0f),
		0xff000000,
		255,
		0xff000000,
		0);

	// グロー効果付与の描画
	m_pGlowScreen->Begin();
	{
		for (int i = 0; i < 7; ++i)
		{
			m_LogoPartSprite[i].color = m_logo_color_table[i];
			m_LogoPartSprite[i].Draw();
			m_LogoPartSprite[i].color = 0xffffffff;
		}
		if (m_ButtonClickedFlag)
		{
			switch (m_ButtonCursor)
			{
			case 0:
				m_StartButtonInvSprite.color = 0xffa08040;
				m_StartButtonInvSprite.color.alpha = (unsigned char)(max(1.0f - m_ButtonClickTimer.GetTime(), 0.0f) * 255.0f);
				m_StartButtonInvSprite.Draw();
				m_StartButtonInvSprite.color = 0xffffffff;
				break;
			case 1:
				m_ExitButtonInvSprite.color = 0xffa08040;
				m_ExitButtonInvSprite.color.alpha = (unsigned char)(max(1.0f - m_ButtonClickTimer.GetTime(), 0.0f) * 255.0f);
				m_ExitButtonInvSprite.Draw();
				m_ExitButtonInvSprite.color = 0xffffffff;
				break;
			}
		}
		m_SpotLightSprite.Draw();
		m_pEffectManager->DrawEffects();
		m_ChangeSceneEffectSprite_Dest.Draw();
	}
	m_pGlowScreen->End();

	// 背景色の更新
	if (!m_SpotLightTimer.Finished())
	{
		m_pCommonData->SetBGParamR(0.0f);
		m_pCommonData->SetBGParamG(0.0f);
		m_pCommonData->SetBGParamB(0.0f);
	}
	else
	{
		m_pCommonData->SetBGParamR(m_pCommonData->GetBGParamR() + (0.10 - m_pCommonData->GetBGParamR()) * 0.1f);
		m_pCommonData->SetBGParamG(m_pCommonData->GetBGParamG() + (0.10 - m_pCommonData->GetBGParamG()) * 0.1f);
		m_pCommonData->SetBGParamB(m_pCommonData->GetBGParamB() + (0.10 - m_pCommonData->GetBGParamB()) * 0.1f);
	}
}

void CTitle::Draw()
{
	m_LogoBackSprite.Draw();
	for (int i = 0; i < 7; ++i)
		m_LogoPartSprite[i].Draw();

	m_StartButtonSprite.Draw();
	m_StartButtonInvSprite.Draw();
	m_ExitButtonSprite.Draw();
	m_ExitButtonInvSprite.Draw();
	m_SpotLightSprite.Draw();
	m_pEffectManager->DrawEffects();
	m_ChangeSceneEffectSprite_Dest.Draw();
}

void CTitle::Finalize()
{
	m_LogoBackSprite.Delete();
	for (int i = 0; i < 7; ++i)
		m_LogoPartSprite[i].Delete();
	m_StartButtonSprite.Delete();
	m_StartButtonInvSprite.Delete();
	m_ExitButtonSprite.Delete();
	m_ExitButtonInvSprite.Delete();
	m_SpotLightSprite.Delete();
	m_ChangeSceneEffectSprite.Delete();
	m_ChangeSceneEffectSurface.Delete();
	m_ChangeSceneEffectSprite_Dest.Delete();
}
