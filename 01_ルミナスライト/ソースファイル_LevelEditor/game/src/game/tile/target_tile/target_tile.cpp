#include "target_tile.h"

CTargetTile::CTargetTile(IGameObject* parent)
	: ITile(parent, "TargetTile")
	, m_IsLit(false)
{
}

void CTargetTile::Initialize(DIRECTION_ID direction, COLOR_ID color)
{
	// リソースの読み込み
	m_Sprite.Create("data\\tile_target.ass");
	m_Direction = direction;
	m_Color = color;

	switch (m_Direction)
	{
	case DIRECTION_ID::RIGHT:	m_Sprite.rotation = aqua::DegToRad(0.0f);	break;
	case DIRECTION_ID::DOWN:	m_Sprite.rotation = aqua::DegToRad(90.0f);	break;
	case DIRECTION_ID::LEFT:	m_Sprite.rotation = aqua::DegToRad(180.0f);	break;
	case DIRECTION_ID::UP:		m_Sprite.rotation = aqua::DegToRad(270.0f);	break;
	}

	m_Sprite.Change((int)m_Color);

	m_LitSprite.Create("data\\tile_target_lit.ass");
	m_LitSprite.Change((int)m_Color);
	m_LitSprite.rotation = m_Sprite.rotation;

	m_LaserMaskTexture.Load("data\\tile_target_mask.png");
	m_LaserDestScreen = DxLib::MakeScreen(24, 24, TRUE);
}

void CTargetTile::Update()
{
	switch (m_Direction)
	{
	case DIRECTION_ID::RIGHT:
		m_Sprite.position = m_Position;
		break;
	case DIRECTION_ID::DOWN:
		m_Sprite.position = m_Position;
		m_Sprite.position.x += m_DispSize;
		break;
	case DIRECTION_ID::LEFT:
		m_Sprite.position = m_Position;
		m_Sprite.position.x += m_DispSize;
		m_Sprite.position.y += m_DispSize;
		break;
	case DIRECTION_ID::UP:
		m_Sprite.position = m_Position;
		m_Sprite.position.y += m_DispSize;
		break;
	}
	m_Sprite.scale.x = m_DispSize / (float)m_Sprite.GetFrameWidth();
	m_Sprite.scale.y = m_DispSize / (float)m_Sprite.GetFrameHeight();
	m_LitSprite.position = m_Sprite.position;
	m_LitSprite.scale = m_Sprite.scale;

	// 照射時の発光処理
	m_LitSprite.visible = m_IsLit;
	m_LitSprite.Update();
}

void CTargetTile::Draw()
{
	m_Sprite.color = 0xffffffff;
	m_Sprite.Draw();
	m_LitSprite.Draw();
}

void CTargetTile::Draw_Lit()
{
	m_Sprite.color = 0xff000000;
	m_Sprite.Draw();
	m_LitSprite.Draw();
}

void CTargetTile::Finalize()
{
	m_Sprite.Delete();
	m_LitSprite.Delete();
	m_LaserMaskTexture.Unload();
	DxLib::DeleteGraph(m_LaserDestScreen);
}

TILE_ID CTargetTile::GetTileID() const
{
	return TILE_ID::TARGET;
}

bool CTargetTile::IsMovable() const
{
	return false;
}

std::list<SLaserData> CTargetTile::GetConvertedLaser(const SLaserData& laser) const
{
	return {};
}

void CTargetTile::DrawLaserTrail(aqua::CAnimationSprite laser_sprite, const SLaserData& laser_data) const
{
	// 向きが違う場合は描画しない
	if (laser_data.direction != m_Direction)
		return;

	// 描画モードの設定
	DxLib::SetDrawBlendMode((int)laser_sprite.blend_mode, laser_sprite.color.alpha);
	DxLib::SetDrawBright(laser_sprite.color.red, laser_sprite.color.green, laser_sprite.color.blue);

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

	// 描画モードを元に戻す
	SetDrawBright(aqua::CColor::MAX_COLOR, aqua::CColor::MAX_COLOR, aqua::CColor::MAX_COLOR);
	SetDrawBlendMode((int)aqua::ALPHABLEND::ALPHA, aqua::CColor::MAX_COLOR);
}

void CTargetTile::IrradiateLaser(const SLaserData& laser)
{
	if (m_Direction == laser.direction && m_Color == laser.color)
		m_IsLit = true;
}
