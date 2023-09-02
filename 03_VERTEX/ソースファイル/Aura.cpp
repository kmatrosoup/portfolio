//////////////////////
//   プリプロセス   //
//////////////////////
#include "Aura.h"
#include "DxLib.h"
#include "EffectMgr.h"
#include "Game.h"
#include <cmath>

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const int cAura::m_MaxTime(60);	// 最大生存時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cAura::cAura(VEC2F pos, unsigned int color, float scale, float angle, float motion)
	: cBaseEffect(m_MaxTime)
	, m_Pos(pos)
	, m_Color(color)
	, m_Scale(scale)
	, m_Motion(VEC2F(cosf(angle), sinf(angle))* motion)
{
}

/* デストラクタ */
cAura::~cAura()
{
}

/* 更新 */
void cAura::Update()
{
	--m_Timer;

	m_Pos += m_Motion;
	m_Motion.y += -0.1f;
	m_Motion *= 0.97f;
}

/* 描画・DEFAULT*/
void cAura::Draw_Default() const
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, m_Timer * 255 / m_MaxTime);
	SetDrawBright((m_Color >> 16) % 256, (m_Color >> 8) % 256, m_Color % 256);
	DrawRotaGraph(cGame::ScreenDif.x + (int)m_Pos.x, cGame::ScreenDif.y + (int)m_Pos.y, m_Scale / 10.0, 0.0, cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::AURA), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);
}

/* アクティブ状態 */
bool cAura::Active() const
{
	return m_Timer >= 0;
}
