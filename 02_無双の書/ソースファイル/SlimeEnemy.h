#pragma once

#include "BaseEnemy.h"

class cSlimeEnemy : public cBaseEnemy
{
	// 公開関数
public:
	cSlimeEnemy(float x);	// コンストラクタ

	// 非公開関数
private:
	void m_OwnProcess() override;	// 固有処理
	void m_AddScore() override;		// スコア加算
};
