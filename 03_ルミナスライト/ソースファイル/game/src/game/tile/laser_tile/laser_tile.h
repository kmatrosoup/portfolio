#pragma once

#include "../tile.h"
#include "../../direction_id/direction_id.h"

class CLaserTile : public ITile
{
public:
	CLaserTile(IGameObject* parent);
	~CLaserTile() override = default;

	void Initialize(DIRECTION_ID direction);
	void Update() override;
	void Draw() override;
	void Finalize() override;

	TILE_ID GetTileID() const override;
	bool IsMovable() const override;
	std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const override;
	void DrawLaserTrail(const aqua::CSprite& laser_sprite, const SLaserData& laser_data) const override;
	void DrawOutLine() override;

	std::list<SLaserData> GetGeneratedLaser() const;

private:
	DIRECTION_ID m_Direction;

	aqua::CSprite m_Sprite;
	aqua::CSprite m_OutLineSprite;
};
