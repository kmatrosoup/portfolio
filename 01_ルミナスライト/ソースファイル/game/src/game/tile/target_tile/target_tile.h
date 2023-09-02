#pragma once

#include "../tile.h"
#include "../../direction_id/direction_id.h"
#include "../../color_id/color_id.h"

class CTargetTile : public ITile
{
public:
	CTargetTile(IGameObject* parent);
	~CTargetTile() override = default;

	void Initialize(DIRECTION_ID direction, COLOR_ID color);
	void Update() override;
	void Draw() override;
	void DrawLit();
	void Finalize() override;

	TILE_ID GetTileID() const override;
	bool IsMovable() const override;
	std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const override;
	void DrawLaserTrail(const aqua::CSprite& laser_sprite, const SLaserData& laser_data) const override;

	void SetLitFlag(bool flag);
	bool GetLitFlag() const;
	void IrradiateLaser(const SLaserData& laser);

private:
	static const float m_lit_time;
	static const int m_lit_frame_num;

	DIRECTION_ID m_Direction;
	COLOR_ID m_Color;
	bool m_LitFlag;
	aqua::CTimer m_LitTimer;

	aqua::CSprite m_Sprite;
	aqua::CSprite m_LitSprite;
	aqua::CTexture m_LaserMaskTexture;
	int m_LaserDestScreen;
};
