#pragma once

#include "../scene.h"

class CCommonData;
class CField;
class CEffectManager;
class CSoundManager;
class CGlowScreen;

class CGameMain :
	public IScene
{
public:
	CGameMain(IGameObject* parent);
	~CGameMain() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	void SaveClearLevel();

private:
	enum class STATE_ID
	{
		START,
		CANCEL,
		GAMEPLAY,
		SUCCESS,
	};

private:
	static const float m_button_click_time;
	static const aqua::CVector2 m_cancel_button_position;
	static const aqua::CVector2 m_cancel_button_size;
	static const int m_cancel_button_frame_num;
	static const float m_cancel_button_animation_time;
	static constexpr int m_typing_num = 7;
	static const float m_successed_field_edge_space;
	static const float m_success_time;
	static const float m_typing_wait_time;
	static const float m_min_typing_interval;
	static const float m_max_typing_interval;
	static const float m_min_create_particle_time;
	static const float m_max_create_particle_time;
	static const float m_change_scene_time;

	CCommonData* m_pCommonData;
	CField* m_pField;
	CEffectManager* m_pEffectManager;
	CSoundManager* m_pSoundManager;
	CGlowScreen* m_pGlowScreen;

	STATE_ID m_State;
	aqua::CTimer m_ButtonClickTimer;
	bool m_CancelButtonCursorFlag;
	aqua::CTimer m_CancelButtonTimer;
	aqua::CTimer m_SuccessTimer;
	aqua::CTimer m_TypingTimer;
	int m_TypingCount;
	aqua::CTimer m_CreateParticleTimer;
	bool m_ChangeSceneFlag;
	aqua::CTimer m_ChangeSceneInTimer;
	aqua::CTimer m_ChangeSceneOutTimer;

	aqua::CSprite m_CancelButtonSprite;
	aqua::CSprite m_CancelButtonLitSprite;
	aqua::CSprite m_CancelButtonFrameSprite;
	aqua::CSprite m_CancelButtonEffectSprite;
	aqua::CSprite m_SuccessTextSprite[m_typing_num];
	aqua::CSprite m_SuccessBarSprite;
	aqua::CSprite m_ChangeSceneEffectSprite;
	aqua::CSurface m_ChangeSceneEffectSurface;
	aqua::CSprite m_ChangeSceneEffectSprite_Dest;
};
