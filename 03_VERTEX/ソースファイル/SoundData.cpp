//////////////////////
//   プリプロセス   //
//////////////////////
#include "SoundData.h"
#include "DxLib.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
cSoundData* cSoundData::m_pInstance(nullptr);	// インスタンスのポインタ

//////////////////
//   公開関数   //
//////////////////

/* インスタンスの生成 */
void cSoundData::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cSoundData;
}

/* インスタンスの解放 */
void cSoundData::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

/* インスタンスの取得 */
cSoundData& cSoundData::GetInstance()
{
	return *m_pInstance;
}

/* コンストラクタ */
cSoundData::cSoundData()
{
	m_SoundHandle[(int)eSOUND::CURSOR] = LoadSoundMem("data\\sound\\cursor.wav");
	m_SoundHandle[(int)eSOUND::DECIDE] = LoadSoundMem("data\\sound\\decide.wav");
	m_SoundHandle[(int)eSOUND::CANCEL] = LoadSoundMem("data\\sound\\cancel.wav");
	m_SoundHandle[(int)eSOUND::FAIL] = LoadSoundMem("data\\sound\\fail.wav");
	m_SoundHandle[(int)eSOUND::READY] = LoadSoundMem("data\\sound\\ready.wav");
	m_SoundHandle[(int)eSOUND::GAMESTART] = LoadSoundMem("data\\sound\\gamestart.wav");
	m_SoundHandle[(int)eSOUND::COWNTDOWN] = LoadSoundMem("data\\sound\\battle\\countdown.wav");

	m_SoundHandle[(int)eSOUND::DAMAGE] = LoadSoundMem("data\\sound\\battle\\damage.wav");
	m_SoundHandle[(int)eSOUND::CHARGE] = LoadSoundMem("data\\sound\\battle\\charge.wav");
	m_SoundHandle[(int)eSOUND::RUN_SKILL] = LoadSoundMem("data\\sound\\battle\\run_skill.wav");
	m_SoundHandle[(int)eSOUND::RICOCHET] = LoadSoundMem("data\\sound\\battle\\ricochet.wav");
	m_SoundHandle[(int)eSOUND::BLESSING] = LoadSoundMem("data\\sound\\battle\\blessing.wav");
	m_SoundHandle[(int)eSOUND::REFLECT] = LoadSoundMem("data\\sound\\battle\\reflect.wav");

	m_SoundHandle[(int)eSOUND::BULLET_NORMAL] = LoadSoundMem("data\\sound\\battle\\bullet_normal.wav");
	m_SoundHandle[(int)eSOUND::BULLET_CHARGE_0] = LoadSoundMem("data\\sound\\battle\\bullet_charge0.wav");
	m_SoundHandle[(int)eSOUND::BULLET_CHARGE_1] = LoadSoundMem("data\\sound\\battle\\bullet_charge1.wav");
	m_SoundHandle[(int)eSOUND::BULLET_CHARGE_2] = LoadSoundMem("data\\sound\\battle\\bullet_charge2.wav");
	m_SoundHandle[(int)eSOUND::BULLET_LASER] = LoadSoundMem("data\\sound\\battle\\bullet_laser.wav");
	m_SoundHandle[(int)eSOUND::BULLET_MISSILE] = LoadSoundMem("data\\sound\\battle\\bullet_missile.wav");

	m_SoundHandle[(int)eSOUND::BLAST] = LoadSoundMem("data\\sound\\battle\\blast.wav");
	m_SoundHandle[(int)eSOUND::DEATH] = LoadSoundMem("data\\sound\\battle\\death.wav");
	m_SoundHandle[(int)eSOUND::DESTROY] = LoadSoundMem("data\\sound\\battle\\destroy.wav");

	m_SoundHandle[(int)eSOUND::SKILL_DASH] = LoadSoundMem("data\\sound\\battle\\skill_dash.wav");
	m_SoundHandle[(int)eSOUND::SKILL_SHIELD] = LoadSoundMem("data\\sound\\battle\\skill_shield.wav");
	m_SoundHandle[(int)eSOUND::SKILL_MENDING] = LoadSoundMem("data\\sound\\battle\\skill_mending.wav");
	m_SoundHandle[(int)eSOUND::SKILL_FULL_BURST] = LoadSoundMem("data\\sound\\battle\\skill_full_burst.wav");
	m_SoundHandle[(int)eSOUND::SKILL_QUICK_RELOAD_1] = LoadSoundMem("data\\sound\\battle\\skill_quick_reload_1.wav");
	m_SoundHandle[(int)eSOUND::SKILL_QUICK_RELOAD_2] = LoadSoundMem("data\\sound\\battle\\skill_quick_reload_2.wav");
}

/* デストラクタ */
cSoundData::~cSoundData()
{
	for (int i = 0; i < (int)eSOUND::MAX; ++i)
		DeleteSoundMem(m_SoundHandle[i]);
}

/* 再生 */
void cSoundData::Play(eSOUND Sound)
{
	if (Sound == eSOUND::MAX)
		return;

	PlaySoundMem(m_SoundHandle[(int)Sound], DX_PLAYTYPE_BACK);
}
