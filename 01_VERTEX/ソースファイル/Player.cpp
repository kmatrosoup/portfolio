//////////////////////
//   プリプロセス   //
//////////////////////
#include "Player.h"
#include "DxLib.h"
#include "Game.h"
#include "InputMgr.h"
#include "CommonData.h"
#include "SoundData.h"
#include "Field.h"
#include "AtkobjMgr.h"
#include "EffectMgr.h"
#include "Easing.h"
#include "MineTrap.h"
#include <cmath>
using namespace std;

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 公開変数 */
const float	cPlayer::Radius(12.0f);	// 衝突半径
const int	cPlayer::MaxCoreNum(2);	// 残基数
const int	cPlayer::MaxOrbNum(5);	// リベンジャーオーブの最大数

/* 非公開変数 */
const float	cPlayer::m_MoveSpeed(0.8f);						// 元の移動速度
const float	cPlayer::m_Friction(0.8f);						// 摩擦係数
const float	cPlayer::m_MuzzleLength(12.0f);					// 銃口までの距離
const int	cPlayer::m_MaxReadyTime(4);						// 準備時間
const int	cPlayer::m_MaxStopTime(8);						// 硬直時間
const int	cPlayer::m_AddStopTime_Laser(8);				// レーザー選択時の追加硬直時間
const int	cPlayer::m_MaxReloadTime(100);					// 装填開始までの時間
const int	cPlayer::m_MaxRepairTime(240);					// 復活時間
const int	cPlayer::m_MaxChargeEffectTime(24);				// チャージ完了エフェクト最大時間
const int	cPlayer::m_MaxSkillCoolTime[(int)eSKILL::MAX]	// スキルのクールタイム
{
	60 * 6,
	60 * 1,
	60 * 16,
	60 * 10,
	60 * 24,
	60 * 18,
	60 * 12,
	60 * 8,
};
const int	cPlayer::m_MaxSkillNeedTime[(int)eSKILL::MAX]	// スキル発動の所要時間
{
	0,
	0,
	0,
	30,
	60 * 3,
	0,
	0,
	0,
};
const int		cPlayer::m_MaxShieldTime(14);			// シールド展開時間
const int		cPlayer::m_MaxElectricTrapTime(300);	// 電気トラップの設置時間
const int		cPlayer::m_MaxDeathBulletTimes(10);		// デス弾発射までの最大回数
const int		cPlayer::m_MaxMendingNum(3);			// 修繕の最大回数
const int		cPlayer::m_MaxRandomTime(90);			// ランダムの発動時間
const float		cPlayer::m_OrbRotation(0.1f);			// リベンジャーオーブの回転速度
const int		cPlayer::m_AdversityLine(3);			// 逆境発動の残りHP
const int		cPlayer::m_AdversityValue(2);			// 逆境発動の増加攻撃力
const int		cPlayer::m_MaxDamageTime(30);			// ダメージ演出の最大時間
const int		cPlayer::m_MaxFrameTime(8);				// 1コマのフレーム数
const int		cPlayer::m_DispOffset(-4);				// 表示位置のズレ
const double	cPlayer::m_DispScale(1.6);				// 表示する拡大率

bool	cPlayer::m_LoadedFlag(false);			// リソースの読み込みフラグ
int		cPlayer::m_Image_ChargeRing(-1);		// 画像ハンドル・チャージリング
int		cPlayer::m_Image_SkillTimeRing(-1);		// 画像ハンドル・スキルリング
int		cPlayer::m_Image_Player[3 * 8]{};		// 画像ハンドル・プレイヤー
int		cPlayer::m_Image_PlayerColor[3 * 8]{};	// 画像ハンドル・プレイヤーの色部分

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cPlayer::cPlayer(int PlayerID, sPlayerInitData InitData)
	: PlayerID(PlayerID)
	, MaxHP(10 + InitData.hp * 2)
	, AtkDamage(1 + InitData.atk)
	, MaxShotCoolTime(48 - InitData.interval * 7)
	, MaxBulletNum(6 + InitData.bulletnum)
	, Bullet(InitData.bullet)
	, Skill(InitData.skill)
	, Ability(InitData.ability)
	, MaxSkillCoolTime((Skill != eSKILL::MAX) ? m_MaxSkillCoolTime[(int)Skill] : 0)
	, MaxMineTrapNum(3 + InitData.bulletnum / 3)

	, CoreNum(m_CoreNum)
	, Pos(m_Pos)
	, Angle(m_Angle)
	, HP(m_HP)
	, BulletNum(m_BulletNum)
	, SkillCoolTime(m_SkillCoolTime)
	, OrbNum(m_OrbNum)
	, OrbAngle(m_OrbAngle)

	, m_CoreNum(MaxCoreNum)
	, m_State(eSTATE::MOVE)
	, m_Motion(0.0f, 0.0f)
	, m_Angle(0.0f)
	, m_StateTimer(0)
	, m_HP(MaxHP)
	, m_ShotCoolTime(0)
	, m_BulletNum(MaxBulletNum)
	, m_ReloadInterval(18 - InitData.bulletnum)
	, m_ReloadTime(0)
	, m_SkillCoolTime(0)
	, m_SkillNeedTime(0)
	, m_ShotFlag(false)
	, m_RunSkillFlag(false)
	, m_AimFlag(false)
	, m_ChargeTime_First(60 - InitData.interval * 5)
	, m_ChargeTime_Second(120 - InitData.interval * 10)
	, m_ChargeTime(0)
	, m_ChargeEffectTime(0)
	, m_FullburstValue(0)
	, m_BerserkerValue(0)
	, m_PalsyTime(0)
	, m_ShieldTime(0)
	, m_ElectricTrapTime(0)
	, m_MendingNum(m_MaxMendingNum)
	, m_RandomTime(0)
	, m_RandomInterval(0)
	, m_NowRandomSkill(eSKILL::MAX)
	, m_RunRandomSkillTimer(0)
	, m_RicochetTimes((InitData.ability == eABILITY::RICOCHET) * 1)
	, m_DeathBulletTimes(GetRand(m_MaxDeathBulletTimes))
	, m_OrbNum(0)
	, m_OrbAngle(0.0f)
	, m_DamageTimer(0)
	, m_AnimationTimer(0)
{
}

