#include "game_main.h"
#include "../../../common_data/common_data.h"
#include "../../../back_ground/back_ground.h"
#include "../../../fade_screen/fade_screen.h"
#include "../../../effect_manager/effect_manager.h"
#include "../../../sound_manager/sound_manager.h"
#include "../../../field/field.h"
#include <fstream>

const float				CGameMain::m_button_click_time = 0.4f;
const aqua::CVector2	CGameMain::m_cancel_button_position = aqua::CVector2(40.0f, 40.0f);
const aqua::CVector2	CGameMain::m_cancel_button_size = aqua::CVector2(120.0f, 96.0f);
constexpr int			CGameMain::m_typing_num;
const float				CGameMain::m_successed_field_edge_space = 160.0f;
const float				CGameMain::m_success_time = 2.0f;
const float				CGameMain::m_typing_wait_time = 1.0f;
const float				CGameMain::m_min_typing_interval = 0.04f;
const float				CGameMain::m_max_typing_interval = 0.16f;
const float				CGameMain::m_min_create_particle_time = 0.02f;
const float				CGameMain::m_max_create_particle_time = 0.08f;
const std::string		CGameMain::m_save_data_file = "data\\savedata.txt";

CGameMain::CGameMain(IGameObject* parent)
	: IScene(parent, "GameMain")
	, m_pCommonData(nullptr)
	, m_pBackGround(nullptr)
	, m_pFadeScreen(nullptr)
	, m_pEffectManager(nullptr)
	, m_pSoundManager(nullptr)
	, m_pField(nullptr)
	, m_State(STATE_ID::START)
	, m_TypingCount(0)
	, m_IsOverCancelButton(false)
{
}

void CGameMain::Initialize()
{
	// ゲームオブジェクトの取得
	m_pCommonData = (CCommonData*)aqua::FindGameObject("CommonData");
	m_pBackGround = (CBackGround*)aqua::FindGameObject("BackGround");
	m_pFadeScreen = (CFadeScreen*)aqua::FindGameObject("FadeScreen");
	m_pEffectManager = (CEffectManager*)aqua::FindGameObject("EffectManager");
	m_pSoundManager = (CSoundManager*)aqua::FindGameObject("SoundManager");

	// ゲームオブジェクトの生成
	m_pField = aqua::CreateGameObject<CField>(this);
	m_pField->Initialize();

	// リソースの読み込み
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

	for (int i = 0; i < m_typing_num; ++i)
	{
		switch (i)
		{
		case 0:	m_SuccessTextSprite[i].Create("data\\success_s.png");	break;
		case 1:	m_SuccessTextSprite[i].Create("data\\success_u.png");	break;
		case 2:	m_SuccessTextSprite[i].Create("data\\success_c.png");	break;
		case 3:	m_SuccessTextSprite[i].Create("data\\success_c.png");	break;
		case 4:	m_SuccessTextSprite[i].Create("data\\success_e.png");	break;
		case 5:	m_SuccessTextSprite[i].Create("data\\success_s.png");	break;
		case 6:	m_SuccessTextSprite[i].Create("data\\success_s.png");	break;
		}
		m_SuccessTextSprite[i].scale.x = 6.0f;
		m_SuccessTextSprite[i].scale.y = 6.0f;
		m_SuccessTextSprite[i].position.x = 100.0f + 140.0f * i;
		m_SuccessTextSprite[i].position.y = 200.0f;
		m_SuccessTextSprite[i].visible = false;
	}
	m_SuccessBarSprite.Create("data\\success_bar.png");
	m_SuccessBarSprite.scale.x = 6.0f;
	m_SuccessBarSprite.scale.y = 6.0f;
	m_SuccessBarSprite.visible = false;

	// 初期化処理
	m_ButtonClickTimer.Setup(m_button_click_time);
	m_SuccessTimer.Setup(m_success_time);
	m_TypingTimer.Setup(m_typing_wait_time);
	m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);
	m_pFadeScreen->SetFade(false);
}

