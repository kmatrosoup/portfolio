#pragma once

#include "aqua.h"
#include "../tile/tile_id.h"
#include "../laser_data/laser_data.h"

class ITile;
class CCommonData;
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
	bool CheckSuccess() const;
	float GetTileSize() const;
	void SetTargetEdgeSpace(float space);
	void SetControlFlag(bool flag);

private:
	void LoadMapData(const std::string& file_name);

private:
	static const float m_first_edge_space;
	static const float m_laser_trail_time;
	static const int m_laser_trail_frame_num;
	static const std::map<std::string, DIRECTION_ID> m_direction_table;
	static const std::map<std::string, COLOR_ID> m_color_table;

	CCommonData* m_pCommonData;
	CSoundManager* m_pSoundManager;
	CGlowScreen* m_pGlowScreen;
	int m_FieldWidth;
	int m_FieldHeight;
	std::vector<std::vector<ITile*>> m_pTileLocate;
	float m_TileSize;
	float m_EdgeSpace;
	float m_TargetEdgeSpace;
	std::list<std::pair<aqua::CPoint, SLaserData>> m_LaserChain;
	aqua::CPoint m_CursorLocate;
	ITile* m_pCursorTile;
	aqua::CTimer m_LaserTrailTimer;
	bool m_SuccessFlag;
	bool m_CanControlFlag;

	aqua::CSprite m_LaserTrailSprite;
	aqua::CSprite m_OutAreaTileSprite;
	aqua::CSurface m_GlowMaskSurface;
	aqua::CSprite m_GlowMaskSprite;
	aqua::CBoxPrimitive m_CursorHorizontalBox;
	aqua::CBoxPrimitive m_CursorVerticalBox;
};
