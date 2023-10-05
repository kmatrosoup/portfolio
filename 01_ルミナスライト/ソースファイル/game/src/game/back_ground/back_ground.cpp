#include "back_ground.h"

CBackGround::CBackGround(aqua::IGameObject* parent)
	: IGameObject(parent, "BackGround")
	, paramR(0.0f)
	, paramG(0.0f)
	, paramB(0.0f)
	, m_FillBox()
{
}

void CBackGround::Initialize()
{
	m_FillBox.Setup(aqua::CVector2::ZERO, (float)aqua::GetWindowWidth(), (float)aqua::GetWindowHeight());
}

void CBackGround::Update()
{
}

void CBackGround::Draw()
{
	m_FillBox.color.red = (unsigned char)(255.0f * paramR);
	m_FillBox.color.green = (unsigned char)(255.0f * paramG);
	m_FillBox.color.blue = (unsigned char)(255.0f * paramB);
	m_FillBox.Draw();
}

void CBackGround::Finalize()
{
}
