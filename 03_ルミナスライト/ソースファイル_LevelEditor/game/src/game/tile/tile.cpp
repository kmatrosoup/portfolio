#include "tile.h"

ITile::ITile(IGameObject* parent, const std::string& name)
	: IGameObject(parent, name, "Tile")
	, m_DispSize(0.0f)
{
}

void ITile::SetLocate(const aqua::CPoint& locate)
{
	m_Locate = locate;
}

const aqua::CPoint& ITile::GetLocate() const
{
	return m_Locate;
}

void ITile::SetPosition(const aqua::CVector2& position)
{
	m_Position = position;
}

void ITile::SetDispSize(float size)
{
	m_DispSize = size;
}
