//////////////////////
//   プリプロセス   //
//////////////////////
#include "Circle.h"
#include "DxLib.h"
#include "EffectMgr.h"
#include "Game.h"
#include <cmath>

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const int cCircle::m_MaxTime(30);	// 最大生存時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cCircle::cCircle(VEC2F pos, float radius, float motion, unsigned int color)
	: cBaseEffect(m_MaxTime)
	, m_Pos(pos)
	, m_Radius(radius)
	, m_Motion(motion)
	, m_Color(color)
{
}

/* デストラクタ */
cCircle::~cCircle()
{
}

/* 更新 */
void cCircle::Update()
{
	--m_Timer;

	m_Radius += m_Motion;
	m_Motion *= 0.92f;
}

/* 描画・DEFAULT*/
void cCircle::Draw_Default() const
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, __max(m_Timer * 2, 0));
	SetDrawBright((m_Color >> 16) % 256, (m_Color >> 8) % 256, m_Color % 256);
	DrawRotaGraph(cGame::ScreenDif.x + (int)m_Pos.x, cGame::ScreenDif.y + (int)m_Pos.y, m_Radius / 25.0, 0.0, cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::RING_OUT), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);
}

/* アクティブ状態 */
bool cCircle::Active() const
{
	return m_Timer >= 0;
}
