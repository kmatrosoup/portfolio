//////////////////////
//   プリプロセス   //
//////////////////////
#include "Wind.h"
#include "DxLib.h"
#include "Player.h"
#include "Field.h"
#include "Game.h"
#include "SoundData.h"
#include "AtkobjMgr.h"
#include "Easing.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cWind::cWind(cPlayer* Owner, VEC2F Pos, float Radius, float Power, int Time)
	: cBaseAtkobj(Owner)
	, m_Pos(Pos)
	, m_Radius(Radius)
	, m_Power(Power)
	, m_Timer(Time)
{
}

/* デストラクタ */
cWind::~cWind()
{
}

/* 更新 */
void cWind::Update()
{
	--m_Timer;

	// 衝突判定
	for (auto& i : cGame::pVecPlayer)
	{
		if (m_Owner)
			if (i->PlayerID == m_Owner->PlayerID)
				continue;
		if (i->IsDead())
			continue;

		float distance = sqrtf((i->Pos.x - m_Pos.x) * (i->Pos.x - m_Pos.x) + (i->Pos.y - m_Pos.y) * (i->Pos.y - m_Pos.y));
		if (distance < m_Radius) {
			float angle = atan2f(i->Pos.y - m_Pos.y, i->Pos.x - m_Pos.x);
			i->AddMotion(VEC2F(cosf(angle), sinf(angle)) * (Easing::Cubic::Out((m_Radius - distance) / m_Radius) * m_Power));
		}
	}
}

/* アクティブ状態 */
bool cWind::Active() const
{
	return m_Timer >= 0;
}
