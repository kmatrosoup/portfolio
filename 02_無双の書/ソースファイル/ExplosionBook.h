#pragma once

#include "BaseBook.h"

class cExplosionBook : public cBaseBook
{
	// 公開関数
public:
	cExplosionBook(VEC2F pos, VEC2F motion, float angle, bool no_erase_flag);	// コンストラクタ

	// 非公開関数
private:
	void m_RunMagicProcess() override;	// 発動時の処理
};