/* デストラクタ */
cPlayer::~cPlayer()
{
}

/* 更新 */
void cPlayer::Update()
{
	// タイマー制御
	if (m_StateTimer > 0)			--m_StateTimer;
	if (m_ShotCoolTime > 0)			--m_ShotCoolTime;
	if (m_SkillCoolTime > 0)		--m_SkillCoolTime;
	if (m_ChargeEffectTime > 0)		--m_ChargeEffectTime;
	if (m_PalsyTime > 0)			--m_PalsyTime;
	if (m_ShieldTime > 0)			--m_ShieldTime;
	if (m_ElectricTrapTime > 0)		--m_ElectricTrapTime;
	if (m_RunRandomSkillTimer > 0)	--m_RunRandomSkillTimer;
	if (m_DamageTimer > 0)			--m_DamageTimer;
	{
		if (m_ReloadTime >= -m_ReloadInterval)
			--m_ReloadTime;
		if (m_ReloadTime < -m_ReloadInterval)
			if (m_BulletNum < MaxBulletNum)
				m_ReloadTime = 0;
		if (m_ReloadTime == 0)
			if (m_BulletNum < MaxBulletNum)
				++m_BulletNum;
	}
	if (Skill == eSKILL::MAX || !cInputMgr::Keep(cCommonData::InputDevice[PlayerID], eINPUT_BUTTON::BUTTON_1) || m_State != eSTATE::MOVE)
		m_SkillNeedTime = 0;
	m_ShotFlag = false;
	m_RunSkillFlag = false;
	m_AimFlag = cInputMgr::Keep(cCommonData::InputDevice[PlayerID], eINPUT_BUTTON::BUTTON_L);

	int inputX = cInputMgr::Keep(cCommonData::InputDevice[PlayerID], eINPUT_BUTTON::STICK_RIGHT) - cInputMgr::Keep(cCommonData::InputDevice[PlayerID], eINPUT_BUTTON::STICK_LEFT);
	int inputY = cInputMgr::Keep(cCommonData::InputDevice[PlayerID], eINPUT_BUTTON::STICK_DOWN) - cInputMgr::Keep(cCommonData::InputDevice[PlayerID], eINPUT_BUTTON::STICK_UP);
	float angle_prev = m_Angle;
	float move_speed = m_MoveSpeed;
	if (m_PalsyTime > 0)				move_speed *= 0.2f;
	if (m_SkillNeedTime > 0)			move_speed *= 0.5f;
	if (m_AimFlag)						move_speed *= 0.5f;
	if (m_State == eSTATE::REPAIR)		move_speed *= 1.5f;
	if (m_State == eSTATE::CHARGE)		move_speed *= 0.5f;
	if (m_State == eSTATE::SKILL_DASH)	move_speed *= 3.0f;
	if (Ability == eABILITY::BERSERKER) {
		switch (m_BerserkerValue) {
		break;	case -2:	move_speed *= 0.8f;
		break;	case -1:	move_speed *= 0.9f;
		break;	case 1:		move_speed *= 1.2f;
		break;	case 2:		move_speed *= 1.4f;
		}
	}

	switch (m_State) {
	case eSTATE::MOVE:	// 移動
	{
		if (inputX || inputY)
		{
			m_Angle = atan2f((float)inputY, (float)inputX);
			m_Motion.x += cosf(m_Angle) * move_speed;
			m_Motion.y += sinf(m_Angle) * move_speed;
		}
		if (m_AimFlag)
			m_Angle = angle_prev;
		m_Motion *= m_Friction;

		if (m_ShotCoolTime <= 0 && m_BulletNum > 0) {
			if (cInputMgr::Keep(cCommonData::InputDevice[PlayerID], eINPUT_BUTTON::BUTTON_2)) {
				if (Bullet == eBULLET::CHARGE) {
					m_State = eSTATE::CHARGE;
					m_ChargeTime = 0;
					cSoundData::GetInstance().Play(eSOUND::CHARGE);
				}
				else {
					m_State = eSTATE::READY;
					m_StateTimer = m_MaxReadyTime;
				}
			}
		}

		if (m_State == eSTATE::MOVE && Skill != eSKILL::MAX && m_SkillCoolTime <= 0) {
			if ((cInputMgr::In(cCommonData::InputDevice[PlayerID], eINPUT_BUTTON::BUTTON_1) && m_SkillNeedTime == 0) ||
				(cInputMgr::Keep(cCommonData::InputDevice[PlayerID], eINPUT_BUTTON::BUTTON_1) && m_SkillNeedTime > 0)) {
				if (++m_SkillNeedTime > m_MaxSkillNeedTime[(int)Skill]) {
					m_SkillNeedTime = 0;
					m_RunSkill();
				}
			}
		}
	}
	break;
	case eSTATE::READY:	// 準備
	{
		if (inputX || inputY)
			m_Angle = atan2f((float)inputY, (float)inputX);
		if (m_AimFlag)
			m_Angle = angle_prev;
		m_Motion *= m_Friction;

		if (m_StateTimer <= 0) {
			{
				VEC2F shot_pos = m_Pos + VEC2F(cosf(m_Angle), sinf(m_Angle)) * m_MuzzleLength;
				int adversity = (Ability == eABILITY::ADVERSITY && m_HP <= m_AdversityLine) * m_AdversityValue;
				if (Ability == eABILITY::DEATH && (m_DeathBulletTimes = __max(m_DeathBulletTimes - 1, 0)) == 0) {
					cAtkobjMgr::GetInstance().Create_DeathBullet(this, shot_pos, m_Angle, m_RicochetTimes);
					m_DeathBulletTimes = GetRand(m_MaxDeathBulletTimes);
				}
				else {
					switch (Bullet) {
					break;	case eBULLET::NORMAL:	cAtkobjMgr::GetInstance().Create_NormalBullet(this, AtkDamage + adversity, shot_pos, m_Angle, m_RicochetTimes);
					break;	case eBULLET::CHARGE:	cAtkobjMgr::GetInstance().Create_NormalBullet(this, AtkDamage + adversity, shot_pos, m_Angle, m_RicochetTimes);
					break;	case eBULLET::LASER:	cAtkobjMgr::GetInstance().Create_Laser(this, AtkDamage + adversity, shot_pos, m_Angle, m_RicochetTimes);
					break;	case eBULLET::MISSILE:	cAtkobjMgr::GetInstance().Create_Misslie(this, AtkDamage + adversity, shot_pos, m_Angle, m_RicochetTimes);
					}
				}
			}
			m_ShotFlag = true;
			m_State = eSTATE::STOP;
			m_StateTimer = m_MaxStopTime + (Bullet == eBULLET::LASER) * m_AddStopTime_Laser;
			m_ShotCoolTime = MaxShotCoolTime;
			--m_BulletNum;
			m_ReloadTime = m_MaxReloadTime;
			m_OrbNum = 0;
		}
	}
	break;
	case eSTATE::STOP:	// 硬直
	{
		m_Motion *= m_Friction;

		if (m_StateTimer <= 0)
			m_State = eSTATE::MOVE;
	}
	break;
	case eSTATE::REPAIR:	// 復活
	{
		if (inputX || inputY)
		{
			m_Angle = atan2f((float)inputY, (float)inputX);
			m_Motion.x += cosf(m_Angle) * move_speed;
			m_Motion.y += sinf(m_Angle) * move_speed;
		}
		if (m_AimFlag)
			m_Angle = angle_prev;
		m_Motion *= m_Friction;

		if (m_StateTimer <= 0) {
			if (m_HP >= MaxHP) {
				m_State = eSTATE::MOVE;
			}
			else {
				++m_HP;
				m_StateTimer = m_MaxRepairTime / MaxHP;
			}
		}
	}
	break;
	case eSTATE::DEAD:	// 死亡
	{
		m_Motion = VEC2F::ZERO;
		m_OrbNum = 0;
	}
	break;
	case eSTATE::CHARGE:	// チャージ
	{
		if (inputX || inputY)
		{
			m_Angle = atan2f((float)inputY, (float)inputX);
			m_Motion.x += cosf(m_Angle) * move_speed;
			m_Motion.y += sinf(m_Angle) * move_speed;
		}
		if (m_AimFlag)
			m_Angle = angle_prev;
		m_Motion *= m_Friction;

		if (m_ChargeTime < m_ChargeTime_Second) {
			++m_ChargeTime;
			if (m_ChargeTime == m_ChargeTime_First || m_ChargeTime == m_ChargeTime_Second) {
				m_ChargeEffectTime = m_MaxChargeEffectTime;
				cSoundData::GetInstance().Play(eSOUND::RUN_SKILL);
			}
		}

		if (cInputMgr::Out(cCommonData::InputDevice[PlayerID], eINPUT_BUTTON::BUTTON_2)) {
			VEC2F shot_pos = m_Pos + VEC2F(cosf(m_Angle), sinf(m_Angle)) * m_MuzzleLength;
			int charge = (m_ChargeTime >= m_ChargeTime_First) + (m_ChargeTime >= m_ChargeTime_Second);
			int adversity = (Ability == eABILITY::ADVERSITY && m_HP <= m_AdversityLine) * m_AdversityValue;
			cAtkobjMgr::GetInstance().Create_ChargeBullet(this, AtkDamage + (m_ChargeTime >= m_ChargeTime_First) + (m_ChargeTime >= m_ChargeTime_Second) * 2 + adversity, shot_pos, m_Angle, m_RicochetTimes, charge);

			m_ShotFlag = true;
			m_State = eSTATE::STOP;
			m_StateTimer = m_MaxStopTime;
			m_ShotCoolTime = MaxShotCoolTime;
			m_BulletNum -= 1 + (m_ChargeTime >= m_ChargeTime_First) + (m_ChargeTime >= m_ChargeTime_Second);
			m_ReloadTime = m_MaxReloadTime;
			m_OrbNum = 0;
		}
	}
	break;
	case eSTATE::SKILL_DASH:	// スキル・ダッシュ
	{
		if (inputX || inputY)
			m_Angle = atan2f((float)inputY, (float)inputX);
		if (m_StateTimer <= 16) {
			if (m_StateTimer == 16) {
				m_Motion.x = cosf(m_Angle) * 20.0f;
				m_Motion.y = sinf(m_Angle) * 20.0f;
				cAtkobjMgr::GetInstance().Create_Wind(this, m_Pos, 60.0f, 8.0f, 1);
				cSoundData::GetInstance().Play(eSOUND::SKILL_DASH);
			}
			else {
				m_Motion.x += cosf(m_Angle) * move_speed;
				m_Motion.y += sinf(m_Angle) * move_speed;
			}
		}
		if (m_AimFlag)
			m_Angle = angle_prev;
		m_Motion *= m_Friction;

		if (m_StateTimer <= 0)
			m_State = eSTATE::MOVE;
	}
	break;
	case eSTATE::SKILL_SHIELD:	// スキル・シールド展開
	{
		m_Motion *= m_Friction;

		if (m_StateTimer == 17)
			cSoundData::GetInstance().Play(eSOUND::SKILL_SHIELD);

		if (m_StateTimer <= 0)
			m_State = eSTATE::MOVE;
	}
	break;
	case eSTATE::SKILL_ELECTRIC_TRAP:	// スキル・静電トラップ
	{
		m_Motion *= m_Friction;

		if (m_StateTimer <= 0) {
			m_ElectricTrapTime = m_MaxElectricTrapTime;
			m_State = eSTATE::MOVE;
		}
	}
	break;
	case eSTATE::SKILL_MINE_TRAP:	// スキル・地雷トラップ
	{
		m_Motion *= m_Friction;

		if (m_StateTimer <= 0) {
			cAtkobjMgr::GetInstance().Create_MineTrap(this, m_Pos, VEC2F(cosf(m_Angle), sinf(m_Angle)) * 8.0f);
			m_State = eSTATE::MOVE;
		}
	}
	break;
	case eSTATE::SKILL_MENDING:	// スキル・修繕
	{
		m_Motion *= m_Friction;

		if (m_MendingNum > 0 && m_StateTimer == 10)
		{
			m_HP = __min(m_HP + 1, MaxHP);
			--m_MendingNum;
			m_StateTimer = 30;
			cSoundData::GetInstance().Play(eSOUND::SKILL_MENDING);
		}

		if (m_StateTimer <= 0)
		{
			m_State = eSTATE::MOVE;
			m_SkillNeedTime = 0;
		}
	}
	break;
	case eSTATE::SKILL_FULL_BURST:	// スキル・フルバースト
	{
		m_Motion *= m_Friction;
		if (m_StateTimer == 23)
			cSoundData::GetInstance().Play(eSOUND::SKILL_FULL_BURST);
		if (m_StateTimer <= 0) {
			if (m_BulletNum > 0) {
				{
					int adversity = (Ability == eABILITY::ADVERSITY && m_HP <= m_AdversityLine) * m_AdversityValue;
					for (int i = 1; i <= 3; i += 2) {
						float angle = m_Angle + (GetRand(2000) - 1000) * (m_FullburstValue + 6) * 0.00002f * i;
						VEC2F shot_pos = m_Pos + VEC2F(cosf(angle), sinf(angle)) * m_MuzzleLength;
						if (Ability == eABILITY::DEATH && (m_DeathBulletTimes = __max(m_DeathBulletTimes - 1, 0)) == 0) {
							cAtkobjMgr::GetInstance().Create_DeathBullet(this, shot_pos, angle, m_RicochetTimes);
							m_DeathBulletTimes = GetRand(m_MaxDeathBulletTimes);
						}
						else {
							switch (Bullet) {
							break;	case eBULLET::NORMAL:	cAtkobjMgr::GetInstance().Create_NormalBullet(this, AtkDamage + adversity, shot_pos, angle, m_RicochetTimes);
							break;	case eBULLET::CHARGE:	cAtkobjMgr::GetInstance().Create_ChargeBullet(this, AtkDamage + adversity, shot_pos, angle, m_RicochetTimes, GetRand(2));
							break;	case eBULLET::LASER:	cAtkobjMgr::GetInstance().Create_Laser(this, AtkDamage + adversity, shot_pos, angle, m_RicochetTimes);
							break;	case eBULLET::MISSILE:	cAtkobjMgr::GetInstance().Create_Misslie(this, AtkDamage + adversity, shot_pos, angle, m_RicochetTimes);
							}
						}
					}

				}
				m_ShotCoolTime = MaxShotCoolTime;
				--m_BulletNum;
				m_ReloadTime = m_MaxReloadTime;
				m_StateTimer = 3;
				++m_FullburstValue;
			}
			else
				m_State = eSTATE::MOVE;
		}
	}
	break;
	case eSTATE::SKILL_QUICK_RELOAD:	// スキル・クイックリロード
	{
		m_Motion *= m_Friction;

		if (m_StateTimer == 31) {
			cSoundData::GetInstance().Play(eSOUND::SKILL_QUICK_RELOAD_1);
		}
		if (m_StateTimer == 6) {
			m_BulletNum = MaxBulletNum;
			cSoundData::GetInstance().Play(eSOUND::SKILL_QUICK_RELOAD_2);
		}

		if (m_StateTimer <= 0)
			m_State = eSTATE::MOVE;
	}
	break;
	}

	// ランダム
	if (m_RandomTime > 0 || m_RandomInterval > 0) {
		if (m_State != eSTATE::REPAIR && m_State != eSTATE::DEAD) {
			if (m_RandomTime > 0)
				--m_RandomTime;
			if (--m_RandomInterval <= 0)
				if (m_RandomTime > 0) {
					m_RandomInterval = 2 + (m_MaxRandomTime - m_RandomTime) / 7;
					m_NowRandomSkill = (eSKILL)(((int)m_NowRandomSkill + 1) % (int)eSKILL::RANDOM);
					cSoundData::GetInstance().Play(eSOUND::CURSOR);
				}
				else {
					m_RunRandomSkillTimer = 18;
					m_SkillProcess(m_NowRandomSkill);
					cSoundData::GetInstance().Play(eSOUND::RUN_SKILL);
				}
		}
		else {
			m_RandomTime = 0;
			m_RandomInterval = 0;
		}
	}

	// 風圧
	if (m_State != eSTATE::DEAD)
		cAtkobjMgr::GetInstance().Create_Wind(this, m_Pos + m_Motion, Radius * 2.0f, 4.0f, 1);

	// 電気トラップ
	if (m_ElectricTrapTime > 0) {
		if (m_State != eSTATE::REPAIR && m_State != eSTATE::DEAD) {
			if (m_ElectricTrapTime % 12 == 0)
				cAtkobjMgr::GetInstance().Create_ElectricTrap(this, m_Pos);
		}
		else {
			m_ElectricTrapTime = 0;
		}
	}

	// リベンジャーオーブの角度
	m_OrbAngle += m_OrbRotation;
	while (m_OrbAngle >= DX_PI_F)
		m_OrbAngle -= DX_PI_F * 2.0f;

	// 逆境のエフェクト
	if (m_State != eSTATE::REPAIR && m_State != eSTATE::DEAD && Ability == eABILITY::ADVERSITY && m_HP < m_AdversityLine)
		cEffectMgr::GetInstance().Create_Aura(m_Pos, 1.2f, DX_PI_F / 180.0f * GetRand(359), GetRand(100) / 100.0f, 0x70181c);

	// バーサーカーのエフェクト
	if (m_State != eSTATE::DEAD) {
		switch (m_BerserkerValue) {
		break;	case -2:	cEffectMgr::GetInstance().Create_Aura(m_Pos, 1.8f, DX_PI_F / 180.0f * GetRand(359), GetRand(100) / 60.0f, 0x113388);
		break;	case -1:	cEffectMgr::GetInstance().Create_Aura(m_Pos, 1.4f, DX_PI_F / 180.0f * GetRand(359), GetRand(100) / 100.0f, 0x102077);
		break;	case 1:		cEffectMgr::GetInstance().Create_Aura(m_Pos, 1.4f, DX_PI_F / 180.0f * GetRand(359), GetRand(100) / 100.0f, 0x772010);
		break;	case 2:		cEffectMgr::GetInstance().Create_Aura(m_Pos, 1.8f, DX_PI_F / 180.0f * GetRand(359), GetRand(100) / 60.0f, 0x883311);
		}
	}

	// アニメーション
	if (m_State != eSTATE::DEAD) {
		(inputX || inputY) ?
			++m_AnimationTimer %= (m_MaxFrameTime * 4) :
			m_AnimationTimer = 0;
	}

	// 移動処理
	m_MoveProcess();
}

