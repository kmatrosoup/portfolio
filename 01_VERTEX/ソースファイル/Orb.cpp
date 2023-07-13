//////////////////////
//   プリプロセス   //
//////////////////////
#include "Orb.h"
#include "DxLib.h"
#include "Player.h"
#include "Field.h"
#include "Game.h"
#include "SoundData.h"
#include <cmath>

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const float cOrb::m_Radius(5.0f);		// 半径
const float cOrb::m_Speed(6.0f);		// 移動速度
const float cOrb::m_Distance(30.0f);	// 所有者との距離
const int cOrb::m_MaxHomingTime(36);	// ホーミング最大時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cOrb::cOrb(cPlayer* Owner, int AtkDamage, int id)
	: cBaseAtkobj(Owner)
	, m_HitFlag(false)
	, m_AtkDamage(AtkDamage)
	, m_Pos(m_Owner->Pos)
	, m_Pos_prev(m_Pos)
	, m_Angle(0.0f)
	, m_FollowFlag(true)
	, m_FollowID(id)
	, m_HomingTimer(0)
{
}

/* デストラクタ */
cOrb::~cOrb()
{
}

/* 更新 */
void cOrb::Update()
{
	m_Pos_prev = m_Pos;

	// 移動処理
	if (m_FollowFlag) {
		if (m_Owner->CheckShot() || m_Owner->IsDead()) {
			m_FollowFlag = false;
		}
		else {
			m_Angle = m_Owner->OrbAngle + (DX_PI_F * 2.0f / m_Owner->OrbNum) * m_FollowID;
			m_Pos = m_Owner->Pos + VEC2F(cosf(m_Angle), sinf(m_Angle)) * m_Distance;
			return;
		}
	}

	if (m_HomingTimer < m_MaxHomingTime) {
		// 最寄りの他プレイヤーへ追尾
		cPlayer* t_player = cGame::GetNearestOtherPlayer(m_Owner);
		if (t_player)
		{
			float tmp_angle = atan2f(t_player->Pos.y - m_Pos.y, t_player->Pos.x - m_Pos.x) - m_Angle;
			if (sinf(tmp_angle) < 0.0f)	m_Angle -= 0.12f;
			if (sinf(tmp_angle) > 0.0f)	m_Angle += 0.12f;
		}
		++m_HomingTimer;
	}
	m_Pos.x += cosf(m_Angle) * m_Speed;
	m_Pos.y += sinf(m_Angle) * m_Speed;
	if (sinf(m_Angle) > 0.0f) {
		for (const auto& i : cField::LineU) {
			float param = (i.first.y * cField::TileSize - m_Pos_prev.y) / (m_Pos.y - m_Pos_prev.y);
			if (param >= 0.0f && param < 1.0f) {
				float tmpX = m_Pos_prev.x + (m_Pos.x - m_Pos_prev.x) * param;
				if (tmpX >= (float)(i.first.x * cField::TileSize) && tmpX <= (float)((i.first.x + i.second) * cField::TileSize)) {
					m_Angle = DX_PI_F * 2 - m_Angle;
					m_Pos = m_Pos_prev + (m_Pos - m_Pos_prev) * param;
					m_HitFlag = true;
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
					m_HitFlag = true;
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
					m_HitFlag = true;
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
					m_HitFlag = true;
					break;
				}
			}
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
void cOrb::Draw_Default() const
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
void cOrb::Draw_FadeAdd() const
{
	unsigned int color = 0xffffff;
	switch (m_Owner->PlayerID) {
	break;	case 0:	color = 0xff8080;
	break;	case 1:	color = 0x8080ff;
	break;	case 2:	color = 0x80ff80;
	break;	case 3:	color = 0xffff80;
	}

	DrawLine(
		cGame::ScreenDif.x + (int)m_Pos_prev.x,
		cGame::ScreenDif.y + (int)m_Pos_prev.y,
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		color, 1 + (m_AtkDamage - 1) * 2);
}

/* アクティブ状態 */
bool cOrb::Active() const
{
	return !m_HitFlag;
}
