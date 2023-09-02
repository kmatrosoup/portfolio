//////////////////////
//   プリプロセス   //
//////////////////////
#include "BaseAtkobj.h"
#include "Player.h"

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cBaseAtkobj::cBaseAtkobj(cPlayer* Owner)
	: m_Owner(Owner)
{
}
