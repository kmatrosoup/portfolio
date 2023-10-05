#pragma once

#include "aqua.h"
#include "../tile/tile_id.h"
#include "../laser_data/laser_data.h"

class ITile;
class CCommonData;
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

	TILE_ID GetTileID(const aqua::CPoint& locate) const;	// タイルIDを取得
	bool IsSuccess() const;									// 成功判定
	float GetTileSize() const;								// タイルの大きさを取得
	void SetTargetEdgeSpace(float space);					// 画面端余白の大きさを設定
	void SetControlFlag(bool flag);							// 操作可能フラグを設定

private:
	void LoadMapData(const std::string& file_name);	// レベル情報を読み込む

	void Update_01_TileDrawSettings();		// タイルの描画設定を更新
	void Update_02_MovementTile();			// タイルの移動処理
	void Update_03_Tiles();					// タイルの更新
	void Update_04_CalcLaser();				// レーザーの軌道計算
	void Update_05_CheckSuccess();			// 成功判定
	void Update_06_CursorDrawSettings();	// カーソルの描画設定を更新
	void Update_07_Animations();			// 各種アニメーションの更新

private:
	static const float m_first_edge_space;								// 開始時の画面端余白の大きさ
	static const std::map<std::string, DIRECTION_ID> m_direction_table;	// 文字列に対応する方向ID
	static const std::map<std::string, COLOR_ID> m_color_table;			// 文字列に対応する色ID

	CCommonData* m_pCommonData;		// 共有データオブジェクト
	CSoundManager* m_pSoundManager;	// サウンド管理オブジェクト

	bool m_IsSuccess;												// 成功判定
	bool m_CanControl;												// 操作可能判定
	int m_TileNumX;													// 横タイル数
	int m_TileNumY;													// 縦タイル数
	float m_DispTileSize;											// 表示するタイルの大きさ
	float m_EdgeSpace;												// 画面端余白の大きさ
	float m_TargetEdgeSpace;										// 最終的な画面端余白の大きさ
	aqua::CPoint m_CursorLocate;									// カーソル位置
	ITile* m_pCursoredTile;											// マウスオーバー中のタイル
	std::vector<std::vector<ITile*>> m_pTiles;						// 各タイルのデータ
	std::list<std::pair<aqua::CPoint, SLaserData>> m_LaserChain;	// レーザーの軌跡データ

	aqua::CAnimationSprite m_LaserTrailSprite;	// レーザーの軌跡画像
	aqua::CSprite m_OutAreaTileSprite;			// 範囲外のタイル画像
	aqua::CBoxPrimitive m_CursorHorizontalBox;	// マウスオーバー位置の横強調線
	aqua::CBoxPrimitive m_CursorVerticalBox;	// マウスオーバー位置の縦強調線
};
