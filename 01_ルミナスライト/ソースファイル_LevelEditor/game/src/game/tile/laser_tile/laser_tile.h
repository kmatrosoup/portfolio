#pragma once

#include "../tile.h"
#include "../../direction_id/direction_id.h"

class CLaserTile : public ITile
{
public:
	CLaserTile(IGameObject* parent);
	~CLaserTile() override = default;

	void Initialize(DIRECTION_ID direction);
	void Update() override;
	void Draw() override;
	void Draw_Lit() override;
	void Finalize() override;

	TILE_ID GetTileID() const override;																		// タイルIDを取得
	bool IsMovable() const override;																		// 移動可能か判定
	std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const override;						// 入力光に対する出力光を取得
	void DrawLaserTrail(aqua::CAnimationSprite laser_sprite, const SLaserData& laser_data) const override;	// 同じ位置のレーザーを描画
	void DrawOutLine() override;																			// アウトラインを描画

	std::list<SLaserData> GetGeneratedLaser() const;

	DIRECTION_ID GetDirectionID() const { return m_Direction; }	// 方向IDを取得

private:
	DIRECTION_ID m_Direction;	// 方向ID

	aqua::CSprite m_Sprite;			// タイル画像
	aqua::CSprite m_OutLineSprite;	// アウトライン画像
};