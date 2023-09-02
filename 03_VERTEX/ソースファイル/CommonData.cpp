//////////////////////
//   プリプロセス   //
//////////////////////
#include "CommonData.h"
#include "DxLib.h"
#include "InputMgr.h"
#include "Player.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 公開変数 */
eINPUT_DEVICE cCommonData::InputDevice[4]{ eINPUT_DEVICE::KEY_MOUSE, eINPUT_DEVICE::PAD_1, eINPUT_DEVICE::PAD_2, eINPUT_DEVICE::PAD_3 };	// 入力デバイスID
int cCommonData::FieldID(0);																												// フィールドID
int cCommonData::Ranking[4]{ -1, -1, -1, -1 };																								// プレイヤー順位
bool cCommonData::Player4Flag(false);																										// 4人モードフラグ
sPlayerInitData cCommonData::PlayerInitData[4]{};																							// プレイヤー初期化情報

/* 非公開変数 */
int cCommonData::m_Image_Icon_Status[(int)eSTATUS::MAX]{ -1, -1, -1, -1 };					// 画像ハンドル・アイコン・ステータス
int cCommonData::m_Image_Icon_Bullet[(int)eBULLET::MAX]{ -1, -1, -1, -1 };					// 画像ハンドル・アイコン・弾
int cCommonData::m_Image_Icon_Skill[(int)eSKILL::MAX]{ -1, -1, -1, -1, -1, -1, -1, -1 };	// 画像ハンドル・アイコン・スキル
int cCommonData::m_Image_Icon_Ability[(int)eABILITY::MAX]{ -1, -1, -1, -1, -1, -1 };		// 画像ハンドル・アイコン・能力

//////////////////
//   公開関数   //
//////////////////

/* 初期化 */
void cCommonData::Init()
{
	m_Image_Icon_Status[(int)eSTATUS::HP] = LoadGraph("data\\image\\icon\\status_hp.png");
	m_Image_Icon_Status[(int)eSTATUS::ATK] = LoadGraph("data\\image\\icon\\status_atk.png");
	m_Image_Icon_Status[(int)eSTATUS::INTERVAL] = LoadGraph("data\\image\\icon\\status_interval.png");
	m_Image_Icon_Status[(int)eSTATUS::BULLET] = LoadGraph("data\\image\\icon\\status_bullet.png");

	m_Image_Icon_Bullet[(int)eBULLET::NORMAL] = LoadGraph("data\\image\\icon\\bullet_normal.png");
	m_Image_Icon_Bullet[(int)eBULLET::CHARGE] = LoadGraph("data\\image\\icon\\bullet_charge.png");
	m_Image_Icon_Bullet[(int)eBULLET::LASER] = LoadGraph("data\\image\\icon\\bullet_laser.png");
	m_Image_Icon_Bullet[(int)eBULLET::MISSILE] = LoadGraph("data\\image\\icon\\bullet_missile.png");

	m_Image_Icon_Skill[(int)eSKILL::DASH] = LoadGraph("data\\image\\icon\\skill_dash.png");
	m_Image_Icon_Skill[(int)eSKILL::SHIELD] = LoadGraph("data\\image\\icon\\skill_shield.png");
	m_Image_Icon_Skill[(int)eSKILL::ELECTRIC_TRAP] = LoadGraph("data\\image\\icon\\skill_electric_trap.png");
	m_Image_Icon_Skill[(int)eSKILL::MINE_TRAP] = LoadGraph("data\\image\\icon\\skill_mine_trap.png");
	m_Image_Icon_Skill[(int)eSKILL::MENDING] = LoadGraph("data\\image\\icon\\skill_mending.png");
	m_Image_Icon_Skill[(int)eSKILL::FULL_BURST] = LoadGraph("data\\image\\icon\\skill_full_burst.png");
	m_Image_Icon_Skill[(int)eSKILL::QUICK_RELOAD] = LoadGraph("data\\image\\icon\\skill_quick_reload.png");
	m_Image_Icon_Skill[(int)eSKILL::RANDOM] = LoadGraph("data\\image\\icon\\skill_random.png");

	m_Image_Icon_Ability[(int)eABILITY::RICOCHET] = LoadGraph("data\\image\\icon\\ability_ricochet.png");
	m_Image_Icon_Ability[(int)eABILITY::BLESSING] = LoadGraph("data\\image\\icon\\ability_blessing.png");
	m_Image_Icon_Ability[(int)eABILITY::ADVERSITY] = LoadGraph("data\\image\\icon\\ability_adversity.png");
	m_Image_Icon_Ability[(int)eABILITY::BERSERKER] = LoadGraph("data\\image\\icon\\ability_berserker.png");
	m_Image_Icon_Ability[(int)eABILITY::REVENGER_ORB] = LoadGraph("data\\image\\icon\\ability_revenger_orb.png");
	m_Image_Icon_Ability[(int)eABILITY::DEATH] = LoadGraph("data\\image\\icon\\ability_death.png");
}

/* 終了 */
void cCommonData::Fin()
{
	for (int i = 0; i < (int)eSTATUS::MAX; ++i) {
		DeleteGraph(m_Image_Icon_Status[i]);
		m_Image_Icon_Status[i] = -1;
	}
	for (int i = 0; i < (int)eBULLET::MAX; ++i) {
		DeleteGraph(m_Image_Icon_Bullet[i]);
		m_Image_Icon_Bullet[i] = -1;
	}
	for (int i = 0; i < (int)eSKILL::MAX; ++i) {
		DeleteGraph(m_Image_Icon_Skill[i]);
		m_Image_Icon_Skill[i] = -1;
	}
	for (int i = 0; i < (int)eABILITY::MAX; ++i) {
		DeleteGraph(m_Image_Icon_Ability[i]);
		m_Image_Icon_Ability[i] = -1;
	}
}

/* アイコンを取得・ステータス */
int cCommonData::GetIcon_Status(eSTATUS type)
{
	if ((int)type < 0 || (int)type >= (int)eSTATUS::MAX)
		return -1;

	return m_Image_Icon_Status[(int)type];
}

/* アイコンを取得・弾 */
int cCommonData::GetIcon_Bullet(eBULLET type)
{
	if ((int)type < 0 || (int)type >= (int)eBULLET::MAX)
		return -1;

	return m_Image_Icon_Bullet[(int)type];
}

/* アイコンを取得・スキル */
int cCommonData::GetIcon_Skill(eSKILL type)
{
	if ((int)type < 0 || (int)type >= (int)eSKILL::MAX)
		return -1;

	return m_Image_Icon_Skill[(int)type];
}

/* アイコンを取得・能力 */
int cCommonData::GetIcon_Ability(eABILITY type)
{
	if ((int)type < 0 || (int)type >= (int)eABILITY::MAX)
		return -1;

	return m_Image_Icon_Ability[(int)type];
}
