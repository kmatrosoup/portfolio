#include "lamp.h"

CLamp::CLamp(IGameObject* parent)
	: IGameObject(parent, "Lamp")
	, m_AngleVelocity(0.0f)
{
}

void CLamp::Initialize(float pos_x, float anchor_y, bool lit_flag)
{
	m_PositionX = pos_x;

	m_BaseSprite.Create("data\\lamp.png");
	m_BaseSprite.scale.x = 4.0f;
	m_BaseSprite.scale.y = 4.0f;
	m_BaseSprite.anchor.x = m_BaseSprite.GetTextureWidth() / 2.0f;
	m_BaseSprite.anchor.y = anchor_y / m_BaseSprite.scale.y;
	m_LitSprite.Create("data\\lamp_lit.png");
	m_LitSprite.scale = m_BaseSprite.scale;
	m_LitSprite.anchor = m_BaseSprite.anchor;
	m_LitSprite.blend_mode = aqua::ALPHABLEND::ADD;
	m_LitSprite.visible = lit_flag;
}

void CLamp::Update()
{
	{
		aqua::CVector2 mpos = (aqua::CVector2)aqua::mouse::GetCursorPos();
		aqua::CVector2 mpos_prev = mpos - (aqua::CVector2)aqua::mouse::GetCursorMovement();

		if (std::signbit(mpos_prev.x - m_PositionX) != std::signbit(mpos.x - m_PositionX) &&
			min(mpos_prev.y, mpos.y) <= m_BaseSprite.GetTextureHeight() * m_BaseSprite.scale.y * (1.0f - m_BaseSprite.anchor.y / (float)m_BaseSprite.GetTextureHeight()))
			m_AngleVelocity += (mpos_prev.x - mpos.x) * 0.0004f;
	}

	m_AngleVelocity -= m_BaseSprite.rotation * 0.02f;
	m_AngleVelocity *= 0.98f;
	m_BaseSprite.rotation += m_AngleVelocity;
	if (m_BaseSprite.rotation >= aqua::PI / 2.0f)
	{
		m_BaseSprite.rotation = aqua::PI / 2.0f;
		m_AngleVelocity *= -1.0f;
	}
	if (m_BaseSprite.rotation <= -aqua::PI / 2.0f)
	{
		m_BaseSprite.rotation = -aqua::PI / 2.0f;
		m_AngleVelocity *= -1.0f;
	}

	m_BaseSprite.position.x = m_PositionX - m_BaseSprite.anchor.x;
	m_BaseSprite.position.y = -m_BaseSprite.anchor.y;
	m_LitSprite.position = m_BaseSprite.position;
	m_LitSprite.rotation = m_BaseSprite.rotation;
}

void CLamp::Draw()
{
	m_BaseSprite.Draw();
}

void CLamp::Draw_Lit()
{
	m_LitSprite.Draw();
}

void CLamp::Finalize()
{
	m_BaseSprite.Delete();
	m_LitSprite.Delete();
}
