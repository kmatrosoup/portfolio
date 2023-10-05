#pragma once

#include "aqua.h"

class CGlowScreen : public aqua::IGameObject
{
public:
	CGlowScreen(IGameObject* parent);
	~CGlowScreen() override = default;

	void Initialize();
	void Draw();
	void Finalize();

	void Begin();		// スクリーンへの描画開始
	void End();			// スクリーンへの描画終了
	void Clear();		// スクリーンのクリア

private:
	static constexpr int m_glow_sprite_num = 6;	// 発光用画像の枚数

	aqua::CSurface m_SourceSurface;					// 発光用元画像スクリーン
	aqua::CSurface m_DownScaleSurface;				// 発光用低解像スクリーン
	aqua::CSprite m_SourceSprite;					// 発光用元画像
	aqua::CSprite m_GlowSprite[m_glow_sprite_num];	// 発光用結果画像
};
