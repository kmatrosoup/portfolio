#pragma once

#include "../scene.h"

class CCommonData;
class CEffectManager;
class CSoundManager;
class CGlowScreen;

class CTitle :
	public IScene
{
public:
	CTitle(IGameObject* parent);
	~CTitle() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	static const int m_start_button_pos_y;
	static const int m_exit_button_pos_y;
	static const int m_button_height;
	static const float m_min_part_time;
	static const float m_max_part_time;
	static const float m_spot_light_time;
	static const unsigned int m_logo_color_table[7];
	static const float m_button_click_time;
	static const float m_min_create_particle_time;
	static const float m_max_create_particle_time;
	static const float m_change_scene_time;

	CCommonData* m_pCommonData;
	CEffectManager* m_pEffectManager;
	CSoundManager* m_pSoundManager;
	CGlowScreen* m_pGlowScreen;

	int m_ButtonCursor;			// [-1]:未選択   [0]:スタート   [1]:終了
	int m_ButtonCursor_prev;
	bool m_ButtonClickedFlag;
	bool m_ChangeSceneFlag;
	aqua::CTimer m_LogoPartTimer[7];
	aqua::CTimer m_ButtonClickTimer;
	aqua::CTimer m_SpotLightTimer;
	aqua::CTimer m_CreateParticleTimer;
	aqua::CTimer m_ChangeSceneTimer;

	aqua::CSprite m_LogoBackSprite;
	aqua::CSprite m_LogoPartSprite[7];
	aqua::CSprite m_StartButtonSprite;
	aqua::CSprite m_StartButtonInvSprite;
	aqua::CSprite m_ExitButtonSprite;
	aqua::CSprite m_ExitButtonInvSprite;
	aqua::CSprite m_SpotLightSprite;
	aqua::CSprite m_ChangeSceneEffectSprite;
	aqua::CSurface m_ChangeSceneEffectSurface;
	aqua::CSprite m_ChangeSceneEffectSprite_Dest;
};
