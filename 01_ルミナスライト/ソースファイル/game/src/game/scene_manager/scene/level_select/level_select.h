#pragma once

#include "../scene.h"

class CCommonData;
class CEffectManager;
class CSoundManager;
class CGlowScreen;

class CLevelSelect :
	public IScene
{
public:
	CLevelSelect(IGameObject* parent);
	~CLevelSelect() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	void LoadClearedLevel();

private:
	static const aqua::CVector2 m_cancel_button_position;
	static const aqua::CVector2 m_cancel_button_size;
	static const aqua::CVector2 m_level_button_size;
	static const float m_button_click_time;
	static const int m_cancel_button_frame_num;
	static const float m_cancel_button_animation_time;
	static const int m_level_button_frame_num;
	static const float m_level_button_animation_time;
	static const float m_min_create_particle_time;
	static const float m_max_create_particle_time;
	static const float m_change_scene_time;

	CCommonData* m_pCommonData;
	CEffectManager* m_pEffectManager;
	CSoundManager* m_pSoundManager;
	CGlowScreen* m_pGlowScreen;

	int m_ClearedLevel;
	int m_ButtonCursor;			// [-1]:未選択   [0]:キャンセルボタン   [1~12]:レベルボタン
	int m_ButtonCursor_prev;
	aqua::CVector2 m_LevelButtonPosition[12];
	bool m_ButtonClickedFlag;
	aqua::CTimer m_ButtonClickTimer;
	aqua::CTimer m_CancelButtonTimer;
	aqua::CTimer m_LevelButtonTimer[12];
	aqua::CTimer m_CreateParticleTimer;
	bool m_ChangeSceneFlag;
	aqua::CTimer m_ChangeSceneInTimer;
	aqua::CTimer m_ChangeSceneOutTimer;

	aqua::CSprite m_CancelButtonSprite;
	aqua::CSprite m_CancelButtonLitSprite;
	aqua::CSprite m_CancelButtonFrameSprite;
	aqua::CSprite m_CancelButtonEffectSprite;
	aqua::CSprite m_LevelButtonSprite[12];
	aqua::CSprite m_LevelButtonLitSprite[12];
	aqua::CSprite m_LevelButtonFrameSprite;
	aqua::CSprite m_LevelButtonEffectSprite;
	aqua::CSprite m_ChangeSceneEffectSprite;
	aqua::CSurface m_ChangeSceneEffectSurface;
	aqua::CSprite m_ChangeSceneEffectSprite_Dest;
};
