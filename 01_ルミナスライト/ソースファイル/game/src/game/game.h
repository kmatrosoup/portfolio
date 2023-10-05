#pragma once

#include "aqua.h"

class CCommonData;
class CBackGround;
class CSceneManager;
class CEffectManager;
class CFadeScreen;
class CGlowScreen;
class CSoundManager;

class CGame
	: public aqua::IGameObject
{
public:
	CGame(aqua::IGameObject* parent);
	~CGame(void) = default;

	void Initialize(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	CCommonData* m_pCommonData;			// 共有データオブジェクト
	CBackGround* m_pBackGround;			// 背景オブジェクト
	CSceneManager* m_pSceneManager;		// シーン管理オブジェクト
	CEffectManager* m_pEffectManager;	// エフェクト管理オブジェクト
	CFadeScreen* m_pFadeScreen;			// フェード演出スクリーンオブジェクト
	CGlowScreen* m_pGlowScreen;			// 発光スクリーンオブジェクト
	CSoundManager* m_pSoundManager;		// サウンド管理オブジェクト
};
