#include "glass_tile.h"
#include "../../field/field.h"

CGlassTile::CGlassTile(IGameObject* parent)
	: ITile(parent, "GlassTile")
	, m_pField(nullptr)
{
}

void CGlassTile::Initialize()
{
	m_Sprite.Create("data\\tile_glass.png");
	m_pField = (CField*)aqua::FindGameObject("Field");

	for (int i = 0; i < 4; ++i)
		m_PartSprite[i].Create("data\\tile_glass_part.png");
}

void CGlassTile::Update()
{
	m_Sprite.position = m_Position;
	m_Sprite.scale.x = m_DispSize / (float)m_Sprite.GetTextureWidth();
	m_Sprite.scale.y = m_DispSize / (float)m_Sprite.GetTextureHeight();

	for (int i = 0; i < 4; ++i)
	{
		m_PartSprite[i].position = m_Position + aqua::CVector2(m_DispSize, m_DispSize) / 2.0f;

		int dir_x = (i % 2) - !(i % 2);
		int dir_y = ((i / 2) % 2) - !((i / 2) % 2);
		TILE_ID connect_x = m_pField->GetTileID(m_Locate + aqua::CPoint(dir_x, 0));
		TILE_ID connect_y = m_pField->GetTileID(m_Locate + aqua::CPoint(0, dir_y));
		TILE_ID connect_xy = m_pField->GetTileID(m_Locate + aqua::CPoint(dir_x, dir_y));
		int r = (
			(connect_x == TILE_ID::WALL || connect_x == TILE_ID::GLASS || connect_x == TILE_ID::MAX) * 1 +
			(connect_y == TILE_ID::WALL || connect_y == TILE_ID::GLASS || connect_y == TILE_ID::MAX) * 2 +
			(connect_xy == TILE_ID::WALL || connect_xy == TILE_ID::GLASS || connect_xy == TILE_ID::MAX) * 4);
		m_PartSprite[i].rect.left = m_PartSprite[i].GetTextureWidth() / 8 * r;
		m_PartSprite[i].rect.right = m_PartSprite[i].rect.left + m_PartSprite[i].GetTextureWidth() / 8;
		m_PartSprite[i].rect.top = 0;
		m_PartSprite[i].rect.bottom = m_PartSprite[i].GetTextureHeight();

		m_PartSprite[i].scale.x = m_DispSize / 2.0f / (float)m_PartSprite[i].GetTextureWidth() * 8.0f * dir_x;
		m_PartSprite[i].scale.y = m_DispSize / 2.0f / (float)m_PartSprite[i].GetTextureHeight() * dir_y;
	}
}

void CGlassTile::Draw()
{
	m_Sprite.Draw();
	for (int i = 0; i < 4; ++i)
	{
		m_PartSprite[i].color = 0xffffffff;
		m_PartSprite[i].Draw();
	}
}

void CGlassTile::Draw_Lit()
{
	for (int i = 0; i < 4; ++i)
	{
		m_PartSprite[i].color = 0xff000000;
		m_PartSprite[i].Draw();
	}
}

void CGlassTile::Finalize()
{
	m_Sprite.Delete();
	for (int i = 0; i < 4; ++i)
		m_PartSprite[i].Delete();
}

TILE_ID CGlassTile::GetTileID() const
{
	return TILE_ID::GLASS;
}

bool CGlassTile::IsMovable() const
{
	return false;
}

std::list<SLaserData> CGlassTile::GetConvertedLaser(const SLaserData& laser) const
{
	return { laser };
}

void CGlassTile::DrawLaserTrail(aqua::CAnimationSprite laser_sprite, const SLaserData& laser_data) const
{
	// ï`âÊÉÇÅ[ÉhÇÃê›íË
	DxLib::SetDrawBlendMode((int)laser_sprite.blend_mode, laser_sprite.color.alpha);
	DxLib::SetDrawBright(laser_sprite.color.red, laser_sprite.color.green, laser_sprite.color.blue);

	// ï`âÊ
	DxLib::DrawRectRotaGraph3F(
		laser_sprite.position.x + laser_sprite.anchor.x,
		laser_sprite.position.y + laser_sprite.anchor.y,
		m_tile_size * (laser_sprite.GetCurrentFrameID() % laser_sprite.GetFrameCols()),
		m_tile_size * (laser_sprite.GetCurrentFrameID() / laser_sprite.GetFrameCols()),
		m_tile_size,
		m_tile_size,
		laser_sprite.anchor.x,
		laser_sprite.anchor.y,
		laser_sprite.scale.x,
		laser_sprite.scale.y,
		laser_sprite.rotation,
		laser_sprite.GetResourceHandle(), TRUE);

	// ï`âÊÉÇÅ[ÉhÇå≥Ç…ñﬂÇ∑
	SetDrawBright(aqua::CColor::MAX_COLOR, aqua::CColor::MAX_COLOR, aqua::CColor::MAX_COLOR);
	SetDrawBlendMode((int)aqua::ALPHABLEND::ALPHA, aqua::CColor::MAX_COLOR);
}
