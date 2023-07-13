// プリプロセス
#include "ExplosionBook.h"
#include "AtkObjMgr.h"

// コンストラクタ
cExplosionBook::cExplosionBook(VEC2F pos, VEC2F motion, float angle, bool no_erase_flag)
	: cBaseBook(eSPELL::EXPLOSION, pos, motion, angle, 1, no_erase_flag)
{
}

// 発動時の処理
void cExplosionBook::m_RunMagicProcess()
{
	cAtkObjMgr::GetInstance().CreateAtkObj(eATK_OBJ::EXPLOSION, m_Pos, m_InversionFlag);
}
