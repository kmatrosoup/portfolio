//////////////////////
//   プリプロセス   //
//////////////////////
#include "DeathBullet.h"
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
const float cDeathBullet::m_Radius(5.0f);	// 半径
const float cDeathBullet::m_Speed(6.0f);	// 移動速度

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cDeathBullet::cDeathBullet(cPlayer* Owner, VEC2F Pos, float Angle, int Ricochet)
	: cBaseAtkobj(Owner)
	, m_HitFlag(false)
	, m_Pos(Pos)
	, m_Pos_prev(m_Pos)
	, m_Angle(Angle)
	, m_Ricochet(Ricochet)
{
	cGame::ShakeEffect(10);
	cEffectMgr::GetInstance().Create_MuzzleFlash(m_Pos, 1.2f, m_Angle);
	cSoundData::GetInstance().Play(eSOUND::BULLET_NORMAL);
}

/* デストラクタ */
cDeathBullet::~cDeathBullet()
{
}

/* 更新 */
void cDeathBullet::Update()
{
	m_Pos_prev = m_Pos;

	// 移動処理
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
			nearest_player->TakenDamageProcess(m_Owner->PlayerID, -1);
			m_Owner->DealtDamageProcess(nearest_player->PlayerID);
			cGame::ShakeEffect(30);
		}
	}
}

/* 描画・ADD */
void cDeathBullet::Draw_FadeAdd() const
{
	unsigned int color = 0xffffff;
	switch (m_Owner->PlayerID) {
	break;	case 0:	color = 0xff8080;
	break;	case 1:	color = 0x8080ff;
	break;	case 2:	color = 0x80ff80;
	break;	case 3:	color = 0xffff80;
	}

	float offsetX = (GetRand(1000) - 500) / 100.0f;
	float offsetY = (GetRand(1000) - 500) / 100.0f;
	float angle = DX_PI_F * 2.0f * GetRand(999) / 1000.0f;
	float size = 5.0f + GetRand(500) / 100.0f;

	DrawQuadrangle(
		cGame::ScreenDif.x + (int)(m_Pos.x + offsetX + cosf(DX_PI_F / 2.0f * 0.0f + angle) * size),
		cGame::ScreenDif.y + (int)(m_Pos.y + offsetY + sinf(DX_PI_F / 2.0f * 0.0f + angle) * size),
		cGame::ScreenDif.x + (int)(m_Pos.x + offsetX + cosf(DX_PI_F / 2.0f * 1.0f + angle) * size),
		cGame::ScreenDif.y + (int)(m_Pos.y + offsetY + sinf(DX_PI_F / 2.0f * 1.0f + angle) * size),
		cGame::ScreenDif.x + (int)(m_Pos.x + offsetX + cosf(DX_PI_F / 2.0f * 2.0f + angle) * size),
		cGame::ScreenDif.y + (int)(m_Pos.y + offsetY + sinf(DX_PI_F / 2.0f * 2.0f + angle) * size),
		cGame::ScreenDif.x + (int)(m_Pos.x + offsetX + cosf(DX_PI_F / 2.0f * 3.0f + angle) * size),
		cGame::ScreenDif.y + (int)(m_Pos.y + offsetY + sinf(DX_PI_F / 2.0f * 3.0f + angle) * size),
		color, TRUE);
}

/* アクティブ状態 */
bool cDeathBullet::Active() const
{
	return !m_HitFlag && m_Ricochet >= 0;
}
