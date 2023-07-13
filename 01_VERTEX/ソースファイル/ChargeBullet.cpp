//////////////////////
//   プリプロセス   //
//////////////////////
#include "ChargeBullet.h"
#include "DxLib.h"
#include "Player.h"
#include "Field.h"
#include "Game.h"
#include "SoundData.h"
#include "EffectMgr.h"
#include <cmath>

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const float cChargeBullet::m_Radius_0(3.0f);		// 半径・溜め0
const float cChargeBullet::m_Radius_1(6.0f);		// 半径・溜め1
const float cChargeBullet::m_Radius_2(9.0f);		// 半径・溜め2
const float cChargeBullet::m_Speed_Start_0(10.0f);	// 初速・溜め0
const float cChargeBullet::m_Speed_Start_1(14.0f);	// 初速・溜め1
const float cChargeBullet::m_Speed_Start_2(18.0f);	// 初速・溜め2
const float cChargeBullet::m_Speed_End(5.0f);		// 最低速度

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cChargeBullet::cChargeBullet(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet, int ChargePower)
	: cBaseAtkobj(Owner)
	, m_HitFlag(false)
	, m_AtkDamage(AtkDamage)
	, m_Pos(Pos)
	, m_Pos_prev(m_Pos)
	, m_Radius(ChargePower != 2 ? ChargePower != 1 ? m_Radius_0 : m_Radius_1 : m_Radius_2)
	, m_Speed(ChargePower != 2 ? ChargePower != 1 ? m_Speed_Start_0 : m_Speed_Start_1 : m_Speed_Start_2)
	, m_Angle(Angle)
	, m_Ricochet(Ricochet)
	, m_ChargePower(ChargePower)
{
	switch (m_ChargePower) {
	case 0:
		cSoundData::GetInstance().Play(eSOUND::BULLET_CHARGE_0);
		cGame::ShakeEffect(7 + m_AtkDamage * 3);
		cEffectMgr::GetInstance().Create_MuzzleFlash(m_Pos, 1.0f + m_AtkDamage * 0.1f, m_Angle);
		break;
	case 1:
		cSoundData::GetInstance().Play(eSOUND::BULLET_CHARGE_1);
		cGame::ShakeEffect(7 + m_AtkDamage * 4);
		cEffectMgr::GetInstance().Create_MuzzleFlash(m_Pos, 1.0f + m_AtkDamage * 0.15f, m_Angle);
		break;
	case 2:
		cSoundData::GetInstance().Play(eSOUND::BULLET_CHARGE_2);
		cGame::ShakeEffect(7 + m_AtkDamage * 5);
		cEffectMgr::GetInstance().Create_MuzzleFlash(m_Pos, 1.0f + m_AtkDamage * 0.2f, m_Angle);
		break;
	}
}

/* デストラクタ */
cChargeBullet::~cChargeBullet()
{
}

/* 更新 */
void cChargeBullet::Update()
{
	m_Pos_prev = m_Pos;

	// 移動処理
	m_Speed += (m_Speed_End - m_Speed) * 0.1f;
	m_Pos.x += cosf(m_Angle) * m_Speed;
	m_Pos.y += sinf(m_Angle) * m_Speed;

	{
		bool ricochet_flag = false;
		if (sinf(m_Angle) > 0.0f) {
			for (const auto& i : cField::LineU) {
				float param = (i.first.y * cField::TileSize - m_Pos_prev.y) / (m_Pos.y - m_Pos_prev.y);
				if (param >= 0.0f && param < 1.0f) {
					float tmpX = m_Pos_prev.x + (m_Pos.x - m_Pos_prev.x) * param;
					if (tmpX >= (float)(i.first.x * cField::TileSize) && tmpX <= (float)((i.first.x + i.second) * cField::TileSize)) {
						m_Angle = DX_PI_F * 2 - m_Angle;
						m_Pos = m_Pos_prev + (m_Pos - m_Pos_prev) * param;
						ricochet_flag = true;
						break;
					}
				}
			}
		}
		else if (sinf(m_Angle) < 0.0f) {
			for (const auto& i : cField::LineD) {
				float param = (i.first.y * cField::TileSize - m_Pos_prev.y) / (m_Pos.y - m_Pos_prev.y);
				if (param >= 0.0f && param < 1.0f) {
					float tmpX = m_Pos_prev.x + (m_Pos.x - m_Pos_prev.x) * param;
					if (tmpX >= (float)(i.first.x * cField::TileSize) && tmpX <= (float)((i.first.x + i.second) * cField::TileSize)) {
						m_Angle = DX_PI_F * 2 - m_Angle;
						m_Pos = m_Pos_prev + (m_Pos - m_Pos_prev) * param;
						ricochet_flag = true;
						break;
					}
				}
			}
		}
		if (cosf(m_Angle) > 0.0f) {
			for (const auto& i : cField::LineL) {
				float param = (i.first.x * cField::TileSize - m_Pos_prev.x) / (m_Pos.x - m_Pos_prev.x);
				if (param >= 0.0f && param < 1.0f) {
					float tmpY = m_Pos_prev.y + (m_Pos.y - m_Pos_prev.y) * param;
					if (tmpY >= (float)(i.first.y * cField::TileSize) && tmpY <= (float)((i.first.y + i.second) * cField::TileSize)) {
						m_Angle = DX_PI_F - m_Angle;
						m_Pos = m_Pos_prev + (m_Pos - m_Pos_prev) * param;
						ricochet_flag = true;
						break;
					}
				}
			}
		}
		else if (cosf(m_Angle) < 0.0f) {
			for (const auto& i : cField::LineR) {
				float param = (i.first.x * cField::TileSize - m_Pos_prev.x) / (m_Pos.x - m_Pos_prev.x);
				if (param >= 0.0f && param < 1.0f) {
					float tmpY = m_Pos_prev.y + (m_Pos.y - m_Pos_prev.y) * param;
					if (tmpY >= (float)(i.first.y * cField::TileSize) && tmpY <= (float)((i.first.y + i.second) * cField::TileSize)) {
						m_Angle = DX_PI_F - m_Angle;
						m_Pos = m_Pos_prev + (m_Pos - m_Pos_prev) * param;
						ricochet_flag = true;
						break;
					}
				}
			}
		}
		if (ricochet_flag)
			if (--m_Ricochet >= 0)
				cSoundData::GetInstance().Play(eSOUND::RICOCHET);
			else {
				m_HitFlag = true;
				return;
			}
	}
	if (cField::InWall(m_Pos)) {
		m_HitFlag = true;
		return;
	}

	// 衝突判定
	if (m_HitFlag)
		return;

	cPlayer* nearest_player = nullptr;
	float nearest_length;
	for (auto& i : cGame::pVecPlayer)
	{
		if (i->PlayerID == m_Owner->PlayerID)
			continue;
		if (i->IsRepair())
			continue;
		if (i->IsDead())
			continue;

		if (VEC2F::CheckIntersect_lineCircle(m_Pos_prev, m_Pos, i->Pos, i->Radius + m_Radius)) {
			m_HitFlag = true;
			if (!nearest_player) {
				nearest_length = sqrtf((i->Pos.x - m_Pos.x) * (i->Pos.x - m_Pos.x) + (i->Pos.y - m_Pos.y) * (i->Pos.y - m_Pos.y));
				nearest_player = i;
			}
			else {
				float length = sqrtf((i->Pos.x - m_Pos.x) * (i->Pos.x - m_Pos.x) + (i->Pos.y - m_Pos.y) * (i->Pos.y - m_Pos.y));
				if (nearest_length < length) {
					nearest_length = length;
					nearest_player = i;
				}
			}
		}
	}
	if (m_HitFlag) {
		if (nearest_player->IsShield() && cosf(nearest_player->Angle - m_Angle) < 0.0f) {
			m_HitFlag = false;
			m_Owner = nearest_player;
			m_Angle = nearest_player->Angle;
			cSoundData::GetInstance().Play(eSOUND::REFLECT);
		}
		else {
			nearest_player->TakenDamageProcess(m_Owner->PlayerID, m_AtkDamage);
			m_Owner->DealtDamageProcess(nearest_player->PlayerID);
		}
	}
}

