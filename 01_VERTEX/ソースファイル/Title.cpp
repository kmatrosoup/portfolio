//////////////////////
//   プリプロセス   //
//////////////////////
#include "Title.h"
#include "DxLib.h"
#include "System.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "SoundData.h"
#include "BgmData.h"
#include "CommonData.h"
#include "Preview.h"
#include "Config.h"
#include <cmath>

#include "Input.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
cTitle::eTITLE_MENU	cTitle::m_Cursor(eTITLE_MENU::PVP);	// カーソル・メニュー
int					cTitle::m_CursorPvP(0);				// カーソル・PvP
const int			cTitle::m_MaxTime(60);				// 最大時間
const int			cTitle::m_MaxStartTime(150);		// 最大開始時間
const VEC2			cTitle::m_TitlePos(Sw / 2, 160);	// タイトルの位置

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cTitle::cTitle()
	: m_SelectedPvP(false)
	, m_Timer(0)
	, m_StartTimer(0)
{
	// 画像の読み込み
	m_Image_Title = LoadGraph("data\\image\\title\\title.png");
	m_Image_TitleText[0] = LoadGraph("data\\image\\title\\title_text_0.png");
	m_Image_TitleText[1] = LoadGraph("data\\image\\title\\title_text_1.png");
	m_Image_TitleText[2] = LoadGraph("data\\image\\title\\title_text_2.png");
	m_Image_TitleText[3] = LoadGraph("data\\image\\title\\title_text_3.png");
	m_Image_TitleText[4] = LoadGraph("data\\image\\title\\title_text_4.png");
	m_Image_TitleText[5] = LoadGraph("data\\image\\title\\title_text_5.png");
	m_Image_TitleText[6] = LoadGraph("data\\image\\title\\title_text_6.png");
	m_Image_Menu[(int)eTITLE_MENU::PVP] = LoadGraph("data\\image\\title\\menu_pvp.png");
	m_Image_Menu[(int)eTITLE_MENU::CONFIG] = LoadGraph("data\\image\\title\\menu_config.png");

	// BGMの再生
	cBgmData::GetInstance().Play(eBGM::TITLE, true);
}

/* デストラクタ */
cTitle::~cTitle()
{
	// 画像の解放
	DeleteGraph(m_Image_Title);;
	for (int i = 0; i < (int)eTITLE_MENU::MAX; ++i)	DeleteGraph(m_Image_Menu[i]);
}

/* 更新 */
cBaseScene* cTitle::Update()
{
	if (Input::Keyboard::In(KEY_INPUT_Z))
		return new cPreview();

	// タイマー制御
	++m_Timer %= m_MaxTime;
	if (m_StartTimer < m_MaxStartTime)
		++m_StartTimer;

	// 操作
	if (m_StartTimer >= 90) {
		// カーソル移動
		if (!m_SelectedPvP) {
			int inputX = (cInputMgr::In(eINPUT_DEVICE::ALL, eINPUT_BUTTON::STICK_RIGHT) - cInputMgr::In(eINPUT_DEVICE::ALL, eINPUT_BUTTON::STICK_LEFT));
			if (inputX)
			{
				m_Cursor = (eTITLE_MENU)(((int)m_Cursor + (int)eTITLE_MENU::MAX + inputX) % (int)eTITLE_MENU::MAX);
				cSoundData::GetInstance().Play(eSOUND::CURSOR);
			}
		}
		else {
			if (cInputMgr::In(eINPUT_DEVICE::ALL, eINPUT_BUTTON::STICK_RIGHT) || cInputMgr::In(eINPUT_DEVICE::ALL, eINPUT_BUTTON::STICK_LEFT)) {
				++m_CursorPvP %= 2;
				cSoundData::GetInstance().Play(eSOUND::CURSOR);
			}
			cCommonData::Player4Flag = (m_CursorPvP == 1);
		}

		// 決定
		if (cInputMgr::In(eINPUT_DEVICE::ALL, eINPUT_BUTTON::BUTTON_2))
		{
			switch (m_Cursor)
			{
			case eTITLE_MENU::PVP:
				if (m_SelectedPvP) {
					cSoundData::GetInstance().Play(eSOUND::DECIDE);
					return new cPreview();
				}
				else {
					m_SelectedPvP = true;
					cSoundData::GetInstance().Play(eSOUND::DECIDE);
				}
				break;
			case eTITLE_MENU::CONFIG:
				cSoundData::GetInstance().Play(eSOUND::DECIDE);
				return new cConfig();
				break;
			}
		}

		// キャンセル
		if (cInputMgr::In(eINPUT_DEVICE::ALL, eINPUT_BUTTON::BUTTON_1))
		{
			if (m_SelectedPvP) {
				m_SelectedPvP = false;
				cSoundData::GetInstance().Play(eSOUND::CANCEL);
			}
		}
	}

	return this;
}

