//////////////////////
//   プリプロセス   //
//////////////////////
#include "Laser.h"
#include "DxLib.h"
#include "Player.h"
#include "Field.h"
#include "Game.h"
#include "SoundData.h"
#include "AtkobjMgr.h"
#include <cmath>
using namespace std;

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const float cLaser::m_HitWidth(4.0f);			// 衝突幅
const float cLaser::m_HitWidth_UpScale(2.0f);	// 攻撃力による追加衝突幅
const int cLaser::m_MaxTime(40);				// 生存最大時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cLaser::cLaser(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet)
	: cBaseAtkobj(Owner)
	, m_AtkDamage(AtkDamage)
	, m_Timer(m_MaxTime)
{
	m_Ray.push_back({ Pos, VEC2F(cosf(Angle), sinf(Angle)) * 1000.0f });

	if (cField::InWall(m_Ray.front().first)) {
		m_Ray.front().second = VEC2F::ZERO;
		return;
	}

	for (auto& ri : m_Ray)
	{
		bool col_flag = false;
		VEC2F prev_pos = ri.first + ri.second;
		const float now_angle = atan2f(ri.second.y, ri.second.x);
		float next_angle;
		float lowest_param = 1.0f;

		if (ri.second.y > 0.0f) {
			for (const auto& li : cField::LineU) {
				float param = (li.first.y * cField::TileSize - ri.first.y) / ri.second.y;
				if (param >= 0.0f && param < 1.0f && lowest_param > param) {
					float tmpX = ri.first.x + ri.second.x * param;
					if (tmpX >= (float)(li.first.x * cField::TileSize) && tmpX <= (float)((li.first.x + li.second) * cField::TileSize)) {
						lowest_param = param;
						prev_pos = ri.first + ri.second * param;
						next_angle = DX_PI_F * 2.0f - now_angle;
						col_flag = true;
					}
				}
			}
		}
		if (ri.second.y < 0.0f) {
			for (const auto& li : cField::LineD) {
				float param = (li.first.y * cField::TileSize - ri.first.y) / ri.second.y;
				if (param >= 0.0f && param < 1.0f && lowest_param > param) {
					float tmpX = ri.first.x + ri.second.x * param;
					if (tmpX >= (float)(li.first.x * cField::TileSize) && tmpX <= (float)((li.first.x + li.second) * cField::TileSize)) {
						lowest_param = param;
						prev_pos = ri.first + ri.second * param;
						next_angle = DX_PI_F * 2.0f - now_angle;
						col_flag = true;
					}
				}
			}
		}
		if (ri.second.x > 0.0f) {
			for (const auto& li : cField::LineL) {
				float param = (li.first.x * cField::TileSize - ri.first.x) / ri.second.x;
				if (param >= 0.0f && param < 1.0f && lowest_param > param) {
					float tmpY = ri.first.y + ri.second.y * param;
					if (tmpY >= (float)(li.first.y * cField::TileSize) && tmpY <= (float)((li.first.y + li.second) * cField::TileSize)) {
						lowest_param = param;
						prev_pos = ri.first + ri.second * param;
						next_angle = DX_PI_F - now_angle;
						col_flag = true;
					}
				}
			}
		}
		if (ri.second.x < 0.0f) {
			for (const auto& li : cField::LineR) {
				float param = (li.first.x * cField::TileSize - ri.first.x) / ri.second.x;
				if (param >= 0.0f && param < 1.0f && lowest_param > param) {
					float tmpY = ri.first.y + ri.second.y * param;
					if (tmpY >= (float)(li.first.y * cField::TileSize) && tmpY <= (float)((li.first.y + li.second) * cField::TileSize)) {
						lowest_param = param;
						prev_pos = ri.first + ri.second * param;
						next_angle = DX_PI_F - now_angle;
						col_flag = true;
					}
				}
			}
		}
		ri.second = prev_pos - ri.first;
		if (col_flag && --Ricochet >= 0)
			m_Ray.push_back({ prev_pos, VEC2F(cosf(next_angle), sinf(next_angle)) * 1000.0f });
	}
}

/* デストラクタ */
cLaser::~cLaser()
{
}

/* 更新 */
void cLaser::Update()
{
	--m_Timer;

	if (m_Timer == 20)
		cSoundData::GetInstance().Play(eSOUND::BULLET_LASER);

	// 衝突判定
	if (m_Timer >= 20 || m_Timer < 16)
		return;

	for (auto& pi : cGame::pVecPlayer)
	{
		if (pi->PlayerID == m_Owner->PlayerID)
			continue;
		if (find(m_HitPlayerID.begin(), m_HitPlayerID.end(), pi->PlayerID) != m_HitPlayerID.end())
			continue;
		if (pi->IsRepair())
			continue;
		if (pi->IsDead())
			continue;

		for (auto& ri : m_Ray)
		{
			if (VEC2F::CheckIntersect_lineCircle(get<0>(ri), get<0>(ri) + get<1>(ri), pi->Pos, pi->Radius + m_HitWidth + m_HitWidth_UpScale * m_AtkDamage)) {
				if (pi->IsShield() && cosf(pi->Angle - atan2f(get<1>(ri).y, get<1>(ri).x)) < 0.0f) {
					m_HitPlayerID.push_back(pi->PlayerID);
					cSoundData::GetInstance().Play(eSOUND::REFLECT);
				}
				else {
					m_HitPlayerID.push_back(pi->PlayerID);
					pi->TakenDamageProcess(m_Owner->PlayerID, m_AtkDamage);
					m_Owner->DealtDamageProcess(pi->PlayerID);
				}
				break;
			}
		}
	}
}

/* 描画・DEFAULT */
void cLaser::Draw_Default() const
{
}

/* 描画・ADD */
void cLaser::Draw_FadeAdd() const
{
	unsigned int color = 0xffffff;
	switch (m_Owner->PlayerID) {
	break;	case 0:	color = 0xff8080;
	break;	case 1:	color = 0x8080ff;
	break;	case 2:	color = 0x80ff80;
	break;	case 3:	color = 0xffff80;
	}

	if (m_Timer > 20 && m_Timer % 5 == 0) {
		for (const auto& i : m_Ray)
			DrawLine(
				cGame::ScreenDif.x + (int)i.first.x,
				cGame::ScreenDif.y + (int)i.first.y,
				cGame::ScreenDif.x + (int)(i.first.x + i.second.x),
				cGame::ScreenDif.y + (int)(i.first.y + i.second.y),
				color);
	}

	if (m_Timer <= 20) {
		for (const auto& i : m_Ray)
			DrawLine(
				cGame::ScreenDif.x + (int)i.first.x,
				cGame::ScreenDif.y + (int)i.first.y,
				cGame::ScreenDif.x + (int)(i.first.x + i.second.x),
				cGame::ScreenDif.y + (int)(i.first.y + i.second.y),
				color, 1 + (m_Timer * m_AtkDamage) / 3);
	}
}

/* アクティブ状態 */
bool cLaser::Active() const
{
	return m_Timer >= 0;
}
