//////////////////////
//   プリプロセス   //
//////////////////////
#include "BgmData.h"
#include "DxLib.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
cBgmData* cBgmData::m_pInstance(nullptr);	// インスタンスのポインタ

//////////////////
//   公開関数   //
//////////////////

/* インスタンスの生成 */
void cBgmData::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cBgmData;
}

/* インスタンスの解放 */
void cBgmData::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

/* インスタンスの取得 */
cBgmData& cBgmData::GetInstance()
{
	return *m_pInstance;
}

/* コンストラクタ */
cBgmData::cBgmData()
{
	m_BgmHandle[(int)eBGM::TITLE] = LoadSoundMem("data\\bgm\\FreeBGM_Underworld.mp3");
	m_BgmHandle[(int)eBGM::PREVIEW] = -1;
	m_BgmHandle[(int)eBGM::SELECT] = -1;
	m_BgmHandle[(int)eBGM::BATTLE] = LoadSoundMem("data\\bgm\\Assimilator_free-bgm-ver2.mp3");
	m_BgmHandle[(int)eBGM::RESULT] = LoadSoundMem("data\\bgm\\Isochronous_free_ver.mp3");

	for (int i = 0; i < (int)eBGM::MAX; ++i)
		ChangeVolumeSoundMem(128, m_BgmHandle[i]);
}

/* デストラクタ */
cBgmData::~cBgmData()
{
	for (int i = 0; i < (int)eBGM::MAX; ++i)
		DeleteSoundMem(m_BgmHandle[i]);
}

/* 再生 */
void cBgmData::Play(eBGM Bgm, bool TopPositionFlag)
{
	if (Bgm == eBGM::MAX)
		return;

	for (int i = 0; i < (int)eBGM::MAX; ++i)
		if (TopPositionFlag || i != (int)Bgm)
			StopSoundMem(m_BgmHandle[i]);
	if (TopPositionFlag || !CheckSoundMem(m_BgmHandle[(int)Bgm]))
		PlaySoundMem(m_BgmHandle[(int)Bgm], DX_PLAYTYPE_LOOP);
}

/* 停止 */
void cBgmData::Stop(eBGM Bgm)
{
	if (Bgm == eBGM::MAX) {
		for (int i = 0; i < (int)eBGM::MAX; ++i)
			StopSoundMem(m_BgmHandle[i]);
	}
	else
		StopSoundMem(m_BgmHandle[(int)Bgm]);
}
