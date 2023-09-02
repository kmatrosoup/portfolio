#include "line_effect.h"
#include <algorithm>

const float CLineEffect::m_max_wait_time = 0.3f;
const float CLineEffect::m_min_active_time = 0.3f;
const float CLineEffect::m_max_active_time = 0.6f;
const float CLineEffect::m_min_base_thickness = 10.0f;
const float CLineEffect::m_max_base_thickness = 40.0f;

CLineEffect::CLineEffect(aqua::IGameObject* parent)
	: IEffect(parent, "LineEffect")
	, m_BaseThickness(0.0f)
{
}

void CLineEffect::Initialize(float pos_y)
{
	m_PositionY = pos_y;
	m_WaitTimer.Setup(m_max_wait_time * aqua::Rand(1000) / 1000.0f);
	m_ActiveTimer.Setup(m_min_active_time + (m_max_active_time - m_min_active_time) * aqua::Rand(1000) / 1000.0f);
	m_BaseThickness = m_min_base_thickness + (m_max_base_thickness - m_min_base_thickness) * aqua::Rand(1000) / 1000.0f;
	m_LinePrimitive.Setup(
		aqua::CVector2(0.0f, m_PositionY),
		aqua::CVector2((float)aqua::GetWindowWidth(), m_PositionY),
		0xff202830,
		0.0f,
		aqua::ALPHABLEND::ADD);
}

void CLineEffect::Update()
{
	if (!m_WaitTimer.Finished())
	{
		m_WaitTimer.Update();
		return;
	}

	m_LinePrimitive.thickness = aqua::easing::OutCubic(m_ActiveTimer.GetTime(), m_ActiveTimer.GetLimit(), m_BaseThickness, 0.0f);

	m_ActiveTimer.Update();
	if (m_ActiveTimer.Finished())
		DeleteObject();
}

void CLineEffect::Draw()
{
	if (!m_WaitTimer.Finished())
		return;

	m_LinePrimitive.Draw();
}

void CLineEffect::Finalize()
{
}
