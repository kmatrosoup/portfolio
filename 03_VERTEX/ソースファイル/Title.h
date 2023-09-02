#pragma once

#include "BaseScene.h"
#include "VEC2.h"

class cTitle : public cBaseScene
{
	/* 列挙型 */
private:
	enum class eTITLE_MENU
	{
		PVP,
		CONFIG,

		MAX
	};

	/* 公開関数 */
public:
	cTitle();						// コンストラクタ
	~cTitle() override;				// デストラクタ
	cBaseScene* Update() override;	// 更新
	void Draw() const override;		// 描画

	/* 非公開関数 */
private:
	cTitle(const cTitle&) = delete;
	cTitle& operator=(const cTitle&) = delete;
	cTitle(cTitle&&) = delete;
	cTitle& operator=(cTitle&&) = delete;

	/* 非公開変数 */
private:
	static eTITLE_MENU m_Cursor;		// カーソル・メニュー
	static int m_CursorPvP;				// カーソル・PvP
	static const int m_MaxTime;			// 最大時間
	static const int m_MaxStartTime;	// 最大開始時間
	static const VEC2 m_TitlePos;		// タイトルの位置

	bool m_SelectedPvP;		// プレイ人数選択
	int m_Timer;			// 経過時間
	int m_StartTimer;		// 開始時間

	int m_Image_Title;							// 画像ハンドル・タイトル
	int m_Image_TitleText[7];					// 画像ハンドル・タイトル文字
	int m_Image_Menu[(int)eTITLE_MENU::MAX];	// 画像ハンドル・メニュー
};
