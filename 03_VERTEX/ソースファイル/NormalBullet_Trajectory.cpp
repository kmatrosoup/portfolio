//////////////////////
//   プリプロセス   //
//////////////////////
#include "NormalBullet_Trajectory.h"
#include "DxLib.h"
#include "EffectMgr.h"
#include "Game.h"
#include <cmath>

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const int cNormalBullet_Trajectory::m_MaxTime(20);	// 最大生存時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cNormalBullet_Trajectory::cNormalBullet_Trajectory(VEC2F pos, float angle, unsigned int color, double max_scale)
	: cBaseEffect(m_MaxTime)
	, m_Pos(pos)
	, m_Angle(angle)
	, m_Color(color)
	, m_MaxScale(max_scale)
{
}

/* デストラクタ */
cNormalBullet_Trajectory::~cNormalBullet_Trajectory()
{
}

/* 更新 */
void cNormalBullet_Trajectory::Update()
{
	--m_Timer;
}

/* 描画・DEFAULT*/
void cNormalBullet_Trajectory::Draw_Default() const
{
	SetDrawBright((m_Color >> 16) % 256, (m_Color >> 8) % 256, (m_Color >> 0) % 256);
	SetDrawBlendMode(DX_BLENDMODE_ADD, m_Timer * 255 / m_MaxTime);
	DrawRotaGraph(
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		((double)m_Timer / (double)m_MaxTime) * 0.01 * m_MaxScale, m_Angle, cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::RHOMBUS), TRUE);
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/* アクティブ状態 */
bool cNormalBullet_Trajectory::Active() const
{
	return m_Timer >= 0;
}
