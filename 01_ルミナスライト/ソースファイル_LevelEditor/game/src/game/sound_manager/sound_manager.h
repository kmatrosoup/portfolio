#pragma once

#include "aqua.h"
#include "sound_id.h"

class CSoundManager : public aqua::IGameObject
{
public:
	CSoundManager(IGameObject* parent);
	~CSoundManager() override = default;

	void Initialize() override;
	void Finalize() override;

	void Play(SOUND_ID id);
	void Stop(SOUND_ID id);
	bool IsPlaying(SOUND_ID id);

private:
	void LoadSoundData(const std::string& file_name);

private:
	aqua::CSoundPlayer m_SoundData[(int)SOUND_ID::MAX];
};
