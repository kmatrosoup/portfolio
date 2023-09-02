//////////////////////
//   プリプロセス   //
//////////////////////
#include "Confetti.h"
#include "DxLib.h"
#include "EffectMgr.h"
#include "Game.h"
#include <cmath>

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const int cConfetti::m_MaxTime(180);	// 最大生存時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cConfetti::cConfetti(VEC2F pos, unsigned int color, float angle, float motion)
	: cBaseEffect(m_MaxTime)
	, m_Pos(pos)
	, m_Color(color)
	, m_Motion(VEC2F(cosf(angle), sinf(angle))* motion)
	, m_FrameTimer(GetRand(23))
	, m_LightAngle(DX_PI_F / 180.0f * GetRand(359))
{
}

/* デストラクタ */
cConfetti::~cConfetti()
{
}

/* 更新 */
void cConfetti::Update()
{
	--m_Timer;
	++m_FrameTimer %= 24;
	m_LightAngle += 0.1f;
	while (m_LightAngle >= DX_PI_F * 2.0f)	m_LightAngle -= DX_PI_F * 2.0f;

	m_Pos += m_Motion;
	m_Motion.x += (GetRand(200) - 100) * 0.005f;
	m_Motion.y += 0.1f;
	m_Motion *= 0.92f;
}

/* 描画・DEFAULT*/
void cConfetti::Draw_Default() const
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, m_Timer * 255 / m_MaxTime);
	SetDrawBright((m_Color >> 16) % 256, (m_Color >> 8) % 256, m_Color % 256);
	int param = (int)(sinf(m_LightAngle) * 200.0f);
	SetDrawAddColor(param, param, param);
	switch ((m_FrameTimer / 8) % 3) {
	case 0:	DrawRotaGraph(cGame::ScreenDif.x + (int)m_Pos.x, cGame::ScreenDif.y + (int)m_Pos.y, 1.0f, 0.0, cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::CONFETTI_1), TRUE);	break;
	case 1:	DrawRotaGraph(cGame::ScreenDif.x + (int)m_Pos.x, cGame::ScreenDif.y + (int)m_Pos.y, 1.0f, 0.0, cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::CONFETTI_2), TRUE);	break;
	case 2:	DrawRotaGraph(cGame::ScreenDif.x + (int)m_Pos.x, cGame::ScreenDif.y + (int)m_Pos.y, 1.0f, 0.0, cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::CONFETTI_3), TRUE);	break;
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);
	SetDrawAddColor(0, 0, 0);
}

/* アクティブ状態 */
bool cConfetti::Active() const
{
	return m_Timer >= 0;
}
