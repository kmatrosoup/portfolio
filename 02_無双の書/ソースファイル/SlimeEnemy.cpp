// プリプロセス
#include "SlimeEnemy.h"
#include "Player.h"
#include "Score.h"
#include <cmath>

// コンストラクタ
cSlimeEnemy::cSlimeEnemy(float x)
	: cBaseEnemy(eENEMY::SLIME, 2, 20.0f, x)
{
}

// 固有処理
void cSlimeEnemy::m_OwnProcess()
{
	float angle = atan2f(cPlayer::GetInstance().GetPos().y - m_Pos.y, cPlayer::GetInstance().GetPos().x - m_Pos.x);
	m_Motion += VEC2F(cosf(angle), sinf(angle)) * 0.015f;
	m_InversionFlag = cosf(angle) < 0.0f;
}

// スコア加算
void cSlimeEnemy::m_AddScore()
{
	cScore::GetInstance().AddScore(100);
}
