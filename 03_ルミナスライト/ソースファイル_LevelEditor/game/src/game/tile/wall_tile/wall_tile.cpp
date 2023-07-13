#include "wall_tile.h"
#include "../../field/field.h"

CWallTile::CWallTile(IGameObject* parent)
	: ITile(parent, "WallTile")
	, m_pField(nullptr)
{
}

void CWallTile::Initialize()
{
	m_pField = (CField*)aqua::FindGameObject("Field");

	for (int i = 0; i < 4; ++i)
		m_Sprite[i].Create("data\\tile_wall_part.png");
}

void CWallTile::Update()
{
	for (int i = 0; i < 4; ++i)
	{
		m_Sprite[i].position = m_Position + aqua::CVector2(m_DispSize, m_DispSize) / 2.0f;

		int dir_x = (i % 2) - !(i % 2);
		int dir_y = ((i / 2) % 2) - !((i / 2) % 2);
		TILE_ID connect_x = m_pField->GetTileID(m_Locate + aqua::CPoint(dir_x, 0));
		TILE_ID connect_y = m_pField->GetTileID(m_Locate + aqua::CPoint(0, dir_y));
		TILE_ID connect_xy = m_pField->GetTileID(m_Locate + aqua::CPoint(dir_x, dir_y));
		int r = (
			(connect_x == TILE_ID::WALL || connect_x == TILE_ID::GLASS || connect_x == TILE_ID::MAX) * 1 +
			(connect_y == TILE_ID::WALL || connect_y == TILE_ID::GLASS || connect_y == TILE_ID::MAX) * 2 +
			(connect_xy == TILE_ID::WALL || connect_xy == TILE_ID::GLASS || connect_xy == TILE_ID::MAX) * 4);
		m_Sprite[i].rect.left = m_Sprite[i].GetTextureWidth() / 8 * r;
		m_Sprite[i].rect.right = m_Sprite[i].rect.left + m_Sprite[i].GetTextureWidth() / 8;
		m_Sprite[i].rect.top = 0;
		m_Sprite[i].rect.bottom = m_Sprite[i].GetTextureHeight();

		m_Sprite[i].scale.x = m_DispSize / 2.0f / (float)m_Sprite[i].GetTextureWidth() * 8.0f * dir_x;
		m_Sprite[i].scale.y = m_DispSize / 2.0f / (float)m_Sprite[i].GetTextureHeight() * dir_y;
	}
}

void CWallTile::Draw()
{
	for (int i = 0; i < 4; ++i)
		m_Sprite[i].Draw();
}

void CWallTile::Finalize()
{
	for (int i = 0; i < 4; ++i)
		m_Sprite[i].Delete();
}

TILE_ID CWallTile::GetTileID() const
{
	return TILE_ID::WALL;
}

bool CWallTile::IsMovable() const
{
	return false;
}

std::list<SLaserData> CWallTile::GetConvertedLaser(const SLaserData& laser) const
{
	return {};
}

void CWallTile::DrawLaserTrail(const aqua::CSprite& laser_sprite, const SLaserData& laser_data) const
{
}
