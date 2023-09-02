#pragma once

#include "aqua.h"
#include "../tile/tile_id.h"
#include "../laser_data/laser_data.h"

class ITile;
class CSoundManager;
class CGlowScreen;

class CField :
	public aqua::IGameObject
{
public:
	CField(IGameObject* parent);
	~CField() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	TILE_ID GetTileID(const aqua::CPoint& locate) const;
	float GetTileSize() const;

private:
	void ResizeField(int field_width, int field_height);
	void DeleteTile(const aqua::CPoint& locate);
	void ReplaceTile(const aqua::CPoint& locate);
	void LoadMapData(const std::string& file_name);
	void SaveMapData(const std::string& file_name) const;

private:
	static const float m_laser_trail_time;
	static const int m_laser_trail_frame_num;
	static const std::map<std::string, DIRECTION_ID> m_direction_table_to_id;
	static const std::map<DIRECTION_ID, std::string> m_direction_table_to_name;
	static const std::map<std::string, COLOR_ID> m_color_table_to_id;
	static const std::map<COLOR_ID, std::string> m_color_table_to_name;
	static const float m_edge_space;
	static const int m_tile_menu_num;
	static const std::vector<std::pair<TILE_ID, COLOR_ID>> m_tile_menu;

	CSoundManager* m_pSoundManager;
	CGlowScreen* m_pGlowScreen;
	int m_FieldWidth;
	int m_FieldHeight;
	std::vector<std::vector<ITile*>> m_pTileLocate;
	float m_TileSize;
	std::list<std::pair<aqua::CPoint, SLaserData>> m_LaserChain;
	aqua::CPoint m_CursorLocate;
	aqua::CPoint m_CursorLocate_prev;
	aqua::CTimer m_LaserTrailTimer;
	int m_SelectedTileMenu;
	DIRECTION_ID m_SelectedDirection;

	aqua::CSprite m_LaserTrailSprite;
	aqua::CSprite m_OutAreaTileSprite;
	aqua::CSurface m_GlowMaskSurface;
	aqua::CSprite m_GlowMaskSprite;
	aqua::CBoxPrimitive m_CursorHorizontalBox;
	aqua::CBoxPrimitive m_CursorVerticalBox;
	aqua::CSprite m_TileMenuSprite;
};
