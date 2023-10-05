#include "scene_manager.h"
#include "scene/scene.h"

#include "scene/title/title.h"
#include "scene/level_select/level_select.h"
#include "scene/game_main/game_main.h"

CSceneManager::CSceneManager(IGameObject* parent)
	: IGameObject(parent, "SceneManager")
	, m_pScene(nullptr)
{
}

void CSceneManager::Initialize()
{
	Finalize();

	m_pScene = aqua::CreateGameObject<CTitle>(this);
	m_pScene->Initialize();
}

void CSceneManager::Update()
{
#ifdef _DEBUG
	{
		SCENE_ID next_id = SCENE_ID::DUMMY;

		if (aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::ONE))	next_id = SCENE_ID::TITLE;
		if (aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::TWO))	next_id = SCENE_ID::LEVEL_SELECT;
		if (aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::THREE))	next_id = SCENE_ID::GAME_MAIN;

		if (next_id != SCENE_ID::DUMMY)
		{
			m_pScene->Finalize();
			m_ChildObjectList.erase(std::find(m_ChildObjectList.begin(), m_ChildObjectList.end(), m_pScene));
			AQUA_SAFE_DELETE(m_pScene);

			switch (next_id)
			{
			case SCENE_ID::TITLE:			m_pScene = aqua::CreateGameObject<CTitle>(this);		break;
			case SCENE_ID::LEVEL_SELECT:	m_pScene = aqua::CreateGameObject<CLevelSelect>(this);	break;
			case SCENE_ID::GAME_MAIN:		m_pScene = aqua::CreateGameObject<CGameMain>(this);		break;
			}
			m_pScene->Initialize();
		}
	}
#endif // _DEBUG

	if (m_pScene->GetNextScene() != SCENE_ID::DUMMY)
	{
		// 次のシーンIDを一時保存
		SCENE_ID next_id = m_pScene->GetNextScene();

		// 古いシーンを削除
		m_pScene->Finalize();
		m_ChildObjectList.erase(std::find(m_ChildObjectList.begin(), m_ChildObjectList.end(), m_pScene));
		AQUA_SAFE_DELETE(m_pScene);

		// 次のシーンを生成
		switch (next_id)
		{
		case SCENE_ID::TITLE:			m_pScene = aqua::CreateGameObject<CTitle>(this);		break;
		case SCENE_ID::LEVEL_SELECT:	m_pScene = aqua::CreateGameObject<CLevelSelect>(this);	break;
		case SCENE_ID::GAME_MAIN:		m_pScene = aqua::CreateGameObject<CGameMain>(this);		break;
		}
		m_pScene->Initialize();
	}

	m_pScene->Update();
}

void CSceneManager::Draw()
{
	m_pScene->Draw();
}

void CSceneManager::Draw_Lit()
{
	m_pScene->Draw_Lit();
}

void CSceneManager::Finalize()
{
	if (m_pScene)
	{
		m_pScene->Finalize();
		m_ChildObjectList.erase(std::find(m_ChildObjectList.begin(), m_ChildObjectList.end(), m_pScene));
		AQUA_SAFE_DELETE(m_pScene);
	}
}
