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
	void Draw_Lit() override;
	void Finalize() override;

	TILE_ID GetTileID() const override;																		// タイルIDを取得
	bool IsMovable() const override;																		// 移動可能か判定
	std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const override;						// 入力光に対する出力光を取得
	void DrawLaserTrail(aqua::CAnimationSprite laser_sprite, const SLaserData& laser_data) const override;	// 同じ位置のレーザーを描画

	void IrradiateLaser(const SLaserData& laser);	// 
	bool IsLit() const { return m_IsLit; }			// 発光状態を判定
	void SetLitFlag(bool flag) { m_IsLit = flag; }	// 発光状態を設定

private:
	DIRECTION_ID m_Direction;	// 方向ID
	COLOR_ID m_Color;			// 色ID
	bool m_IsLit;				// 発光状態

	aqua::CAnimationSprite m_Sprite;	// タイル画像
	aqua::CAnimationSprite m_LitSprite;	// 発光用アニメーション画像
	aqua::CTexture m_LaserMaskTexture;	// マスクテクスチャ
	int m_LaserDestScreen;				// フィルター後のレーザースクリーン
};
