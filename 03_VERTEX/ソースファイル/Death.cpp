//////////////////////
//   プリプロセス   //
//////////////////////
#include "Death.h"
#include "DxLib.h"
#include "EffectMgr.h"
#include "Game.h"
#include "Easing.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const int cDeath::m_MaxTime(24);	// 最大生存時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cDeath::cDeath(VEC2F pos)
	: cBaseEffect(m_MaxTime)
	, m_Pos(pos)
	, m_Angle(DX_PI / 180.0 * GetRand(359))
{
}

/* デストラクタ */
cDeath::~cDeath()
{
}

/* 更新 */
void cDeath::Update()
{
	--m_Timer;
}

/* 描画・DEFAULT */
void cDeath::Draw_Default() const
{
	float param = (float)(m_MaxTime - m_Timer) / m_MaxTime;
	SetDrawBright(225, 0, 20);
	DrawCircle(
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		(int)(Easing::Quad::Out(param) * 80.0f),
		0xffffff,
		FALSE,
		1 + (int)(Easing::Quad::Out((float)m_Timer / m_MaxTime) * 3.0f));
	DrawRotaGraph3(
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		50,
		50,
		(Easing::Quad::Out(param) * 5.0),
		0.05 - (param * 0.05),
		m_Angle,
		cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::RHOMBUS), TRUE);
	DrawRotaGraph3(
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		50,
		50,
		(Easing::Quad::Out(param) * 2.5),
		0.05 - (param * 0.05),
		m_Angle + DX_PI / 2.0,
		cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::RHOMBUS), TRUE);
	SetDrawBright(255, 255, 255);
}

/* アクティブ状態 */
bool cDeath::Active() const
{
	return m_Timer >= 0;
}
