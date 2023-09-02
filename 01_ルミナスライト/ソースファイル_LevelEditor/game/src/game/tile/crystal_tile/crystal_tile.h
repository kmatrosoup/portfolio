#pragma once

#include "../tile.h"
#include "../../color_id/color_id.h"

class CCrystalTile : public ITile
{
public:
	CCrystalTile(IGameObject* parent);
	~CCrystalTile() override = default;

	void Initialize(COLOR_ID color);
	void Update() override;
	void Draw() override;
	void Finalize() override;

	TILE_ID GetTileID() const override;
	COLOR_ID GetColorID() const;
	bool IsMovable() const override;
	std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const override;
	void DrawLaserTrail(const aqua::CSprite& laser_sprite, const SLaserData& laser_data) const override;
	void DrawOutLine() override;

private:
	COLOR_ID m_Color;

	aqua::CSprite m_Sprite;
	aqua::CSprite m_OutLineSprite;
	aqua::CTexture m_LaserMaskTexture;
	int m_LaserDestScreen;
};
