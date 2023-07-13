//////////////////////
//   プリプロセス   //
//////////////////////
#include "Config.h"
#include "DxLib.h"
#include "System.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "CommonData.h"
#include "SoundData.h"
#include "Title.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
int cConfig::m_Cursor(0);

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cConfig::cConfig()
{
}

/* デストラクタ */
cConfig::~cConfig()
{
}

/* 更新 */
cBaseScene* cConfig::Update()
{
	// カーソル移動
	{
		int inputX = (cInputMgr::In(eINPUT_DEVICE::ALL, eINPUT_BUTTON::STICK_RIGHT) - cInputMgr::In(eINPUT_DEVICE::ALL, eINPUT_BUTTON::STICK_LEFT));
		if (inputX)
		{
			m_Cursor = (m_Cursor + 4 + inputX) % 4;
			cSoundData::GetInstance().Play(eSOUND::CURSOR);
		}
	}

	// コントローラー変更
	for (int di = 0; di < (int)eINPUT_DEVICE::ALL; ++di)
	{
		if (cInputMgr::In((eINPUT_DEVICE)di, eINPUT_BUTTON::BUTTON_2))
		{
			eINPUT_DEVICE tmp_InputDevice = cCommonData::InputDevice[m_Cursor];
			cCommonData::InputDevice[m_Cursor] = (eINPUT_DEVICE)di;
			for (int pi = 0; pi < 4; ++pi) {
				if (pi == m_Cursor)
					continue;
				if (cCommonData::InputDevice[pi] == (eINPUT_DEVICE)di) {
					cCommonData::InputDevice[pi] = tmp_InputDevice;
					break;
				}
			}
			cSoundData::GetInstance().Play(eSOUND::DECIDE);
			break;
		}
	}

	// キャンセルでタイトルに戻る
	if (cInputMgr::In(eINPUT_DEVICE::ALL, eINPUT_BUTTON::BUTTON_1))
	{
		cSoundData::GetInstance().Play(eSOUND::CANCEL);
		return new cTitle();
	}

	return this;
}

/* 描画 */
void cConfig::Draw() const
{
	for (int i = 0; i < 4; ++i)
	{
		if (i == m_Cursor)
			DrawBox(
				190 + i * 120,
				190,
				298 + i * 120,
				268,
				0xffff00, FALSE);
		DrawFormatString(
			200 + i * 120,
			200,
			(i == 0) ? 0xff4040 : (i == 1) ? 0x4040ff : (i == 2) ? 0x40ff40 : 0xffff40, "Player%d", i + 1);

		switch (cCommonData::InputDevice[i]) {
		case eINPUT_DEVICE::KEY_MOUSE:
			DrawFormatString(
				200 + i * 120,
				240,
				0xffffff, "Keyboard");
			break;
		default:
			DrawFormatString(
				200 + i * 120,
				240,
				0xffffff, "Joypad(%d)", (int)cCommonData::InputDevice[i] + 1);
			break;
		}
	}

	DrawString(0, 0, "[Config]", 0xffffff);
}
