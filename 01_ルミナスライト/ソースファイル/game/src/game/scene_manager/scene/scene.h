#pragma once

#include "aqua.h"
#include "scene_id.h"

class IScene :
	public aqua::IGameObject
{
public:
	IScene(IGameObject* parent, const std::string& name);
	virtual ~IScene() override = default;

	virtual void Initialize() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Draw_Lit() = 0;
	virtual void Finalize() override = 0;

	SCENE_ID GetNextScene() const;	// 次のシーンを取得

protected:
	void ChangeScene(SCENE_ID id);	// シーン切り替え

private:
	SCENE_ID m_NextSceneID;	// 次のシーン
};
