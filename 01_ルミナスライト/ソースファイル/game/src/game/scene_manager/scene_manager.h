#pragma once

#include "aqua.h"

class IScene;

class CSceneManager :
	public aqua::IGameObject
{
public:
	CSceneManager(IGameObject* parent);
	~CSceneManager() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Draw_Lit();
	void Finalize() override;

private:
	IScene* m_pScene;	// シーンオブジェクト
};
