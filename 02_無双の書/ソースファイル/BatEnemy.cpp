// プリプロセス
#include "BatEnemy.h"
#include "Player.h"
#include "Score.h"
#include <cmath>

// コンストラクタ
cBatEnemy::cBatEnemy(float x)
	: cBaseEnemy(eENEMY::BAT, 1, 16.0f, x)
{
}

// 固有処理
void cBatEnemy::m_OwnProcess()
{
	float angle = atan2f(cPlayer::GetInstance().GetPos().y - m_Pos.y, cPlayer::GetInstance().GetPos().x - m_Pos.x);
	m_Motion += VEC2F(cosf(angle), sinf(angle)) * 0.04f;
	m_InversionFlag = cosf(angle) < 0.0f;
}

// スコア加算
void cBatEnemy::m_AddScore()
{
	cScore::GetInstance().AddScore(150);
}
