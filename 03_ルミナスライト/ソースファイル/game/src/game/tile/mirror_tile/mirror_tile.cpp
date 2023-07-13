#include "mirror_tile.h"

CMirrorTile::CMirrorTile(IGameObject* parent)
	: ITile(parent, "MirrorTile")
{
}

void CMirrorTile::Initialize(DIRECTION_ID direction)
{
	m_Sprite.Create("data\\tile_mirror.png");
	m_Sprite.anchor.x = m_Sprite.GetTextureWidth() / 2.0f;
	m_Sprite.anchor.y = m_Sprite.GetTextureHeight() / 2.0f;
	m_OutLineSprite.Create("data\\tile_mirror_outline.png");
	m_OutLineSprite.anchor.x = m_OutLineSprite.GetTextureWidth() / 2.0f;
	m_OutLineSprite.anchor.y = m_OutLineSprite.GetTextureHeight() / 2.0f;
	m_LaserMaskTexture.Load("data\\tile_mirror_mask.png");
	m_LaserDestScreen = DxLib::MakeScreen(24, 24, TRUE);

	m_Direction = direction;
	switch (m_Direction)
	{
	case DIRECTION_ID::RIGHT:	m_Sprite.rotation = aqua::DegToRad(0.0f);	break;
	case DIRECTION_ID::LEFT:	m_Sprite.rotation = aqua::DegToRad(90.0f);	break;
	}
	m_OutLineSprite.rotation = m_Sprite.rotation;
}

void CMirrorTile::Update()
{
	m_Sprite.scale.x = m_DispSize / (float)m_Sprite.GetTextureWidth();
	m_Sprite.scale.y = m_DispSize / (float)m_Sprite.GetTextureHeight();
	m_Sprite.position = m_Position - m_Sprite.anchor + aqua::CVector2(m_DispSize, m_DispSize) * 0.5f;

	m_OutLineSprite.scale = m_Sprite.scale;
	m_OutLineSprite.position = m_Sprite.position + m_Sprite.anchor - m_OutLineSprite.anchor;
}

void CMirrorTile::Draw()
{
	m_Sprite.Draw();
}

void CMirrorTile::Finalize()
{
	m_Sprite.Delete();
	m_OutLineSprite.Delete();
	m_LaserMaskTexture.Unload();
	DxLib::DeleteGraph(m_LaserDestScreen);
}

TILE_ID CMirrorTile::GetTileID() const
{
	return TILE_ID::MIRROR;
}

bool CMirrorTile::IsMovable() const
{
	return true;
}

std::list<SLaserData> CMirrorTile::GetConvertedLaser(const SLaserData& laser) const
{
	switch (laser.direction)
	{
	case DIRECTION_ID::RIGHT:
		if (m_Direction == DIRECTION_ID::RIGHT)	return { SLaserData(DIRECTION_ID::UP,		laser.color) };
		else									return { SLaserData(DIRECTION_ID::DOWN,		laser.color) };
		break;
	case DIRECTION_ID::DOWN:
		if (m_Direction == DIRECTION_ID::RIGHT)	return { SLaserData(DIRECTION_ID::LEFT,		laser.color) };
		else									return { SLaserData(DIRECTION_ID::RIGHT,	laser.color) };
		break;
	case DIRECTION_ID::LEFT:
		if (m_Direction == DIRECTION_ID::RIGHT)	return { SLaserData(DIRECTION_ID::DOWN,		laser.color) };
		else									return { SLaserData(DIRECTION_ID::UP,		laser.color) };
		break;
	case DIRECTION_ID::UP:
		if (m_Direction == DIRECTION_ID::RIGHT)	return { SLaserData(DIRECTION_ID::RIGHT,	laser.color) };
		else									return { SLaserData(DIRECTION_ID::LEFT,		laser.color) };
		break;
	}

	return {};
}

void CMirrorTile::DrawLaserTrail(const aqua::CSprite& laser_sprite, const SLaserData& laser_data) const
{
	// 描画モードの設定
	DxLib::SetDrawBlendMode((int)laser_sprite.blend_mode, laser_sprite.color.alpha);
	DxLib::SetDrawBright(laser_sprite.color.red, laser_sprite.color.green, laser_sprite.color.blue);

	// 入射光
	{
		// 画像のブレンド
		DxLib::GraphBlendRectBlt(
			laser_sprite.GetResourceHandle(),
			m_LaserMaskTexture.GetResourceHandle(),
			m_LaserDestScreen,
			laser_sprite.rect.left,
			laser_sprite.rect.top,
			laser_sprite.rect.right,
			laser_sprite.rect.bottom,
			24 * ((int)laser_data.direction % 2 == (int)m_Direction / 2),
			0,
			0,
			0,
			255, DX_GRAPH_BLEND_MULTIPLE_A_ONLY);

		// 描画
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

	// 反射光
	{
		// 画像のブレンド
		DxLib::GraphBlendRectBlt(
			laser_sprite.GetResourceHandle(),
			m_LaserMaskTexture.GetResourceHandle(),
			m_LaserDestScreen,
			laser_sprite.rect.left,
			laser_sprite.rect.top,
			laser_sprite.rect.right,
			laser_sprite.rect.bottom,
			48 + 24 * ((int)laser_data.direction % 2 == (int)m_Direction / 2),
			0,
			0,
			0,
			255, DX_GRAPH_BLEND_MULTIPLE_A_ONLY);

		// 描画
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
			laser_sprite.rotation + aqua::DegToRad(90.0f - 180.0f * ((int)laser_data.direction % 2 == (int)m_Direction / 2)),
			m_LaserDestScreen, TRUE);
	}

	// 描画モードを元に戻す
	SetDrawBright(aqua::CColor::MAX_COLOR, aqua::CColor::MAX_COLOR, aqua::CColor::MAX_COLOR);
	SetDrawBlendMode((int)aqua::ALPHABLEND::ALPHA, aqua::CColor::MAX_COLOR);
}

void CMirrorTile::DrawOutLine()
{
	m_OutLineSprite.Draw();
}
