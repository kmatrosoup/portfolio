#include "laser_tile.h"

CLaserTile::CLaserTile(IGameObject* parent)
	: ITile(parent, "LaserTile")
{
}

void CLaserTile::Initialize(DIRECTION_ID direction)
{
	m_Sprite.Create("data\\tile_laser.png");
	m_Sprite.anchor.x = m_Sprite.GetTextureWidth() / 2.0f;
	m_Sprite.anchor.y = m_Sprite.GetTextureHeight() / 2.0f;
	m_OutLineSprite.Create("data\\tile_laser_outline.png");
	m_OutLineSprite.anchor.x = m_OutLineSprite.GetTextureWidth() / 2.0f;
	m_OutLineSprite.anchor.y = m_OutLineSprite.GetTextureHeight() / 2.0f;
	m_Direction = direction;

	switch (m_Direction)
	{
	case DIRECTION_ID::RIGHT:	m_Sprite.rotation = aqua::DegToRad(0.0f);	break;
	case DIRECTION_ID::DOWN:	m_Sprite.rotation = aqua::DegToRad(90.0f);	break;
	case DIRECTION_ID::LEFT:	m_Sprite.rotation = aqua::DegToRad(180.0f);	break;
	case DIRECTION_ID::UP:		m_Sprite.rotation = aqua::DegToRad(270.0f);	break;
	}
	m_OutLineSprite.rotation = m_Sprite.rotation;
}

void CLaserTile::Update()
{
	m_Sprite.scale.x = m_DispSize / (float)m_Sprite.GetTextureWidth();
	m_Sprite.scale.y = m_DispSize / (float)m_Sprite.GetTextureHeight();
	m_Sprite.position = m_Position - m_Sprite.anchor + aqua::CVector2(m_DispSize, m_DispSize) * 0.5f;

	m_OutLineSprite.scale = m_Sprite.scale;
	m_OutLineSprite.position = m_Sprite.position + m_Sprite.anchor - m_OutLineSprite.anchor;
}

void CLaserTile::Draw()
{
	m_Sprite.Draw();
}

void CLaserTile::Finalize()
{
	m_Sprite.Delete();
	m_OutLineSprite.Delete();
}

TILE_ID CLaserTile::GetTileID() const
{
	return TILE_ID::LASER;
}

bool CLaserTile::IsMovable() const
{
	return true;
}

std::list<SLaserData> CLaserTile::GetGeneratedLaser() const
{
	return { SLaserData(m_Direction, COLOR_ID::WHITE) };
}

std::list<SLaserData> CLaserTile::GetConvertedLaser(const SLaserData& laser) const
{
	return {};
}

void CLaserTile::DrawLaserTrail(const aqua::CSprite& laser_sprite, const SLaserData& laser_data) const
{
}

void CLaserTile::DrawOutLine()
{
	m_OutLineSprite.Draw();
}