/* 描画・DEFAULT */
void cPlayer::Draw_Default() const
{
	// 本体
	if (m_State != eSTATE::DEAD) {
		float angle = m_Angle + DX_PI_F / 8.0f;
		while (angle < 0.0f)			angle += DX_PI_F * 2.0f;
		while (angle >= DX_PI_F * 2.0f) angle -= DX_PI_F * 2.0f;
		int rotate = (int)(angle / (DX_PI_F / 4.0f));
		int frame = 1;
		switch (m_AnimationTimer / m_MaxFrameTime) {
		break;	case 0:	frame = 1;
		break;	case 1:	frame = 0;
		break;	case 2:	frame = 1;
		break;	case 3:	frame = 2;
		}
		if (m_State == eSTATE::REPAIR)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		if (m_DamageTimer > 0) {
			int param = m_DamageTimer * 200 / m_MaxDamageTime;
			SetDrawAddColor(param, -param, -param);
		}
		DrawRotaGraph(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y + m_DispOffset,
			m_DispScale, 0.0, m_Image_Player[rotate * 3 + frame], TRUE);
		SetDrawAddColor(0, 0, 0);
		switch (PlayerID) {
		break;	case 0:		SetDrawAddColor(255, 0, 0);
		break;	case 1:		SetDrawAddColor(0, 0, 255);
		break;	case 2:		SetDrawAddColor(0, 255, 0);
		break;	case 3:		SetDrawAddColor(255, 255, 0);
		break;	default:	SetDrawAddColor(200, 200, 200);
		}
		DrawRotaGraph(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y + m_DispOffset,
			m_DispScale, 0.0, m_Image_PlayerColor[rotate * 3 + frame], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawAddColor(0, 0, 0);
	}

	// チャージショットのゲージ
	if (m_State == eSTATE::CHARGE && m_ChargeTime > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, __min(m_ChargeTime * 4, 200));
		SetDrawBright(255, 255, 64);
		float param = Radius - 1.0f + (m_ChargeTime_Second - m_ChargeTime) * 30 / m_ChargeTime_Second;
		DrawCircleGauge(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y,
			m_ChargeTime * 100.0 / m_ChargeTime_Second, m_Image_ChargeRing);
		DrawCircle(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y,
			(int)param, 0xffffff, FALSE, 2);
		DrawRotaGraph(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y,
			param / 25.0,
			0.0, cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::RING_IN), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawBright(255, 255, 255);
	}

	// チャージ完了エフェクト
	if (m_ChargeEffectTime > 0) {
		float param = Radius + Easing::Quad::Out((float)(m_MaxChargeEffectTime - m_ChargeEffectTime) / m_MaxChargeEffectTime) * 30.0f;
		SetDrawBlendMode(DX_BLENDMODE_ADD, m_ChargeEffectTime * 200 / m_MaxChargeEffectTime);
		SetDrawBright(255, 255, 64);
		DrawCircle(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y,
			(int)param, 0xffffff, FALSE, 1 + m_ChargeEffectTime * 3 / m_MaxChargeEffectTime);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawBright(255, 255, 255);
	}

	// スキル発動のゲージ
	if (Skill != eSKILL::MAX && m_SkillNeedTime > 0) {
		DrawCircleGauge(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y,
			m_SkillNeedTime * 100.0 / m_MaxSkillNeedTime[(int)Skill], m_Image_SkillTimeRing);
	}

	// シールド展開エフェクト
	if (m_ShieldTime > 0) {
		float param = Radius * 1.5f + Easing::Quad::Out((float)(m_MaxShieldTime - m_ShieldTime) / m_MaxShieldTime) * 20.0f;
		SetDrawBlendMode(DX_BLENDMODE_ADD, m_ShieldTime * 255 / m_MaxShieldTime);
		DrawRotaGraph(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y,
			param / 25.0,
			m_Angle, cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::SHIELD), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// ランダムスキル
	if (m_RandomTime > 0 || m_RandomInterval > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
		DrawRotaGraph(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y - 48,
			2.0, 0.0, cCommonData::GetIcon_Skill(m_NowRandomSkill), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (m_RunRandomSkillTimer > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, m_RunRandomSkillTimer * 255 / 18);
		DrawRotaGraph(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y - 48,
			2.0 + ((double)18 - (double)m_RunRandomSkillTimer) / 10.0, 0.0, cCommonData::GetIcon_Skill(m_NowRandomSkill), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

/* 描画・FADE_ADD */
void cPlayer::Draw_FadeAdd() const
{
	if (m_State == eSTATE::SKILL_DASH && m_StateTimer <= 12)
	{
		float angle = m_Angle + DX_PI_F / 8.0f;
		while (angle < 0.0f)			angle += DX_PI_F * 2.0f;
		while (angle >= DX_PI_F * 2.0f) angle -= DX_PI_F * 2.0f;
		int rotate = (int)(angle / (DX_PI_F / 4.0f));
		int frame = 1;
		switch (m_AnimationTimer / m_MaxFrameTime) {
		break;	case 0:	frame = 1;
		break;	case 1:	frame = 0;
		break;	case 2:	frame = 1;
		break;	case 3:	frame = 2;
		}
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		switch (PlayerID) {
		break;	case 0:		SetDrawAddColor(255, 50, 50);
		break;	case 1:		SetDrawAddColor(50, 50, 255);
		break;	case 2:		SetDrawAddColor(50, 255, 50);
		break;	case 3:		SetDrawAddColor(255, 255, 50);
		break;	default:	SetDrawAddColor(210, 210, 210);
		}
		DrawRotaGraph(
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y + m_DispOffset,
			m_DispScale, 0.0, m_Image_PlayerColor[rotate * 3 + frame], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawAddColor(0, 0, 0);
	}
}

/* ショット判定 */
bool cPlayer::CheckShot() const
{
	return m_ShotFlag;
}

/* スキルの発動判定 */
bool cPlayer::CheckRunSkill() const
{
	return m_RunSkillFlag;
}

/* 復活判定 */
bool cPlayer::IsRepair() const
{
	return m_State == eSTATE::REPAIR;
}

/* 死亡判定 */
bool cPlayer::IsDead() const
{
	return m_State == eSTATE::DEAD;
}

/* シールド展開判定 */
bool cPlayer::IsShield() const
{
	return m_ShieldTime > 0;
}

/* 座標を設定 */
void cPlayer::SetPos(VEC2F Pos)
{
	m_Pos = Pos;

	float angle = atan2f(cField::GetCenterPos().y - m_Pos.y, cField::GetCenterPos().x - m_Pos.x);
	if (cosf(angle) > 0.5f)			m_Angle = 0.0f;
	else if (cosf(angle) < -0.5f)	m_Angle = DX_PI_F;
	else if (sinf(angle) > 0.0f)	m_Angle = DX_PI_F / 2.0f;
	else							m_Angle = -DX_PI_F / 2.0f;
}

/* 移動量を加算 */
void cPlayer::AddMotion(VEC2F Motion)
{
	m_Motion += Motion;
}

/* 麻痺状態にする */
void cPlayer::SetPalsyTime(int Time)
{
	m_PalsyTime = __max(m_PalsyTime, Time);
}

/* ダメージを受けたときの処理 */
void cPlayer::TakenDamageProcess(int Owner, int Damage)
{
	if (m_State == eSTATE::REPAIR || m_State == eSTATE::DEAD)
		return;

	unsigned int color = 0xffffff;
	switch (PlayerID) {
	case 0:	color = 0xff4040;	break;
	case 1:	color = 0x4040ff;	break;
	case 2:	color = 0x40ff40;	break;
	case 3:	color = 0xffff40;	break;
	}

	if (Damage != 0)
	{
		// 加護
		if (Damage > 0) {
			if (Ability == eABILITY::BLESSING && GetRand(5) == 0) {
				cSoundData::GetInstance().Play(eSOUND::BLESSING);
				return;
			}
		}

		// HP処理
		int hp_prev = m_HP;
		if (Damage > 0) {
			m_HP -= Damage;
			if (Ability == eABILITY::BERSERKER)
				m_BerserkerValue = __max(m_BerserkerValue - 1, -2);
			if (m_State == eSTATE::MOVE)
				m_SkillNeedTime = 0;
			cSoundData::GetInstance().Play(eSOUND::DAMAGE);
		}
		else if (Damage == -1) {
			(m_HP > 1) ?
				m_HP = 1 :
				m_HP = 0;
			cSoundData::GetInstance().Play(eSOUND::DAMAGE);
			cSoundData::GetInstance().Play(eSOUND::DEATH);
			cEffectMgr::GetInstance().Create_DeathBullet_Hit(m_Pos);
		}

		// オーブの生成
		if (Ability == eABILITY::REVENGER_ORB && m_HP > 0) {
			int orb_num = 1 + (hp_prev - m_HP) / 3;
			for (int i = 0; i < orb_num && m_OrbNum < MaxOrbNum; ++i)
				cAtkobjMgr::GetInstance().Create_Orb(this, 1, m_OrbNum++);
		}

		// ダメージエフェクト
		m_DamageTimer = m_MaxDamageTime;
		for (int i = 0; i < (hp_prev - m_HP) * 6; ++i)
			cEffectMgr::GetInstance().Create_Line(m_Pos, DX_PI_F / 180.0f * GetRand(359), GetRand(120) * 0.1f, color);

		// パッドの振動
		if (cCommonData::InputDevice[PlayerID] != eINPUT_DEVICE::KEY_MOUSE)
			StartJoypadVibration((int)cCommonData::InputDevice[PlayerID] + 1, 1000, 200);
	}

	// HPが0になったときの処理
	if (m_HP <= 0) {
		m_HP = 0;
		m_StateTimer = 15;

		// 残基を減らす
		if (--m_CoreNum > 0) {
			m_State = eSTATE::REPAIR;

			// 演出
			for (int i = 0; i < 20; ++i)
				cEffectMgr::GetInstance().Create_Confetti(m_Pos, DX_PI_F / 180.0f * GetRand(359), GetRand(160) * 0.1f, color);
			for (int i = 0; i < 20; ++i)
				cEffectMgr::GetInstance().Create_Line(m_Pos, DX_PI_F / 180.0f * GetRand(359), GetRand(200) * 0.1f, color);
		}

		// 死亡する
		else {
			m_State = eSTATE::DEAD;
			for (int i = 1 + cCommonData::Player4Flag * 2; i >= 0; --i) {
				if (cCommonData::Ranking[i] == -1) {
					cCommonData::Ranking[i] = PlayerID;
					break;
				}
			}

			// 演出
			for (int i = 0; i < 40; ++i)
				cEffectMgr::GetInstance().Create_Confetti(m_Pos, DX_PI_F / 180.0f * GetRand(359), GetRand(200) * 0.1f, color);
			for (int i = 0; i < 40; ++i)
				cEffectMgr::GetInstance().Create_Line(m_Pos, DX_PI_F / 180.0f * GetRand(359), GetRand(240) * 0.1f, color);
			cEffectMgr::GetInstance().Create_Dead(m_Pos, color);
			cSoundData::GetInstance().Play(eSOUND::DESTROY);
		}
	}
}

/* ダメージを与えたときの処理 */
void cPlayer::DealtDamageProcess(int Target)
{
	if (Ability == eABILITY::BERSERKER)
		m_BerserkerValue = __min(m_BerserkerValue + 1, 2);
}

/* リソースの読み込み */
void cPlayer::LoadResource()
{
	if (m_LoadedFlag)
		return;

	m_LoadedFlag = true;

	m_Image_ChargeRing = LoadGraph("data\\image\\game\\charge_ring.png");
	m_Image_SkillTimeRing = LoadGraph("data\\image\\game\\skill_ring.png");

	LoadDivGraph("data\\image\\player\\player.png", 3 * 8, 3, 8, 24, 24, m_Image_Player);
	LoadDivGraph("data\\image\\player\\player_color.png", 3 * 8, 3, 8, 24, 24, m_Image_PlayerColor);
}

/* リソースの解放 */
void cPlayer::DeleteResource()
{
	if (!m_LoadedFlag)
		return;

	m_LoadedFlag = false;

	DeleteGraph(m_Image_ChargeRing);	m_Image_ChargeRing = -1;
	DeleteGraph(m_Image_SkillTimeRing);	m_Image_SkillTimeRing = -1;
	for (int i = 0; i < 3 * 8; ++i) {
		DeleteGraph(m_Image_Player[i]);			m_Image_Player[i] = -1;
		DeleteGraph(m_Image_PlayerColor[i]);	m_Image_PlayerColor[i] = -1;
	}
}

/* 描画順の比較関数 */
bool cPlayer::Comp(const cPlayer* const lhs, const cPlayer* const rhs)
{
	return lhs->m_Pos.y < rhs->m_Pos.y;
}

////////////////////
//   非公開関数   //
////////////////////

/* 移動処理 */
void cPlayer::m_MoveProcess()
{
	// 角・左下
	if (m_Motion.x > 0.0f || m_Motion.y < 0.0f)
		for (const auto& i : cField::CornerLD) {
			float distance = sqrtf(powf(m_Pos.x - (float)cField::TileSize * i.x, 2.0f) + powf(m_Pos.y - (float)cField::TileSize * i.y, 2.0f));
			if (distance <= Radius) {
				float angle = atan2f((m_Pos.y + m_Motion.y) - (float)cField::TileSize * i.y, (m_Pos.x + m_Motion.x) - (float)cField::TileSize * i.x);
				float ax = -1.0f;
				float ay = 1.0f;
				if (sinf(angle) > 0.0f)
					ax = cosf(angle);
				if (cosf(angle) < 0.0f)
					ay = sinf(angle);
				m_Motion.x = (float)cField::TileSize * i.x + ax * Radius - m_Pos.x;
				m_Motion.y = (float)cField::TileSize * i.y + ay * Radius - m_Pos.y;
			}
		}

	// 角・左上
	if (m_Motion.x > 0.0f || m_Motion.y > 0.0f)
		for (const auto& i : cField::CornerLU) {
			float distance = sqrtf(powf(m_Pos.x - (float)cField::TileSize * i.x, 2.0f) + powf(m_Pos.y - (float)cField::TileSize * i.y, 2.0f));
			if (distance <= Radius) {
				float angle = atan2f((m_Pos.y + m_Motion.y) - (float)cField::TileSize * i.y, (m_Pos.x + m_Motion.x) - (float)cField::TileSize * i.x);
				float ax = -1.0f;
				float ay = -1.0f;
				if (sinf(angle) < 0.0f)
					ax = cosf(angle);
				if (cosf(angle) < 0.0f)
					ay = sinf(angle);
				m_Motion.x = (float)cField::TileSize * i.x + ax * Radius - m_Pos.x;
				m_Motion.y = (float)cField::TileSize * i.y + ay * Radius - m_Pos.y;
			}
		}

	// 角・右上
	if (m_Motion.x < 0.0f || m_Motion.y > 0.0f)
		for (const auto& i : cField::CornerRU) {
			float distance = sqrtf(powf(m_Pos.x - (float)cField::TileSize * i.x, 2.0f) + powf(m_Pos.y - (float)cField::TileSize * i.y, 2.0f));
			if (distance <= Radius) {
				float angle = atan2f((m_Pos.y + m_Motion.y) - (float)cField::TileSize * i.y, (m_Pos.x + m_Motion.x) - (float)cField::TileSize * i.x);
				float ax = 1.0f;
				float ay = -1.0f;
				if (sinf(angle) < 0.0f)
					ax = cosf(angle);
				if (cosf(angle) > 0.0f)
					ay = sinf(angle);
				m_Motion.x = (float)cField::TileSize * i.x + ax * Radius - m_Pos.x;
				m_Motion.y = (float)cField::TileSize * i.y + ay * Radius - m_Pos.y;
			}
		}

	// 角・右下
	if (m_Motion.x < 0.0f || m_Motion.y < 0.0f)
		for (const auto& i : cField::CornerRD) {
			float distance = sqrtf(powf(m_Pos.x - (float)cField::TileSize * i.x, 2.0f) + powf(m_Pos.y - (float)cField::TileSize * i.y, 2.0f));
			if (distance <= Radius) {
				float angle = atan2f((m_Pos.y + m_Motion.y) - (float)cField::TileSize * i.y, (m_Pos.x + m_Motion.x) - (float)cField::TileSize * i.x);
				float ax = 1.0f;
				float ay = 1.0f;
				if (sinf(angle) > 0.0f)
					ax = cosf(angle);
				if (cosf(angle) > 0.0f)
					ay = sinf(angle);
				m_Motion.x = (float)cField::TileSize * i.x + ax * Radius - m_Pos.x;
				m_Motion.y = (float)cField::TileSize * i.y + ay * Radius - m_Pos.y;
			}
		}

	// 線・上
	if (m_Motion.y > 0.0f)
		for (const auto& i : cField::LineU)
			if (m_Pos.x >= (float)cField::TileSize * i.first.x && m_Pos.x <= (float)cField::TileSize * (i.first.x + i.second))
				if (m_Pos.y + Radius <= (float)cField::TileSize * i.first.y && m_Pos.y + Radius + m_Motion.y > (float)cField::TileSize * i.first.y)
					m_Motion.y = (float)cField::TileSize * i.first.y - (m_Pos.y + Radius);

	// 線・下
	if (m_Motion.y < 0.0f)
		for (const auto& i : cField::LineD)
			if (m_Pos.x >= (float)cField::TileSize * i.first.x && m_Pos.x <= (float)cField::TileSize * (i.first.x + i.second))
				if (m_Pos.y - Radius >= (float)cField::TileSize * i.first.y && m_Pos.y - Radius + m_Motion.y < (float)cField::TileSize * i.first.y)
					m_Motion.y = (float)cField::TileSize * i.first.y - (m_Pos.y - Radius);

	// 線・左
	if (m_Motion.x > 0.0f)
		for (const auto& i : cField::LineL)
			if (m_Pos.y >= (float)cField::TileSize * i.first.y && m_Pos.y <= (float)cField::TileSize * (i.first.y + i.second))
				if (m_Pos.x + Radius <= (float)cField::TileSize * i.first.x && m_Pos.x + Radius + m_Motion.x > (float)cField::TileSize * i.first.x)
					m_Motion.x = (float)cField::TileSize * i.first.x - (m_Pos.x + Radius);

	// 線・右
	if (m_Motion.x < 0.0f)
		for (const auto& i : cField::LineR)
			if (m_Pos.y >= (float)cField::TileSize * i.first.y && m_Pos.y <= (float)cField::TileSize * (i.first.y + i.second))
				if (m_Pos.x - Radius >= (float)cField::TileSize * i.first.x && m_Pos.x - Radius + m_Motion.x < (float)cField::TileSize * i.first.x)
					m_Motion.x = (float)cField::TileSize * i.first.x - (m_Pos.x - Radius);

	// 移動
	m_Pos += m_Motion;
}

/* スキル発動 */
void cPlayer::m_RunSkill()
{
	m_SkillProcess(Skill);
	m_RunSkillFlag = true;
	m_SkillCoolTime = MaxSkillCoolTime;
}

/* スキルの内容を実行 */
void cPlayer::m_SkillProcess(eSKILL skill)
{
	switch (skill)
	{
	case eSKILL::DASH:
		m_StateTimer = 24;
		m_State = eSTATE::SKILL_DASH;
		break;
	case eSKILL::SHIELD:
		m_StateTimer = 18;
		m_ShieldTime = m_MaxShieldTime;
		m_State = eSTATE::SKILL_SHIELD;
		break;
	case eSKILL::ELECTRIC_TRAP:
		m_StateTimer = 30;
		m_State = eSTATE::SKILL_ELECTRIC_TRAP;
		break;
	case eSKILL::MINE_TRAP:
		m_StateTimer = 30;
		m_State = eSTATE::SKILL_MINE_TRAP;
		break;
	case eSKILL::MENDING:
		m_MendingNum = m_MaxMendingNum;
		m_StateTimer = 30;
		m_State = eSTATE::SKILL_MENDING;
		break;
	case eSKILL::FULL_BURST:
		m_StateTimer = 24;
		m_BulletNum = MaxBulletNum;
		m_FullburstValue = 0;
		m_State = eSTATE::SKILL_FULL_BURST;
		break;
	case eSKILL::QUICK_RELOAD:
		m_StateTimer = 32;
		m_State = eSTATE::SKILL_QUICK_RELOAD;
		break;
	case eSKILL::RANDOM:
		m_RandomTime = m_MaxRandomTime;
		m_NowRandomSkill = (eSKILL)GetRand((int)eSKILL::RANDOM - 1);
		break;
	}
}
