#include "fade_screen.h"

const float CFadeScreen::m_fade_time = 0.4f;

CFadeScreen::CFadeScreen(aqua::IGameObject* parent)
	: IGameObject(parent, "FadeScreen")
	, m_IsFade(false)
	, m_Param(0.0f)
	, m_BaseSprite()
	, m_DestSurface()
	, m_DestSprite()
{
}

void CFadeScreen::Initialize()
{
	m_BaseSprite.Create("data\\scene_change_effect.png");
	m_DestSurface.Create((int)aqua::GetWindowWidth(), (int)aqua::GetWindowHeight(), true);
	m_DestSprite.Create(m_DestSurface);
}

void CFadeScreen::Update()
{
	// 適用率の更新
	m_Param += aqua::GetDeltaTime() / m_fade_time * (m_IsFade - !m_IsFade);
	m_Param = max(m_Param, 0.0f);
	m_Param = min(m_Param, 1.0f);

	// スクリーンに書き込む
	m_DestSurface.Begin();
	aqua::Clear(0x00000000);
	m_BaseSprite.Draw();
	m_DestSurface.End();
	DxLib::GraphFilter(m_DestSurface.GetTexture().GetResourceHandle(), DX_GRAPH_FILTER_TWO_COLOR,
		(int)(m_Param * 255.0f),
		0xff000000,
		255,
		0xff000000,
		0);
}

void CFadeScreen::Draw()
{
	m_DestSprite.Draw();
}

void CFadeScreen::Draw_Lit()
{
	m_DestSprite.Draw();
}

void CFadeScreen::Finalize()
{
	m_BaseSprite.Delete();
	m_DestSurface.Delete();
	m_DestSprite.Delete();
}
