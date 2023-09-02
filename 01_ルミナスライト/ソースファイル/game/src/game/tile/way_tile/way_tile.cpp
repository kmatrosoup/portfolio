#include "way_tile.h"

CWayTile::CWayTile(IGameObject* parent)
	: ITile(parent, "WayTile")
{
}

void CWayTile::Initialize(DIRECTION_ID direction)
{
	m_Sprite.Create("data\\tile_way.png");
	m_Sprite.anchor.x = m_Sprite.GetTextureWidth() / 2.0f;
	m_Sprite.anchor.y = m_Sprite.GetTextureHeight() / 2.0f;
	m_OutLineSprite.Create("data\\tile_way_outline.png");
	m_OutLineSprite.anchor.x = m_OutLineSprite.GetTextureWidth() / 2.0f;
	m_OutLineSprite.anchor.y = m_OutLineSprite.GetTextureHeight() / 2.0f;
	m_LaserMaskTexture.Load("data\\tile_way_mask.png");
	m_LaserDestScreen = DxLib::MakeScreen(24, 24, TRUE);

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

void CWayTile::Update()
{
	m_Sprite.scale.x = m_DispSize / (float)m_Sprite.GetTextureWidth();
	m_Sprite.scale.y = m_DispSize / (float)m_Sprite.GetTextureHeight();
	m_Sprite.position = m_Position - m_Sprite.anchor + aqua::CVector2(m_DispSize, m_DispSize) * 0.5f;

	m_OutLineSprite.scale = m_Sprite.scale;
	m_OutLineSprite.position = m_Sprite.position + m_Sprite.anchor - m_OutLineSprite.anchor;
}

void CWayTile::Draw()
{
	m_Sprite.Draw();
}

void CWayTile::Finalize()
{
	m_Sprite.Delete();
	m_OutLineSprite.Delete();
	m_LaserMaskTexture.Unload();
	DxLib::DeleteGraph(m_LaserDestScreen);
}

TILE_ID CWayTile::GetTileID() const
{
	return TILE_ID::WAY;
}

bool CWayTile::IsMovable() const
{
	return true;
}

std::list<SLaserData> CWayTile::GetConvertedLaser(const SLaserData& laser) const
{
	if (laser.direction != m_Direction)
		return {};

	return {
		SLaserData((DIRECTION_ID)(((int)laser.direction + 1) % 4), laser.color),
		SLaserData((DIRECTION_ID)(((int)laser.direction + 3) % 4), laser.color) };
}

void CWayTile::DrawLaserTrail(const aqua::CSprite& laser_sprite, const SLaserData& laser_data) const
{
	// å¸Ç´Ç™à·Ç§èÍçáÇÕï`âÊÇµÇ»Ç¢
	if (laser_data.direction != m_Direction)
		return;

	// ï`âÊÉÇÅ[ÉhÇÃê›íË
	DxLib::SetDrawBlendMode((int)laser_sprite.blend_mode, laser_sprite.color.alpha);
	DxLib::SetDrawBright(laser_sprite.color.red, laser_sprite.color.green, laser_sprite.color.blue);

	// ì¸éÀåı
	{
		// âÊëúÇÃÉuÉåÉìÉh
		DxLib::GraphBlendRectBlt(
			laser_sprite.GetResourceHandle(),
			m_LaserMaskTexture.GetResourceHandle(),
			m_LaserDestScreen,
			laser_sprite.rect.left,
			laser_sprite.rect.top,
			laser_sprite.rect.right,
			laser_sprite.rect.bottom,
			0,
			0,
			0,
			0,
			255, DX_GRAPH_BLEND_MULTIPLE_A_ONLY);

		// ï`âÊ
		DxLib::DrawRectRotaGraph3F(
			laser_sprite.position.x + laser_sprite.anchor.x,
			laser_sprite.position.y + laser_sprite.anchor.y,
			0,
			0,
			24,
			24,
			laser_sprite.anchor.x,
			laser_sprite.anchor.y,
			laser_sprite.scale.x,
			laser_sprite.scale.y,
			laser_sprite.rotation,
			m_LaserDestScreen, TRUE);
	}

	// èoóÕåıÇP
	{
		// âÊëúÇÃÉuÉåÉìÉh
		DxLib::GraphBlendRectBlt(
			laser_sprite.GetResourceHandle(),
			m_LaserMaskTexture.GetResourceHandle(),
			m_LaserDestScreen,
			laser_sprite.rect.left,
			laser_sprite.rect.top,
			laser_sprite.rect.right,
			laser_sprite.rect.bottom,
			24,
			0,
			0,
			0,
			255, DX_GRAPH_BLEND_MULTIPLE_A_ONLY);

		// ï`âÊ
		DxLib::DrawRectRotaGraph3F(
			laser_sprite.position.x + laser_sprite.anchor.x,
			laser_sprite.position.y + laser_sprite.anchor.y,
			0,
			0,
			24,
			24,
			laser_sprite.anchor.x,
			laser_sprite.anchor.y,
			laser_sprite.scale.x,
			laser_sprite.scale.y,
			laser_sprite.rotation - aqua::DegToRad(90),
			m_LaserDestScreen, TRUE);
	}

	// èoóÕåıÇQ
	{
		// âÊëúÇÃÉuÉåÉìÉh
		DxLib::GraphBlendRectBlt(
			laser_sprite.GetResourceHandle(),
			m_LaserMaskTexture.GetResourceHandle(),
			m_LaserDestScreen,
			laser_sprite.rect.left,
			laser_sprite.rect.top,
			laser_sprite.rect.right,
			laser_sprite.rect.bottom,
			24,
			0,
			0,
			0,
			255, DX_GRAPH_BLEND_MULTIPLE_A_ONLY);

		// ï`âÊ
		DxLib::DrawRectRotaGraph3F(
			laser_sprite.position.x + laser_sprite.anchor.x,
			laser_sprite.position.y + laser_sprite.anchor.y,
			0,
			0,
			24,
			24,
			laser_sprite.anchor.x,
			laser_sprite.anchor.y,
			laser_sprite.scale.x,
			laser_sprite.scale.y,
			laser_sprite.rotation + aqua::DegToRad(90),
			m_LaserDestScreen, TRUE);
	}

	// ï`âÊÉÇÅ[ÉhÇå≥Ç…ñﬂÇ∑
	SetDrawBright(aqua::CColor::MAX_COLOR, aqua::CColor::MAX_COLOR, aqua::CColor::MAX_COLOR);
	SetDrawBlendMode((int)aqua::ALPHABLEND::ALPHA, aqua::CColor::MAX_COLOR);
}

void CWayTile::DrawOutLine()
{
	m_OutLineSprite.Draw();
}
