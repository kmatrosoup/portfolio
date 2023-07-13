#pragma once

#include "BaseBook.h"

class cFireballBook : public cBaseBook
{
	// 公開関数
public:
	cFireballBook(VEC2F pos, VEC2F motion, float angle, bool no_erase_flag);	// コンストラクタ

	// 非公開関数
private:
	void m_RunMagicProcess() override;	// 発動時の処理
};
