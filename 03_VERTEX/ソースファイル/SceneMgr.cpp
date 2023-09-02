//////////////////////
//   プリプロセス   //
//////////////////////
#include "SceneMgr.h"
#include "DxLib.h"
#include "CommonData.h"
#include "SoundData.h"
#include "BgmData.h"
#include "EffectMgr.h"

#include "Title.h"
#include "Config.h"
#include "Select.h"
#include "Game.h"
#include "Result.h"

#include "Input.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
cSceneMgr* cSceneMgr::m_pInstance(nullptr);	// インスタンスのポインタ

//////////////////
//   公開関数   //
//////////////////

/* インスタンスの生成 */
void cSceneMgr::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cSceneMgr;
}

/* インスタンスの解放 */
void cSceneMgr::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

/* インスタンスの取得 */
cSceneMgr& cSceneMgr::GetInstance()
{
	return *m_pInstance;
}


/* コンストラクタ */
cSceneMgr::cSceneMgr()
	: m_pNowScene(nullptr)
	, m_pNextScene(nullptr)
	, m_ExitGameFlag(false)
{
	for (int i = 0; i < (int)eSCREEN::MAX; ++i) {
		m_Screen[i] = MakeScreen(Sw, Sh);
		SetDrawScreen(m_Screen[i]);
		ClearDrawScreen();
	}
	SetDrawScreen(DX_SCREEN_BACK);

	cSoundData::CreateInstance();
	cBgmData::CreateInstance();

	cCommonData::Init();
	m_pNowScene = m_pNextScene = new cTitle();
}

/* デストラクタ */
cSceneMgr::~cSceneMgr()
{
	for (int i = 0; i < (int)eSCREEN::MAX; ++i)
		DeleteGraph(m_Screen[i]);

	cSoundData::DestroyInstance();
	cBgmData::DestroyInstance();

	cCommonData::Fin();

	if (m_pNowScene != m_pNextScene) {
		delete m_pNowScene;
		if (m_pNextScene)
			delete m_pNextScene;
	}
	else {
		delete m_pNowScene;
	}
}

/* 更新 */
void cSceneMgr::Update()
{
	if (m_pNextScene != m_pNowScene)
	{
		delete m_pNowScene;
		m_pNowScene = m_pNextScene;
	}

	m_pNextScene = m_pNowScene->Update();
}

/* 描画 */
void cSceneMgr::Draw()
{
	SetDrawScreen(m_Screen[(int)eSCREEN::FADE_ADD]);
	SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
	DrawBox(0, 0, Sw, Sh, 0x202020, TRUE);

	SetDrawScreen(m_Screen[(int)eSCREEN::DEFAULT]);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	ClearDrawScreen();
	m_pNowScene->Draw();
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawGraph(0, 0, m_Screen[(int)eSCREEN::FADE_ADD], FALSE);

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawExtendGraph(0, 0, Sw * SScale, Sh * SScale, m_Screen[(int)eSCREEN::DEFAULT], FALSE);

	if (!m_pNextScene)
		m_ExitGameFlag = true;
}

/* ゲーム終了判定 */
bool cSceneMgr::CheckExitGame()
{
	return m_ExitGameFlag;
}

/* 描画先スクリーンの変更 */
void cSceneMgr::ChangeScreen(eSCREEN Screen)
{
	if (Screen == eSCREEN::MAX)
		return;

	SetDrawScreen(m_Screen[(int)Screen]);
}
