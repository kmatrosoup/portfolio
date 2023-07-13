//////////////////////
//   プリプロセス   //
//////////////////////
#include "Dead.h"
#include "DxLib.h"
#include "EffectMgr.h"
#include "Game.h"
#include "Easing.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const int cDead::m_MaxTime(26);	// 最大生存時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cDead::cDead(VEC2F pos, unsigned int color)
	: cBaseEffect(m_MaxTime)
	, m_Pos(pos)
	, m_Color(color)
{
}

/* デストラクタ */
cDead::~cDead()
{
}

/* 更新 */
void cDead::Update()
{
	--m_Timer;
}

/* 描画・DEFAULT*/
void cDead::Draw_Default() const
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawCircle(
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		(int)((1.0f - Easing::Cubic::In((float)m_Timer / (float)m_MaxTime)) * 160.0f),
		m_Color,
		FALSE,
		(int)((Easing::Cubic::In((float)m_Timer / (float)m_MaxTime)) * 30.0f));
	SetDrawBlendMode(DX_BLENDMODE_ADD, m_Timer * 80 / m_MaxTime);
	DrawBox(0, 0, Sw, Sh, m_Color, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/* アクティブ状態 */
bool cDead::Active() const
{
	return m_Timer >= 0;
}
