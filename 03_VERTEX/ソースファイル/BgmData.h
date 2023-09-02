#pragma once

#include "System.h"

/* BGMデータ */
enum class eBGM
{
	TITLE,		// タイトル画面
	PREVIEW,	// プレビュー画面
	SELECT,		// ステータス画面
	BATTLE,		// バトル画面
	RESULT,		// リザルト画面

	MAX
};

/* BGMデータ管理クラス */
class cBgmData
{
	/* 公開関数 */
public:
	static void CreateInstance();	// インスタンスの生成
	static void DestroyInstance();	// インスタンスの解放
	static cBgmData& GetInstance();	// インスタンスの取得

	void Play(eBGM Bgm, bool TopPositionFlag);	// 再生
	void Stop(eBGM Bgm);						// 停止

	/* 非公開関数 */
private:
	::DISALLOW_COPY_CONSTRUCTOR(cBgmData);
	::DISALLOW_MOVE_CONSTRUCTOR(cBgmData);

	cBgmData();	// コンストラクタ
	~cBgmData();	// デストラクタ

	/* 非公開変数 */
private:
	static cBgmData* m_pInstance;	// インスタンスのポインタ

	int m_BgmHandle[(int)eBGM::MAX];	// BGMハンドル
};
