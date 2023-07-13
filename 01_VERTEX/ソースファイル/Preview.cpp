//////////////////////
//   プリプロセス   //
//////////////////////
#include "Preview.h"
#include "DxLib.h"
#include "System.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "Field.h"
#include "SoundData.h"
#include "CommonData.h"
#include "Select.h"

#include "Input.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const int cPreview::m_MaxTime(120);

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cPreview::cPreview()
	: m_Timer(0)
{
	m_Image_Circle = LoadGraph("data\\image\\circle.png");

	cCommonData::FieldID = GetRand(cField::GetMapDataNum() - 1);
}

/* デストラクタ */
cPreview::~cPreview()
{
	DeleteGraph(m_Image_Circle);
}

/* 更新 */
cBaseScene* cPreview::Update()
{
	if (Input::Keyboard::In(KEY_INPUT_Z))
		return new cSelect();

	if (++m_Timer >= m_MaxTime)
		return new cSelect();

	return this;
}

/* 描画 */
void cPreview::Draw() const
{
	DrawCircleGauge(
		101,
		101,
		m_Timer * 100.0 / m_MaxTime, m_Image_Circle);
	DrawCircle(
		100,
		100,
		32, 0xffffff, FALSE, 2);

	DrawString(0, 0, "[Preview]", 0xffffff);
}
