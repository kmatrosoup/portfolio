//////////////////////
//   プリプロセス   //
//////////////////////
#include "BaseEffect.h"
#include "Player.h"

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cBaseEffect::cBaseEffect(int time)
	: m_Timer(time)
{
}

/* デストラクタ */
cBaseEffect::~cBaseEffect()
{
}
