//////////////////////
//   プリプロセス   //
//////////////////////
#include "Blast.h"
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
const int cBlast::m_MaxTime(24);	// 最大生存時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cBlast::cBlast(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Radius)
	: cBaseAtkobj(Owner)
	, m_AtkDamage(AtkDamage)
	, m_Pos(Pos)
	, m_Radius(Radius)
	, m_Timer(m_MaxTime)
{
	unsigned int color = 0xe0e0e0;
	if (m_Owner) {
		switch (m_Owner->PlayerID) {
		break;	case 0:	color = 0xe04040;
		break;	case 1:	color = 0x4040e0;
		break;	case 2:	color = 0x40e040;
		break;	case 3:	color = 0xe0e040;
		}
	}

	for (int i = 0; i < 20; ++i)
		cEffectMgr::GetInstance().Create_Line(m_Pos, DX_PI_F / 180.0f * GetRand(359), (20 + GetRand(60)) / 500.0f * m_Radius, color);
	cEffectMgr::GetInstance().Create_Circle(m_Pos, m_Radius, m_Radius * 0.04f, color);
	cSoundData::GetInstance().Play(eSOUND::BLAST);
}

/* デストラクタ */
cBlast::~cBlast()
{
}

/* 更新 */
void cBlast::Update()
{
	--m_Timer;

	// 衝突判定
	if (m_Timer < m_MaxTime - 4)
		return;

	for (auto& i : cGame::pVecPlayer)
	{
		if (m_Owner)
			if (i->PlayerID == m_Owner->PlayerID)
				continue;
		if (find(m_HitPlayerID.begin(), m_HitPlayerID.end(), i->PlayerID) != m_HitPlayerID.end())
			continue;
		if (i->IsRepair())
			continue;
		if (i->IsDead())
			continue;

		if ((i->Pos.x - m_Pos.x) * (i->Pos.x - m_Pos.x) + (i->Pos.y - m_Pos.y) * (i->Pos.y - m_Pos.y) <= (i->Radius + m_Radius) * (i->Radius + m_Radius)) {
			m_HitPlayerID.push_back(i->PlayerID);
			(m_Owner) ?
				i->TakenDamageProcess(m_Owner->PlayerID, m_AtkDamage) :
				i->TakenDamageProcess(4, m_AtkDamage);
			if (m_Owner)
				m_Owner->DealtDamageProcess(i->PlayerID);
		}
	}
}

/* 描画・ADD */
void cBlast::Draw_FadeAdd() const
{
	unsigned int color = 0x80808080;
	if (m_Owner) {
		switch (m_Owner->PlayerID) {
		break;	case 0:	color = 0x80800000;
		break;	case 1:	color = 0x80000080;
		break;	case 2:	color = 0x80008000;
		break;	case 3:	color = 0x80808000;
		}
	}

	SetDrawAddColor((int)(m_Timer * 255.0f / m_MaxTime), (int)(m_Timer * 255.0f / m_MaxTime), (int)(m_Timer * 255.0f / m_MaxTime));
	DrawCircle(
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		(int)m_Radius, color, TRUE);
	SetDrawAddColor(0, 0, 0);
}

/* アクティブ状態 */
bool cBlast::Active() const
{
	return m_Timer >= 0;
}
