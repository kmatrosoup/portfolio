#pragma once

#include "aqua.h"
#include "../tile/tile_id.h"
#include "../laser_data/laser_data.h"
#include <filesystem>

class ITile;
class CSoundManager;

class CField :
	public aqua::IGameObject
{
public:
	CField(IGameObject* parent);
	~CField() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Draw_Lit();
	void Finalize() override;

	TILE_ID GetTileID(const aqua::CPoint& locate) const;
	float GetTileSize() const;

private:
	void ResizeField(int field_width, int field_height);
	void DeleteTile(const aqua::CPoint& locate);
	void ReplaceTile(const aqua::CPoint& locate);
	void LoadMapData(const std::string& file_name);
	void SaveMapData(const std::string& file_name) const;

	void Update_01_LoadDragFileMapData();	// ドラッグアンドドロップしたファイルを読み込む
	void Update_02_ChangeTileMenu();		// 設置するタイルの変更
	void Update_03_ChangeTileNum();			// タイル数の変更
	void Update_04_TileDrawSettings();		// タイルの描画設定を更新
	void Update_05_PlaceAndEraseTile();		// タイルの設置/削除処理
	void Update_06_SaveMapData();			// タイルデータの保存
	void Update_07_Tiles();					// タイルの更新
	void Update_08_CalcLaser();				// レーザーの軌道計算
	void Update_09_CursorDrawSettings();	// カーソルの描画設定を更新
	void Update_10_Animations();			// 各種アニメーションの更新

private:
	static const std::map<std::string, DIRECTION_ID> m_direction_table_to_id;
	static const std::map<DIRECTION_ID, std::string> m_direction_table_to_name;
	static const std::map<std::string, COLOR_ID> m_color_table_to_id;
	static const std::map<COLOR_ID, std::string> m_color_table_to_name;
	static const float m_edge_space;
	static const int m_tile_menu_num;
	static const std::vector<std::pair<TILE_ID, COLOR_ID>> m_tile_menu;

	CSoundManager* m_pSoundManager;

	int m_TileNumX;													// 横タイル数
	int m_TileNumY;													// 縦タイル数
	int m_SelectedTileMenu;											// 選択中の設置するタイル
	float m_DispTileSize;											// 表示するタイルの大きさ
	DIRECTION_ID m_SelectedDirection;								// 設置するタイルの方向
	aqua::CPoint m_CursorLocate;									// カーソル位置
	aqua::CPoint m_CursorLocate_prev;								// １フレーム前のカーソル位置
	std::filesystem::path m_LoadFilePath;							// 読み込んだファイルのパス
	std::vector<std::vector<ITile*>> m_pTiles;						// 各タイルのデータ
	std::list<std::pair<aqua::CPoint, SLaserData>> m_LaserChain;	// レーザーの軌跡データ

	aqua::CAnimationSprite m_LaserTrailSprite;	// レーザーの軌跡画像
	aqua::CSprite m_OutAreaTileSprite;			// 範囲外のタイル画像
	aqua::CBoxPrimitive m_CursorHorizontalBox;	// マウスオーバー位置の横強調線
	aqua::CBoxPrimitive m_CursorVerticalBox;	// マウスオーバー位置の縦強調線
	aqua::CSprite m_TileMenuSprite;
};
