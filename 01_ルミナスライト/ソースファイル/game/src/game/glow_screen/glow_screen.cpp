#include "glow_screen.h"

constexpr int CGlowScreen::m_glow_sprite_num;

CGlowScreen::CGlowScreen(IGameObject* parent)
	: IGameObject(parent, "GlowScreen")
{
}

void CGlowScreen::Initialize()
{
	m_SourceSurface.Create(aqua::GetWindowWidth(), aqua::GetWindowHeight(), true);
	m_DownScaleSurface.Create(aqua::GetWindowWidth() / 4, aqua::GetWindowHeight() / 4, true);

	m_SourceSprite.Create(m_SourceSurface);
	m_SourceSprite.scale = aqua::CVector2::ONE / 4.0f;

	for (int i = 0; i < m_glow_sprite_num; ++i)
	{
		m_GlowSprite[i].Create(m_DownScaleSurface);
		m_GlowSprite[i].blend_mode = aqua::ALPHABLEND::ADD;
		m_GlowSprite[i].scale = aqua::CVector2::ONE * 4.0f;
		int pixel_width = 1 << i;
		pixel_width = max(pixel_width, 8);
		pixel_width = min(pixel_width, 32);
		m_GlowSprite[i].ApplyGaussFilter(pixel_width, 100 << i);
	}
}

void CGlowScreen::Draw()
{
	m_DownScaleSurface.Begin();
	aqua::Clear(0xff000000);
	m_SourceSprite.Draw();
	m_DownScaleSurface.End();

	for (int i = 0; i < m_glow_sprite_num; ++i)
		m_GlowSprite[i].Draw();
}

void CGlowScreen::Finalize()
{
	m_SourceSurface.Delete();
	m_DownScaleSurface.Delete();
	m_SourceSprite.Delete();
	for (int i = 0; i < m_glow_sprite_num; ++i)
		m_GlowSprite[i].Delete();
}

void CGlowScreen::Begin()
{
	m_SourceSurface.Begin();
}

void CGlowScreen::End()
{
	m_SourceSurface.End();
}

void CGlowScreen::Clear()
{
	m_SourceSurface.Begin();
	aqua::Clear(0xff000000);
	m_SourceSurface.End();
}
