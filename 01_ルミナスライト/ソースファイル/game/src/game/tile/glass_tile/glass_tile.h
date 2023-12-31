#pragma once

#include "../tile.h"

class CField;

class CGlassTile : public ITile
{
public:
	CGlassTile(IGameObject* parent);
	~CGlassTile() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Draw_Lit() override;
	void Finalize() override;

	TILE_ID GetTileID() const override;																		// タイルIDを取得
	bool IsMovable() const override;																		// 移動可能か判定
	std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const override;						// 入力光に対する出力光を取得
	void DrawLaserTrail(aqua::CAnimationSprite laser_sprite, const SLaserData& laser_data) const override;	// 同じ位置のレーザーを描画

private:
	CField* m_pField;	// フィールドオブジェクト

	aqua::CSprite m_Sprite;			// タイル画像
	aqua::CSprite m_PartSprite[4];	// 枠の分割画像
};
