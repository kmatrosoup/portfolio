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
	static constexpr int m_glow_sprite_num = 6;

	aqua::CSurface m_SourceSurface;
	aqua::CSurface m_DownScaleSurface;
	aqua::CSprite m_SourceSprite;
	aqua::CSprite m_GlowSprite[m_glow_sprite_num];
};
