#pragma once

#include "BaseScene.h"
#include "InputMgr.h"
#include "Player.h"
#include <vector>
#include <list>
#include <string>

class cSelect : public cBaseScene
{
	/* 列挙定数 */
private:
	enum class eMENU_TYPE	// メニューの種類
	{
		STATUS,
		BULLET,
		SKILL,
		ABILITY,

		MAX
	};

	enum class eACTIVE_BUTTON	// 使用するボタンの種類
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DECIDE,
		CANCEL,

		MAX
	};

	/* 構造体 */
private:
	struct sStatus
	{
		std::vector<int> NeedCell;
		int MaxCount;
		std::string Name;
		std::string Lore;

		sStatus(std::vector<int> NeedCell, int MaxCount, std::string Name, std::string Lore)	// コンストラクタ
			: NeedCell(NeedCell)
			, MaxCount(MaxCount)
			, Name(Name)
			, Lore(Lore)
		{}
	};

	/* 公開関数 */
public:
	cSelect();						// コンストラクタ
	~cSelect() override;			// デストラクタ
	cBaseScene* Update() override;	// 更新
	void Draw() const override;		// 描画

	/* 非公開関数 */
private:
	cSelect(const cSelect&) = delete;
	cSelect& operator=(const cSelect&) = delete;
	cSelect(cSelect&&) = delete;
	cSelect& operator=(cSelect&&) = delete;

	void m_DrawIcon(int x, int y, eMENU_TYPE menu, int id, bool calm_flag) const;	// アイコンの描画
	void m_DrawCustomArea(int x, int y, int player, bool inversion) const;			// カスタマイズ領域の描画
	void m_DrawLoreArea(int x, int y, int player) const;							// アイコン説明領域の描画

	/* 非公開変数 */
private:
	static const std::vector<sStatus>	m_Menu[(int)eMENU_TYPE::MAX];				// メニュー
	static const eINPUT_BUTTON			m_ButtonHandle[(int)eACTIVE_BUTTON::MAX];	// 対応するボタン

	int				m_RestCell[4];			// 残りのポイント数
	eMENU_TYPE		m_CursorType[4];		// 現在のカーソルの種類
	int				m_CursorPos[4];			// 現在のカーソルの位置
	std::list<int>	m_SelectedStatus[4];	// 選択中のステータス
	eBULLET			m_SelectedBullet[4];	// 選択中の弾
	eSKILL			m_SelectedSkill[4];		// 選択中のスキル
	eABILITY		m_SelectedAbility[4];	// 選択中のアビリティ
	eACTIVE_BUTTON	m_HoldButton[4];		// 長押し中のボタン
	int				m_HoldTime[4];			// 長押し時間
	bool			m_Ready[4];				// 準備完了
	int				m_StartTimer;			// 開始時間
	int				m_BrightTimer;			// 明滅時間
	int				m_EntryTimer;			// 入場時間

	int m_Image_Cell;		// 画像ハンドル・ポイント
	int m_Image_IconFrame;	// 画像ハンドル・アイコンフレーム
	int m_Image_IconCursor;	// 画像ハンドル・アイコンカーソル
	int m_Image_Entry;		// 画像ハンドル・入場演出
	int m_Font_Name;		// フォントハンドル・名前
	int m_Font_Lore;		// フォントハンドル・説明
};
