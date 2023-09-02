#pragma once

#include "../tile.h"

class CField;

class CWallTile : public ITile
{
public:
	CWallTile(IGameObject* parent);
	~CWallTile() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	TILE_ID GetTileID() const override;
	bool IsMovable() const override;
	std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const override;
	void DrawLaserTrail(const aqua::CSprite& laser_sprite, const SLaserData& laser_data) const override;

private:
	CField* m_pField;

	aqua::CSprite m_Sprite[4];
};
