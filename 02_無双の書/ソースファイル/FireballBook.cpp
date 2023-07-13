// プリプロセス
#include "FireballBook.h"
#include "AtkObjMgr.h"

// コンストラクタ
cFireballBook::cFireballBook(VEC2F pos, VEC2F motion, float angle, bool no_erase_flag)
	: cBaseBook(eSPELL::FIRE_BALL, pos, motion, angle, 3, no_erase_flag)
{
}

// 発動時の処理
void cFireballBook::m_RunMagicProcess()
{
	cAtkObjMgr::GetInstance().CreateAtkObj(eATK_OBJ::FIREBALL, m_Pos, m_InversionFlag);
}