/* 描画・DEFAULT */
void cChargeBullet::Draw_Default() const
{
	unsigned int color = 0xffffff;
	switch (m_Owner->PlayerID) {
	break;	case 0:	color = 0xff4040;
	break;	case 1:	color = 0x4040ff;
	break;	case 2:	color = 0x40ff40;
	break;	case 3:	color = 0xffff40;
	}

	DrawCircle(
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		(int)m_Radius, color, FALSE, 2);
}

/* 描画・ADD */
void cChargeBullet::Draw_FadeAdd() const
{
	unsigned int color = 0xffffff;
	switch (m_Owner->PlayerID) {
	break;	case 0:	color = 0xff8080;
	break;	case 1:	color = 0x8080ff;
	break;	case 2:	color = 0x80ff80;
	break;	case 3:	color = 0xffff80;
	}

	if (m_AtkDamage >= 1)
		DrawLine(
			cGame::ScreenDif.x + (int)m_Pos_prev.x,
			cGame::ScreenDif.y + (int)m_Pos_prev.y,
			cGame::ScreenDif.x + (int)m_Pos.x,
			cGame::ScreenDif.y + (int)m_Pos.y,
			color & 0x7f7f7f | 0x404040, m_AtkDamage * 2 - 1);
	DrawLine(
		cGame::ScreenDif.x + (int)m_Pos_prev.x + (int)(cosf(m_Angle - DX_PI_F / 2.0f) * (m_AtkDamage - 1)),
		cGame::ScreenDif.y + (int)m_Pos_prev.y + (int)(sinf(m_Angle - DX_PI_F / 2.0f) * (m_AtkDamage - 1)),
		cGame::ScreenDif.x + (int)m_Pos.x + (int)(cosf(m_Angle - DX_PI_F / 2.0f) * (m_AtkDamage - 1)),
		cGame::ScreenDif.y + (int)m_Pos.y + (int)(sinf(m_Angle - DX_PI_F / 2.0f) * (m_AtkDamage - 1)),
		color);
	DrawLine(
		cGame::ScreenDif.x + (int)m_Pos_prev.x + (int)(cosf(m_Angle + DX_PI_F / 2.0f) * (m_AtkDamage - 1)),
		cGame::ScreenDif.y + (int)m_Pos_prev.y + (int)(sinf(m_Angle + DX_PI_F / 2.0f) * (m_AtkDamage - 1)),
		cGame::ScreenDif.x + (int)m_Pos.x + (int)(cosf(m_Angle + DX_PI_F / 2.0f) * (m_AtkDamage - 1)),
		cGame::ScreenDif.y + (int)m_Pos.y + (int)(sinf(m_Angle + DX_PI_F / 2.0f) * (m_AtkDamage - 1)),
		color);
}

/* アクティブ状態 */
bool cChargeBullet::Active() const
{
	return !m_HitFlag && m_Ricochet >= 0;
}
