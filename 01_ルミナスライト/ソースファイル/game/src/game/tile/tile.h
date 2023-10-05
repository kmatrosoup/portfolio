#pragma once

#include "aqua.h"
#include "tile_id.h"
#include "../laser_data/laser_data.h"

class ITile : public aqua::IGameObject
{
public:
	ITile(IGameObject* parent, const std::string& name);
	virtual ~ITile() override = default;

	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Draw_Lit() = 0;
	virtual void Finalize() override = 0;

	virtual TILE_ID GetTileID() const = 0;																		// タイルIDを取得
	virtual bool IsMovable() const = 0;																			// 移動可能か判定
	virtual std::list<SLaserData> GetConvertedLaser(const SLaserData& laser) const = 0;							// 入力光に対する出力光を取得
	virtual void DrawLaserTrail(aqua::CAnimationSprite laser_sprite, const SLaserData& laser_data) const = 0;	// 同じ位置のレーザーを描画
	virtual void DrawOutLine() {};																				// アウトラインを描画

	void SetLocate(const aqua::CPoint& locate);			// 位置を設定
	const aqua::CPoint& GetLocate() const;				// 位置を取得
	void SetPosition(const aqua::CVector2& position);	// 座標を設定
	void SetDispSize(float size);						// 表示スケールを設定

protected:
	static const int m_tile_size;	// タイルの大きさ

	aqua::CPoint m_Locate;		// タイル位置
	aqua::CVector2 m_Position;	// 座標
	float m_DispSize;			// 表示スケール
};
