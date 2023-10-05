#pragma once

#include "../scene.h"

class CCommonData;
class CBackGround;
class CFadeScreen;
class CEffectManager;
class CSoundManager;
class CField;

class CGameMain :
	public IScene
{
public:
	CGameMain(IGameObject* parent);
	~CGameMain() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Draw_Lit() override;
	void Finalize() override;

private:
	void SaveClearLevel();	// クリアレベルの保存

private:
	enum class STATE_ID	// シーンの状態ID
	{
		START,		// 開始
		CANCEL,		// 戻る
		GAMEPLAY,	// パズル中
		SUCCESS,	// 成功
	};

private:
	static const float m_button_click_time;					// ボタン押下時の演出時間
	static const aqua::CVector2 m_cancel_button_position;	// 戻るボタンの座標
	static const aqua::CVector2 m_cancel_button_size;		// 戻るボタンの大きさ
	static constexpr int m_typing_num = 7;					// 成功演出のタイピング数
	static const float m_successed_field_edge_space;		// 成功時の画面端余白の大きさ
	static const float m_success_time;						// 成功演出の時間
	static const float m_typing_wait_time;					// 成功演出のタイピング開始時間
	static const float m_min_typing_interval;				// 成功演出の最低タイピング間隔
	static const float m_max_typing_interval;				// 成功演出の最大タイピング間隔
	static const float m_min_create_particle_time;			// 光の粒子エフェクトの最低生成間隔
	static const float m_max_create_particle_time;			// 光の粒子エフェクトの最大生成間隔
	static const std::string m_save_data_file;				// セーブデータのファイル名

	CCommonData* m_pCommonData;			// 共有データオブジェクト
	CBackGround* m_pBackGround;			// 背景オブジェクト
	CFadeScreen* m_pFadeScreen;			// フェード演出スクリーンオブジェクト
	CEffectManager* m_pEffectManager;	// エフェクト管理オブジェクト
	CSoundManager* m_pSoundManager;		// サウンド管理オブジェクト

	CField* m_pField;					// フィールドオブジェクト
	STATE_ID m_State;					// シーンの状態ID
	int m_TypingCount;					// 成功演出の現状タイプ数
	bool m_IsOverCancelButton;			// 戻るボタンマウスオーバー判定
	aqua::CTimer m_ButtonClickTimer;	// ボタン押下時の演出時間
	aqua::CTimer m_SuccessTimer;		// 成功演出の時間
	aqua::CTimer m_TypingTimer;			// 成功演出のタイプ時間
	aqua::CTimer m_CreateParticleTimer;	// 光の粒子エフェクトの生成時間

	aqua::CSprite m_CancelButtonSprite;					// 戻るボタンの画像
	aqua::CAnimationSprite m_CancelButtonLitSprite;		// 戻るボタンの発光画像
	aqua::CSprite m_CancelButtonFrameSprite;			// 戻るボタンの縁画像
	aqua::CSprite m_CancelButtonEffectSprite;			// 戻るボタンの演出用画像
	aqua::CSprite m_SuccessTextSprite[m_typing_num];	// 成功演出の文字画像
	aqua::CSprite m_SuccessBarSprite;					// 成功演出のタイプ位置画像
};