/* 描画 */
void cTitle::Draw() const
{
	// タイトル
	for (int i = 0; i < 7; ++i) {
		if (m_StartTimer < 40 + i * 6 || m_StartTimer >= 70 + i * 6)
			continue;
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255 - (m_StartTimer - 40 - i * 6) * 255 / 30);
		DrawRotaGraph(m_TitlePos.x, m_TitlePos.y, 1.0, 0.0, m_Image_TitleText[i], TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (m_StartTimer >= 90) {
		int param = 200 - (m_StartTimer - 90) * 200 / 60;
		SetDrawAddColor(param, param, param);
		DrawRotaGraph(m_TitlePos.x, m_TitlePos.y, 1.0, 0.0, m_Image_Title, TRUE);
		SetDrawAddColor(0, 0, 0);
	}

	// メニュー
	if (m_StartTimer >= 90) {
		float tmp_bright = (sinf(DX_PI_F * 2.0f * m_Timer / m_MaxTime) + 1.0f) / 2.0f;
		for (int i = 0; i < (int)eTITLE_MENU::MAX; ++i) {
			(i == (int)m_Cursor) ?
				SetDrawAddColor((int)(tmp_bright * 50.0f), (int)(tmp_bright * 50.0f), 0) :
				SetDrawAddColor(0, 0, 0);
			switch (i) {
			case (int)eTITLE_MENU::PVP:		DrawRotaGraph(Sw / 2 - 120, 380, 1.0, 0.0, m_Image_Menu[i], TRUE);	break;
			case (int)eTITLE_MENU::CONFIG:	DrawRotaGraph(Sw / 2 + 210, 380, 1.0, 0.0, m_Image_Menu[i], TRUE);	break;
			}
		}
		SetDrawAddColor(0, 0, 0);
	}

	// PvP
	if (m_SelectedPvP) {
		float tmp_bright = (sinf(DX_PI_F * 2.0f * m_Timer / m_MaxTime) + 1.0f) / 2.0f;
		SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
		DrawBox(0, 0, Sw, Sh, 0x808080, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		(m_CursorPvP == 0) ?
			SetDrawAddColor((int)(tmp_bright * 50.0f), (int)(tmp_bright * 50.0f), 0) :
			SetDrawAddColor(0, 0, 0);
		DrawBox(Sw / 2 - 320, 200, Sw / 2 - 40, 420, 0x000000, TRUE);
		(m_CursorPvP == 1) ?
			SetDrawAddColor((int)(tmp_bright * 50.0f), (int)(tmp_bright * 50.0f), 0) :
			SetDrawAddColor(0, 0, 0);
		DrawBox(Sw / 2 + 40, 200, Sw / 2 + 320, 420, 0x000000, TRUE);
		SetDrawAddColor(0, 0, 0);
		DrawBox(Sw / 2 - 320, 200, Sw / 2 - 40, 420, GetColor(255, 255, (m_CursorPvP != 0) * 255), FALSE);
		DrawBox(Sw / 2 + 40, 200, Sw / 2 + 320, 420, GetColor(255, 255, (m_CursorPvP != 1) * 255), FALSE);
		DrawString(Sw / 2 - 320 + 8, 200 + 8, "1 vs 1", GetColor(255, 255, (m_CursorPvP != 0) * 255));
		DrawString(Sw / 2 + 40 + 8, 200 + 8, "1 vs 1 vs 1 vs 1", GetColor(255, 255, (m_CursorPvP != 1) * 255));
	}

	// 開始演出
	if (m_StartTimer >= 90 && m_StartTimer < m_MaxStartTime) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255 - (m_StartTimer - 90) * 255 / 60);
		DrawBox(0, 0, Sw, Sh, 0x808080, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	DrawString(0, 0, "[Title]", 0xffffff);
}