void CGameMain::Update()
{
	// フィールドの更新
	m_pField->Update();

	// 状態に応じて処理の分岐
	switch (m_State)
	{
	case STATE_ID::START:
		m_State = STATE_ID::GAMEPLAY;
		break;

	case STATE_ID::CANCEL:
		m_ButtonClickTimer.Update();
		m_CancelButtonEffectSprite.color.alpha = (unsigned char)(max(1.0f - m_ButtonClickTimer.GetTime(), 0.0f) * 255.0f);
		if (m_ButtonClickTimer.Finished())
			m_pFadeScreen->SetFade(true);
		break;

	case STATE_ID::GAMEPLAY:
	{
		// キャンセルボタン判定
		bool prev_cursor = m_IsOverCancelButton;
		aqua::CVector2 mpos = aqua::mouse::GetCursorPos();
		m_IsOverCancelButton = (
			mpos.x >= m_cancel_button_position.x &&
			mpos.y >= m_cancel_button_position.y &&
			mpos.x < m_cancel_button_position.x + m_cancel_button_size.x &&
			mpos.y < m_cancel_button_position.y + m_cancel_button_size.y);
		if (m_IsOverCancelButton && !prev_cursor)
		{
			m_pSoundManager->Play(SOUND_ID::BUTTON_CURSOR_SE);
		}
		if (m_IsOverCancelButton && aqua::mouse::Trigger(aqua::mouse::BUTTON_ID::LEFT))
		{
			m_State = STATE_ID::CANCEL;
			m_CancelButtonEffectSprite.visible = true;
			m_pSoundManager->Play(SOUND_ID::BUTTON_CLICK_SE);
			break;
		}

		// 成功判定
		if (m_pField->IsSuccess())
		{
			SaveClearLevel();
			m_IsOverCancelButton = false;
			m_SuccessBarSprite.position = m_SuccessTextSprite[0].position;
			m_SuccessBarSprite.visible = true;
			m_pField->SetTargetEdgeSpace(m_successed_field_edge_space);
			for (int i = 0; i < 50; ++i)
				m_pEffectManager->CreateEffect_RandomLine();
			m_pSoundManager->Play(SOUND_ID::SUCCESS_SE);
			m_State = STATE_ID::SUCCESS;
		}
	}
	break;

	case STATE_ID::SUCCESS:
		if (m_TypingCount < m_typing_num)
		{
			m_TypingTimer.Update();
			if (m_TypingTimer.Finished())
			{
				m_SuccessTextSprite[m_TypingCount].visible = true;
				++m_TypingCount;
				if (m_TypingCount < m_typing_num)
					m_SuccessBarSprite.position = m_SuccessTextSprite[m_TypingCount].position;
				else
					m_SuccessBarSprite.visible = false;
				m_TypingTimer.Setup(m_min_typing_interval + (m_max_typing_interval - m_min_typing_interval) * aqua::Rand(1000) / 1000.0f);
				switch (aqua::Rand(2))
				{
				case 0:	m_pSoundManager->Play(SOUND_ID::TYPING_SE_1);	break;
				case 1:	m_pSoundManager->Play(SOUND_ID::TYPING_SE_2);	break;
				case 2:	m_pSoundManager->Play(SOUND_ID::TYPING_SE_3);	break;
				}
			}
		}

		m_SuccessTimer.Update();
		if (m_SuccessTimer.Finished())
			m_pFadeScreen->SetFade(true);
		break;
	}

	// キャンセルボタンの更新
	m_pField->SetControlFlag(!m_IsOverCancelButton);
	m_CancelButtonFrameSprite.visible = m_IsOverCancelButton;
	if (m_IsOverCancelButton)
		m_CancelButtonLitSprite.Update();
	else
		m_CancelButtonLitSprite.ResetAnimationFrame();

	// シーン切り替え処理
	if (m_pFadeScreen->IsFinish())
		ChangeScene(SCENE_ID::LEVEL_SELECT);

	// パーティクル生成の更新
	m_CreateParticleTimer.Update();
	if (m_CreateParticleTimer.Finished())
	{
		m_pEffectManager->CreateEffect_RandomParticle();
		m_CreateParticleTimer.Setup(m_min_create_particle_time + (m_max_create_particle_time - m_min_create_particle_time) * aqua::Rand(1000) / 1000.0f);
	}

	// 背景色の更新
	m_pBackGround->paramR = 0.10f;
	m_pBackGround->paramG = 0.15f;
	m_pBackGround->paramB = 0.20f;
}

void CGameMain::Draw()
{
	// フィールド
	m_pField->Draw();

	// 戻るボタン
	m_CancelButtonSprite.color = 0xffffffff;
	m_CancelButtonSprite.Draw();
	m_CancelButtonLitSprite.Draw();
	m_CancelButtonFrameSprite.Draw();
	m_CancelButtonEffectSprite.Draw();

	// 成功演出
	for (int i = 0; i < m_typing_num; ++i)
	{
		m_SuccessTextSprite[i].color = 0xffffffff;
		m_SuccessTextSprite[i].Draw();
	}
	m_SuccessBarSprite.color = 0xffffffff;
	m_SuccessBarSprite.Draw();
}

void CGameMain::Draw_Lit()
{
	// フィールド
	m_pField->Draw_Lit();

	// 戻るボタン
	m_CancelButtonSprite.color = 0xff000000;
	m_CancelButtonSprite.Draw();
	m_CancelButtonLitSprite.Draw();
	m_CancelButtonEffectSprite.Draw();

	// 成功演出
	for (int i = 0; i < m_typing_num; ++i)
	{
		m_SuccessTextSprite[i].color = 0xff705020;
		m_SuccessTextSprite[i].Draw();
	}
	m_SuccessBarSprite.color = 0xff705020;
	m_SuccessBarSprite.Draw();
}

void CGameMain::Finalize()
{
	IGameObject::Finalize();

	m_CancelButtonSprite.Delete();
	m_CancelButtonLitSprite.Delete();
	m_CancelButtonFrameSprite.Delete();
	m_CancelButtonEffectSprite.Delete();
	for (int i = 0; i < m_typing_num; ++i)
		m_SuccessTextSprite[i].Delete();
	m_SuccessBarSprite.Delete();
}

void CGameMain::SaveClearLevel()
{
	// クリア済みレベルを取得
	int cleared_level = 0;
	{
		std::ifstream ifs(m_save_data_file);
		if (ifs)
			ifs >> cleared_level;

		ifs.close();
	}

	// クリア済みレベルと選択レベルを比較し、高いほうを記録
	std::ofstream ofs(m_save_data_file);
	ofs.clear();
	ofs << max(m_pCommonData->selectedLevel, cleared_level) << std::endl;
}
