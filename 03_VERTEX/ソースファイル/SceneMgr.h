#pragma once

#include "System.h"

class cBaseScene;

/* 描画先スクリーンの種類 */
enum class eSCREEN
{
	DEFAULT,	// デフォルト
	FADE_ADD,	// フェード加算

	MAX
};

/* シーン管理クラス */
class cSceneMgr
{
	/* 公開関数 */
public:
	static void CreateInstance();		// インスタンスの生成
	static void DestroyInstance();		// インスタンスの解放
	static cSceneMgr& GetInstance();	// インスタンスの取得

	void Update();	// 更新
	void Draw();	// 描画

	bool CheckExitGame();				// ゲーム終了判定
	void ChangeScreen(eSCREEN Screen);	// 描画先スクリーンの変更

	/* 非公開関数 */
private:
	::DISALLOW_COPY_CONSTRUCTOR(cSceneMgr);
	::DISALLOW_MOVE_CONSTRUCTOR(cSceneMgr);

	cSceneMgr();	// コンストラクタ
	~cSceneMgr();	// デストラクタ

	/* 非公開変数 */
private:
	static cSceneMgr* m_pInstance;	// インスタンスのポインタ

	cBaseScene* m_pNowScene;			// 現在のシーン
	cBaseScene* m_pNextScene;			// 次のシーン
	bool m_ExitGameFlag;				// ゲーム終了
	int m_Screen[(int)eSCREEN::MAX];	// スクリーンハンドル
};
