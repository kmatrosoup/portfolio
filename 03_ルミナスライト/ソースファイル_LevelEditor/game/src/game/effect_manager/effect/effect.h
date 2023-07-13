#pragma once

#include "aqua.h"

class IEffect : public aqua::IGameObject
{
public:
	IEffect(aqua::IGameObject* parent, const std::string& name);
	virtual ~IEffect() override = default;
};
