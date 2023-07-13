// プリプロセス
#include "SkullEnemy.h"
#include "Player.h"
#include "Score.h"
#include <cmath>

// コンストラクタ
cSkullEnemy::cSkullEnemy(float x)
	: cBaseEnemy(eENEMY::SKULL, 3, 30.0f, x)
{
}

// 固有処理
void cSkullEnemy::m_OwnProcess()
{
	float angle = atan2f(cPlayer::GetInstance().GetPos().y - m_Pos.y, cPlayer::GetInstance().GetPos().x - m_Pos.x);
	m_Motion += VEC2F(cosf(angle), sinf(angle)) * 0.02f;
	m_InversionFlag = cosf(angle) < 0.0f;
}

// スコア加算
void cSkullEnemy::m_AddScore()
{
	cScore::GetInstance().AddScore(200);
}
