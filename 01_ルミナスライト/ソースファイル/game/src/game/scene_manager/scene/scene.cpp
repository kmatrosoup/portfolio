#include "scene.h"

IScene::IScene(IGameObject* parent, const std::string& name)
	: IGameObject(parent, name)
	, m_NextSceneID(SCENE_ID::DUMMY)
{
}

SCENE_ID IScene::GetNextScene() const
{
	return m_NextSceneID;
}

void IScene::ChangeScene(SCENE_ID id)
{
	m_NextSceneID = id;
}
