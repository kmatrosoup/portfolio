#pragma once

#include "aqua.h"
#include "tile_id.h"
#include "../laser_data/laser_data.h"

class ITile : public aqua::IGameObject
{
public:
	ITile(IGameObject* parent, const std::string& name);
	virtual ~ITile() override = default;

	virtual TILE_ID GetTileID() const = 0;
	virtual bool IsMovable() const = 0;
	virtual std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const = 0;
	virtual void DrawLaserTrail(const aqua::CSprite& laser_sprite, const SLaserData& laser_data) const = 0;
	virtual void DrawOutLine() {};

	void SetLocate(const aqua::CPoint& locate);
	const aqua::CPoint& GetLocate() const;
	void SetPosition(const aqua::CVector2& position);
	void SetDispSize(float size);

protected:
	aqua::CPoint m_Locate;
	aqua::CVector2 m_Position;
	float m_DispSize;
};
