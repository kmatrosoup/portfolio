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
	void Draw_Lit() override;
	void Finalize() override;

	TILE_ID GetTileID() const override;																		// タイルIDを取得
	bool IsMovable() const override;																		// 移動可能か判定
	std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const override;						// 入力光に対する出力光を取得
	void DrawLaserTrail(aqua::CAnimationSprite laser_sprite, const SLaserData& laser_data) const override;	// 同じ位置のレーザーを描画
	void DrawOutLine() override;																			// アウトラインを描画

private:
	COLOR_ID m_Color;	// 色ID

	aqua::CAnimationSprite m_Sprite;	// タイル画像
	aqua::CSprite m_OutLineSprite;		// アウトライン画像
	aqua::CTexture m_LaserMaskTexture;	// マスクテクスチャ
	int m_LaserDestScreen;				// フィルター後のレーザースクリーン
};
