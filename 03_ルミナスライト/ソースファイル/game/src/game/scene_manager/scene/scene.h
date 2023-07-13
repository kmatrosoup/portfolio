#pragma once

#include "aqua.h"
#include "scene_id.h"

class IScene :
	public aqua::IGameObject
{
public:
	IScene(IGameObject* parent, const std::string& name);
	virtual ~IScene() override = default;

	SCENE_ID GetNextScene() const;

protected:
	void ChangeScene(SCENE_ID id);

private:
	SCENE_ID m_NextSceneID;
};
