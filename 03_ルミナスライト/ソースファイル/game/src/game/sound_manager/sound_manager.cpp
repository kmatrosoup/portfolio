#include "sound_manager.h"
#include <fstream>

CSoundManager::CSoundManager(IGameObject* parent)
	: IGameObject(parent, "SoundManager")
{
}

void CSoundManager::Initialize()
{
	LoadSoundData("data\\sound\\sound_list.txt");
}

void CSoundManager::Finalize()
{
	for (int i = 0; i < (int)SOUND_ID::MAX; ++i)
		m_SoundData[i].Delete();
}

void CSoundManager::Play(SOUND_ID id)
{
	m_SoundData[(int)id].Play();
}

void CSoundManager::Stop(SOUND_ID id)
{
	m_SoundData[(int)id].Stop();
}

bool CSoundManager::IsPlaying(SOUND_ID id)
{
	return m_SoundData[(int)id].IsPlaying();
}

void CSoundManager::LoadSoundData(const std::string& file_name)
{
	Finalize();

	// ファイルを開く
	std::ifstream ifs(file_name, std::ios::in);
	AQUA_ASSERT(!ifs.fail(), file_name + "の読み込みに失敗しました。\\");

	// 一行ずつ読み込み
	std::string buf;
	for (int i = 0; i < (int)SOUND_ID::MAX; ++i)
	{
		std::getline(ifs, buf);
		m_SoundData[i].Create(buf, i == (int)SOUND_ID::BGM);
	}

	// ファイルを閉じる
	ifs.close();
}