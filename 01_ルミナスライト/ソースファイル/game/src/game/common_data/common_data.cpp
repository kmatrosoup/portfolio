#include "common_data.h"

CCommonData::CCommonData(IGameObject* parent)
	: IGameObject(parent, "CommonData")
	, selectedLevel(1)
{
}
