#pragma once

#include "../scene.h"

class CCommonData;
class CBackGround;
class CFadeScreen;
class CEffectManager;
class CSoundManager;
class CLamp;

class CLevelSelect :
	public IScene
{
public:
	CLevelSelect(IGameObject* parent);
	~CLevelSelect() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Draw_Lit() override;
	void Finalize() override;

private:
	void LoadClearedLevel();	// クリア済みレベルの読み込み

private:
	static const aqua::CVector2 m_cancel_button_position;	// 戻るボタンの座標
	static const aqua::CVector2 m_cancel_button_size;		// 戻るボタンの大きさ
	static const aqua::CVector2 m_level_button_size;		// レベルボタンの大きさ
	static const float m_button_click_time;					// ボタン押下時の演出時間
	static const float m_min_create_particle_time;			// 光の粒子エフェクトの最低生成間隔
	static const float m_max_create_particle_time;			// 光の粒子エフェクトの最大生成間隔
	static const std::string m_save_data_file;				// セーブデータのファイル名

	CCommonData* m_pCommonData;			// 共有データオブジェクト
	CBackGround* m_pBackGround;			// 背景オブジェクト
	CFadeScreen* m_pFadeScreen;			// フェード演出スクリーンオブジェクト
	CEffectManager* m_pEffectManager;	// エフェクト管理オブジェクト
	CSoundManager* m_pSoundManager;		// サウンド管理オブジェクト

	std::list<CLamp*> m_pLamps;					// 背景のランプ
	int m_ClearedLevel;							// 最高クリア済みレベル番号
	int m_ButtonCursor;							// マウスオーバー中のボタンID（[-1]:未選択   [0]:戻るボタン   [1~12]:レベルボタン）
	int m_ButtonCursor_prev;					// マウスオーバー中のボタンID・1フレーム前
	bool m_IsButtonClicked;						// ボタン押下判定
	aqua::CVector2 m_LevelButtonPosition[12];	// レベルボタンの座標
	aqua::CTimer m_ButtonClickTimer;			// ボタン押下時の演出時間
	aqua::CTimer m_CreateParticleTimer;			// 光の粒子エフェクトの生成時間

	aqua::CSprite m_CancelButtonSprite;					// 戻るボタンの画像
	aqua::CAnimationSprite m_CancelButtonLitSprite;		// 戻るボタンの発光画像
	aqua::CSprite m_CancelButtonFrameSprite;			// 戻るボタンの縁画像
	aqua::CSprite m_CancelButtonEffectSprite;			// 戻るボタンの演出用画像
	aqua::CAnimationSprite m_LevelButtonSprite[12];		// レベルボタンの画像
	aqua::CAnimationSprite m_LevelButtonLitSprite[12];	// レベルボタンの発光画像
	aqua::CSprite m_LevelButtonFrameSprite;				// レベルボタンの縁画像
	aqua::CSprite m_LevelButtonEffectSprite;			// レベルボタンの演出用画像
};
