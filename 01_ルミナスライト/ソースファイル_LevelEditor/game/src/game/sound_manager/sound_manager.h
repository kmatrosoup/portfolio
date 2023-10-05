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

	void Play(SOUND_ID id);			// 再生
	void Stop(SOUND_ID id);			// 停止
	bool IsPlaying(SOUND_ID id);	// 再生中判定

private:
	void LoadSoundData(const std::string& file_name); // 音声ファイルの読み込み

private:
	aqua::CSoundPlayer m_SoundData[(int)SOUND_ID::MAX];	// サウンドデータ
};
