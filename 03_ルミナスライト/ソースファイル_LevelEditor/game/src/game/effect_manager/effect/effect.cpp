#include "effect.h"

IEffect::IEffect(aqua::IGameObject* parent, const std::string& name)
	: IGameObject(parent, name, "Effect")
{
}
