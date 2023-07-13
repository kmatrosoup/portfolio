#pragma once

#include "../tile.h"
#include "../../direction_id/direction_id.h"

class CWayTile : public ITile
{
public:
	CWayTile(IGameObject* parent);
	~CWayTile() override = default;

	void Initialize(DIRECTION_ID direction);
	void Update() override;
	void Draw() override;
	void Finalize() override;

	TILE_ID GetTileID() const override;
	bool IsMovable() const override;
	std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const override;
	void DrawLaserTrail(const aqua::CSprite& laser_sprite, const SLaserData& laser_data) const override;
	void DrawOutLine() override;

private:
	DIRECTION_ID m_Direction;

	aqua::CSprite m_Sprite;
	aqua::CSprite m_OutLineSprite;
	aqua::CTexture m_LaserMaskTexture;
	int m_LaserDestScreen;
};
