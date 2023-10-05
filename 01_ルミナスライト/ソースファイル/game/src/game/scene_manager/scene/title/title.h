#pragma once

#include "../scene.h"

class CBackGround;
class CFadeScreen;
class CEffectManager;
class CSoundManager;

class CTitle :
	public IScene
{
public:
	CTitle(IGameObject* parent);
	~CTitle() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Draw_Lit() override;
	void Finalize() override;

private:
	static const int m_start_button_pos_y;				// スタートボタンのY座標
	static const int m_exit_button_pos_y;				// 終了ボタンのY座標
	static const int m_button_height;					// ボタンの高さ
	static const float m_min_part_time;					// タイトル文字の最速点灯時間
	static const float m_max_part_time;					// タイトル文字の最遅点灯時間
	static const float m_spot_light_time;				// スポットライトの点灯時間
	static const unsigned int m_logo_color_table[7];	// タイトル文字の色コード
	static const float m_button_click_time;				// ボタン押下時の演出時間
	static const float m_min_create_particle_time;		// 光の粒子エフェクトの最低生成間隔
	static const float m_max_create_particle_time;		// 光の粒子エフェクトの最大生成間隔

	CBackGround* m_pBackGround;			// 背景オブジェクト
	CFadeScreen* m_pFadeScreen;			// フェード演出スクリーンオブジェクト
	CEffectManager* m_pEffectManager;	// エフェクト管理オブジェクト
	CSoundManager* m_pSoundManager;		// サウンド管理オブジェクト

	int m_ButtonCursor;					// マウスオーバー中のボタンID（[-1]:未選択 [0]:スタート [1]:終了）
	int m_ButtonCursor_prev;			// マウスオーバー中のボタンID・1フレーム前
	bool m_ButtonClickedFlag;			// ボタン押下判定
	bool m_IsPlayedLightUpSE;			// スポットライト点灯SEの再生済み判定
	aqua::CTimer m_LogoPartTimer[7];	// タイトル文字の点灯時間
	aqua::CTimer m_ButtonClickTimer;	// ボタン押下時の演出時間
	aqua::CTimer m_SpotLightTimer;		// スポットライトの点灯時間
	aqua::CTimer m_CreateParticleTimer;	// 光の粒子エフェクトの生成時間

	aqua::CSprite m_LogoBackSprite;					// タイトル文字の背景画像
	aqua::CSprite m_LogoPartSprite[7];				// タイトル文字の分割画像
	aqua::CSprite m_StartButtonSprite;				// スタートボタンの画像
	aqua::CSprite m_StartButtonInvSprite;			// スタートボタンの画像（明度反転）
	aqua::CSprite m_ExitButtonSprite;				// 終了ボタンの画像
	aqua::CSprite m_ExitButtonInvSprite;			// 終了ボタンの画像（明度反転）
	aqua::CSprite m_SpotLightSprite;				// スポットライトの画像
};
