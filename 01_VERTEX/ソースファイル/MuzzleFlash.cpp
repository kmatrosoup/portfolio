//////////////////////
//   プリプロセス   //
//////////////////////
#include "MuzzleFlash.h"
#include "DxLib.h"
#include "EffectMgr.h"
#include "Game.h"
#include <cmath>

////////////////////
//   定数の定義   //
////////////////////

#define MUZZLEFLASH_OFFSET (12.0f)	// マズルフラッシュのオフセット

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const int cMuzzleFlash::m_MaxTime(2);	// 最大生存時間

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cMuzzleFlash::cMuzzleFlash(VEC2F pos, float scale, float angle)
	: cBaseEffect(m_MaxTime)
	, m_Pos(pos + VEC2F(cosf(angle), sinf(angle)) * MUZZLEFLASH_OFFSET * scale)
	, m_Scale(scale)
	, m_Angle(angle)
{
}

/* デストラクタ */
cMuzzleFlash::~cMuzzleFlash()
{
}

/* 更新 */
void cMuzzleFlash::Update()
{
	--m_Timer;
}

/* 描画・DEFAULT*/
void cMuzzleFlash::Draw_Default() const
{
	(m_Timer == m_MaxTime - 1) ?
		SetDrawBlendMode(DX_BLENDMODE_SUB, 255) :
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawRotaGraph(cGame::ScreenDif.x + (int)m_Pos.x, cGame::ScreenDif.y + (int)m_Pos.y, m_Scale, m_Angle, cEffectMgr::GetInstance().GetImageHandle(ePARTICLE::MUZZLE_FLASH), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/* アクティブ状態 */
bool cMuzzleFlash::Active() const
{
	return m_Timer >= 0;
}
