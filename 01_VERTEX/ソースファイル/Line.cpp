//////////////////////
//   プリプロセス   //
//////////////////////
#include "Line.h"
#include "DxLib.h"
#include "EffectMgr.h"
#include "Game.h"
#include <cmath>

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const int cLine::m_MaxTime(60);	// 最大生存時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cLine::cLine(VEC2F pos, float angle, float motion, unsigned int color)
	: cBaseEffect(m_MaxTime)
	, m_Pos(pos)
	, m_Angle(angle)
	, m_Motion(motion)
	, m_Color(color)
{
}

/* デストラクタ */
cLine::~cLine()
{
}

/* 更新 */
void cLine::Update()
{
	--m_Timer;

	m_Pos.x += cosf(m_Angle) * m_Motion;
	m_Pos.y += sinf(m_Angle) * m_Motion;
	m_Motion *= 0.95f;
}

/* 描画・DEFAULT*/
void cLine::Draw_Default() const
{
	int param = m_Timer * 300 / m_MaxTime - 100 - GetRand(200);
	SetDrawBlendMode(DX_BLENDMODE_ADD, __max(m_Timer * 4, 0));
	SetDrawBright((m_Color >> 16) % 256, (m_Color >> 8) % 256, m_Color % 256);
	SetDrawAddColor(param, param, param);
	DrawRotaGraphF(cGame::ScreenDif.x + m_Pos.x, cGame::ScreenDif.y + m_Pos.y, 1.0, m_Angle, cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::LINE), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);
	SetDrawAddColor(0, 0, 0);
}

/* アクティブ状態 */
bool cLine::Active() const
{
	return m_Timer >= 0;
}
