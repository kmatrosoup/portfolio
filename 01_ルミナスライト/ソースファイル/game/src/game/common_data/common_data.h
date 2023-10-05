#pragma once

#include "aqua.h"
#include <string>

class CCommonData : public aqua::IGameObject
{
public:
	CCommonData(IGameObject* parent);
	~CCommonData() override = default;

public:
	int selectedLevel;	// ‘I‘ðƒŒƒxƒ‹
};
