#include "crystal_tile.h"

CCrystalTile::CCrystalTile(IGameObject* parent)
	: ITile(parent, "CrystalTile")
{
}

void CCrystalTile::Initialize(COLOR_ID color)
{
	m_Sprite.Create("data\\tile_crystal.ass");
	m_OutLineSprite.Create("data\\tile_crystal_outline.png");
	m_LaserMaskTexture.Load("data\\tile_crystal_mask.png");
	m_LaserDestScreen = DxLib::MakeScreen(24, 24, TRUE);

	m_Color = color;

	m_Sprite.Change((int)m_Color);
}

void CCrystalTile::Update()
{
	m_Sprite.position = m_Position;
	m_Sprite.scale.x = m_DispSize / (float)m_Sprite.GetFrameWidth();
	m_Sprite.scale.y = m_DispSize / (float)m_Sprite.GetFrameHeight();

	m_OutLineSprite.scale = m_Sprite.scale;
	m_OutLineSprite.position = m_Sprite.position - m_OutLineSprite.scale;
}

void CCrystalTile::Draw()
{
	m_Sprite.color = 0xffffffff;
	m_Sprite.blend_mode = aqua::ALPHABLEND::ALPHA;
	m_Sprite.Draw();
}

void CCrystalTile::Draw_Lit()
{
	m_Sprite.color = 0xff606060;
	m_Sprite.blend_mode = aqua::ALPHABLEND::ADD;
	m_Sprite.Draw();
}

void CCrystalTile::Finalize()
{
	m_Sprite.Delete();
	m_OutLineSprite.Delete();
	m_LaserMaskTexture.Unload();
	DxLib::DeleteGraph(m_LaserDestScreen);
}

TILE_ID CCrystalTile::GetTileID() const
{
	return TILE_ID::CRYSTAL;
}

bool CCrystalTile::IsMovable() const
{
	return true;
}

std::list<SLaserData> CCrystalTile::GetConvertedLaser(const SLaserData& laser) const
{
	return { SLaserData(laser.direction, m_Color) };
}

void CCrystalTile::DrawLaserTrail(aqua::CAnimationSprite laser_sprite, const SLaserData& laser_data) const
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
			m_tile_size * (laser_sprite.GetCurrentFrameID() % laser_sprite.GetFrameCols()),
			m_tile_size * (laser_sprite.GetCurrentFrameID() / laser_sprite.GetFrameCols()),
			m_tile_size * (laser_sprite.GetCurrentFrameID() % laser_sprite.GetFrameCols()) + m_tile_size,
			m_tile_size * (laser_sprite.GetCurrentFrameID() / laser_sprite.GetFrameCols()) + m_tile_size,
			0,
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
			m_tile_size,
			m_tile_size,
			laser_sprite.anchor.x,
			laser_sprite.anchor.y,
			laser_sprite.scale.x,
			laser_sprite.scale.y,
			laser_sprite.rotation,
			m_LaserDestScreen, TRUE);
	}

	// 出力光
	{
		laser_sprite.Change((int)m_Color, false);

		// 画像のブレンド
		DxLib::GraphBlendRectBlt(
			laser_sprite.GetResourceHandle(),
			m_LaserMaskTexture.GetResourceHandle(),
			m_LaserDestScreen,
			m_tile_size * (laser_sprite.GetCurrentFrameID() % laser_sprite.GetFrameCols()),
			m_tile_size * (laser_sprite.GetCurrentFrameID() / laser_sprite.GetFrameCols()),
			m_tile_size * (laser_sprite.GetCurrentFrameID() % laser_sprite.GetFrameCols()) + m_tile_size,
			m_tile_size * (laser_sprite.GetCurrentFrameID() / laser_sprite.GetFrameCols()) + m_tile_size,
			m_tile_size,
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
			m_tile_size,
			m_tile_size,
			laser_sprite.anchor.x,
			laser_sprite.anchor.y,
			laser_sprite.scale.x,
			laser_sprite.scale.y,
			laser_sprite.rotation,
			m_LaserDestScreen, TRUE);
	}

	// 描画モードを元に戻す
	SetDrawBright(aqua::CColor::MAX_COLOR, aqua::CColor::MAX_COLOR, aqua::CColor::MAX_COLOR);
	SetDrawBlendMode((int)aqua::ALPHABLEND::ALPHA, aqua::CColor::MAX_COLOR);
}

void CCrystalTile::DrawOutLine()
{
	m_OutLineSprite.Draw();
}
