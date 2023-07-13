//////////////////////
//   プリプロセス   //
//////////////////////
#include "ElectricTrap.h"
#include "DxLib.h"
#include "Player.h"
#include "Field.h"
#include "Game.h"
#include "SoundData.h"
#include "AtkobjMgr.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const float cElectricTrap::m_Radius(16.0f);	// 半径
const int cElectricTrap::m_MaxTime(480);	// 最大生存時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cElectricTrap::cElectricTrap(cPlayer* Owner, VEC2F Pos)
	: cBaseAtkobj(Owner)
	, m_Pos(Pos)
	, m_Timer(m_MaxTime)
{
}

/* デストラクタ */
cElectricTrap::~cElectricTrap()
{
}

/* 更新 */
void cElectricTrap::Update()
{
	--m_Timer;

	// 衝突判定
	for (auto& i : cGame::pVecPlayer)
	{
		if (i->PlayerID == m_Owner->PlayerID)
			continue;
		if (i->IsRepair())
			continue;
		if (i->IsDead())
			continue;

		if ((i->Pos.x - m_Pos.x) * (i->Pos.x - m_Pos.x) + (i->Pos.y - m_Pos.y) * (i->Pos.y - m_Pos.y) <= (i->Radius + m_Radius) * (i->Radius + m_Radius))
		{
			i->SetPalsyTime(10);
			break;
		}
	}
}

/* 描画・ADD */
void cElectricTrap::Draw_FadeAdd() const
{
	unsigned int color = 0x20202020;
	switch (m_Owner->PlayerID) {
	break;	case 0:	color = 0x20200000;
	break;	case 1:	color = 0x20000020;
	break;	case 2:	color = 0x20002000;
	break;	case 3:	color = 0x20202000;
	}

	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	{
		int dif = (int)(sinf(DX_PI_F * 2.0f * m_Timer * 0.03f) * 20.0f);
		SetDrawAddColor(dif, dif, dif);
	}

	DrawCircle(
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		(int)m_Radius, color, FALSE, 2);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawAddColor(0, 0, 0);
}

/* アクティブ状態 */
bool cElectricTrap::Active() const
{
	return m_Timer >= 0;
}
