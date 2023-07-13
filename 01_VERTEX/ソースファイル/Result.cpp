//////////////////////
//   プリプロセス   //
//////////////////////
#include "Result.h"
#include "DxLib.h"
#include "System.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "CommonData.h"
#include "SoundData.h"
#include "BgmData.h"
#include "Title.h"
#include "Player.h"

#include <cmath>

#include "Input.h"
using namespace std;

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const int cResult::AlphaBlend(180);	//　アルファ値の数値

////////////////////
//   非公開関数   //
////////////////////

/**
* @param (int x　　　　　　　　) X軸
* @param (int y　　　　　　　　) Y軸
* @param (float Magnification　) 拡大率
* @param (int BulletIconGraph　) 選択された銃の画像
* @param (int SkillGraph　　　　) 選択されたスキルの画像
* @param (int AbilityIconGraph　) 選択されたアビリティの画像
* @param (eRank rank　　　　　) プレイヤーのランキング
*
*/
void DrawStatus(int x, int y, float Magnification, int BulletIconGraph, int SkillGraph, int AbilityIconGraph, eRank rank)
{

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if ((int)rank < 0 || (int)rank >= 4)
		return;

	/* 変数宣言 */
	static const int StatusPoint[(int)eSTATUS::MAX] = { 10, 3, 5,10 };
	static const int StatusPos[(int)eSTATUS::MAX] = { 15, 52, 31, 15 };  //	StatusPos[x]　＝　156　/　StatusPoint[x]

	static const int Space = 25;

	static const string StatusString[(int)eSTATUS::MAX] =
	{
		"体力",
		"攻撃",
		"発射間隔",
		"総弾数"
	};

	auto DrawStatusBar = [=](int y2, int i, unsigned int SP_Color, eSTATUS p_status)
	{
		DrawBox(x + (StatusPos[(int)p_status] * i + 4) * Magnification, y + y2, x + (StatusPos[(int)p_status] * (i + 1)) * Magnification, y + 6 + y2, SP_Color, true);
	};

	auto DrawStatusBox = [=](int y2, unsigned int Color)
	{
		DrawBox(x + (int)(4 / 3 * Magnification), y + y2 - 6, x + (int)(160 * Magnification) - (int)(6 / 3 * Magnification), y + y2 - 3, Color, true);									//	上棒

		DrawBox(x + (int)(4 / 3 * Magnification), y + y2 + 9, x + (int)(160 * Magnification) - (int)(6 / 3 * Magnification), y + y2 + 12, Color, true);									//	下棒

		DrawBox(x + (int)(4 / 3 * Magnification), y + y2 - 6, x + (int)(4 / 3 * Magnification) + 3, y + y2 + 12, Color, true);															//	左横棒

		DrawBox(x + (160 * Magnification) - (int)(6 / 3 * Magnification), y + y2 - 6, x + (int)(160 * Magnification) - (int)(6 / 3 * Magnification) + 4, y + y2 + 12, Color, true);		//	右横棒
	};



	/* 処理 */

	{
		for (auto i = 0; i < 4; ++i)
			DrawFormatString(x - 70, y + Space * i - 5, GetColor(255, 255, 255), "%s", StatusString[i].c_str());
	}

	// HP描画
	{
		for (auto i = 0; i < cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].hp; ++i)
			DrawStatusBar(0 * Space, i, 0xffffff00, eSTATUS::HP);
		for (int i = cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].hp; i < StatusPoint[(int)eSTATUS::HP]; ++i)
			DrawStatusBar(0 * Space, i, 0xff2f2f2f, eSTATUS::HP);

		DrawStatusBox(0 * Space, 0xffffffff);
	}
	//	ATK描画
	{
		for (auto i = 0; i < cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].atk; ++i)
			DrawStatusBar(1 * Space, i, 0xffffff00, eSTATUS::ATK);
		for (int i = cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].atk; i < StatusPoint[(int)eSTATUS::ATK]; ++i)
			DrawStatusBar(1 * Space, i, 0xff2f2f2f, eSTATUS::ATK);
		DrawStatusBox(1 * Space, 0xffffffff);
	}
	//	発射間隔描画
	{
		for (auto i = 0; i < cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].interval; ++i)
			DrawStatusBar(2 * Space, i, 0xffffff00, eSTATUS::INTERVAL);
		for (int i = cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].interval; i < StatusPoint[(int)eSTATUS::INTERVAL]; ++i)
			DrawStatusBar(2 * Space, i, 0xff2f2f2f, eSTATUS::INTERVAL);
		DrawStatusBox(2 * Space, 0xffffffff);
	}
	//	総弾数描画
	{
		for (auto i = 0; i < cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].bulletnum; ++i)
			DrawStatusBar(3 * Space, i, 0xffffff00, eSTATUS::BULLET);
		for (int i = cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].bulletnum; i < StatusPoint[(int)eSTATUS::BULLET]; ++i)
			DrawStatusBar(3 * Space, i, 0xff2f2f2f, eSTATUS::BULLET);
		DrawStatusBox(3 * Space, 0xffffffff);
	}
	//  弾
	{
		if (cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].bullet != eBULLET::MAX)
			DrawRotaGraph(
				x + 30,
				y + 4 * Space + 6 * Magnification,
				Magnification, 0.0, BulletIconGraph, true, false);
	}
	//  スキル
	{
		if (cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].skill != eSKILL::MAX)
			DrawRotaGraph(
				x + (40 * Magnification + 30),
				y + 4 * Space + 6 * Magnification,
				Magnification, 0.0, SkillGraph, true, false);
	}
	//　アビリティ
	{
		if (cCommonData::PlayerInitData[cCommonData::Ranking[(int)rank]].ability != eABILITY::MAX)
			DrawRotaGraph(
				x + (80 * Magnification + 30),
				y + 4 * Space + 6 * Magnification,
				Magnification, 0.0, AbilityIconGraph, true, false);
	}
}

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cResult::cResult()
{
	MedalGraph[0] =
	{
		LoadGraph("data\\image\\kinnmedal.png")
	};

	LoadDivGraph("data\\image\\player\\player.png", 8 * 3, 3, 8, 24, 24, PlayerGraph);
	LoadDivGraph("data\\image\\player\\player_color.png", 8 * 3, 3, 8, 24, 24, PlayerColorGraph);

	for (int i = 1; i <= cCommonData::Player4Flag * 2 + 1; ++i)
		MedalGraph[i] = { LoadGraph("data\\image\\sirokuromedal.png") };

	// BGMの再生
	cBgmData::GetInstance().Play(eBGM::RESULT, false);
}

/* デストラクタ */
cResult::~cResult()
{
}

/* 更新 */
cBaseScene* cResult::Update()
{
	if (Input::Keyboard::In(KEY_INPUT_Z))
		return new cTitle();

	if (cInputMgr::In(eINPUT_DEVICE::ALL, eINPUT_BUTTON::BUTTON_2))
		return new cTitle();

	return this;
}

/* 描画 */
void cResult::Draw() const
{
	//　TO DO
	auto DrawCharacter = [=](int x, int y, int rank, bool scal_flag)
	{
		int CharactorSize = 5 + !scal_flag * 3;

		int MedalSize = 0.5 + !scal_flag * 0.25;

		DrawRotaGraph(x, y, MedalSize, 0, MedalGraph[rank], true, false);

		DrawRotaGraph(x, y, CharactorSize, 0, PlayerGraph[3 * 3 - 2], true, false);

		SetDrawAddColor((PlayerColor[rank] >> 16) % 256, (PlayerColor[rank] >> 8) % 256, PlayerColor[rank] % 256);

		DrawRotaGraph(x, y, CharactorSize, 0, PlayerColorGraph[3 * 3 - 2], true, false);

		SetDrawAddColor(0, 0, 0);
	};

	//　２人対戦
	if (!cCommonData::Player4Flag)
	{
		for (auto i = 0; i < 2; ++i)
		{


			SetDrawBlendMode(DX_BLENDMODE_ALPHA, AlphaBlend);

			DrawBox(0, BoxPos_H * i, Sw, BoxPos_H * (i + 1), PlayerColor[cCommonData::Ranking[i]], TRUE);

			DrawStatus
			(
				Sh - Sh / 4 + 60,
				BoxPos_H * i + 60,
				3,
				cCommonData::GetIcon_Bullet(cCommonData::PlayerInitData[cCommonData::Ranking[i]].bullet),
				cCommonData::GetIcon_Skill(cCommonData::PlayerInitData[cCommonData::Ranking[i]].skill),
				cCommonData::GetIcon_Ability(cCommonData::PlayerInitData[cCommonData::Ranking[i]].ability),
				(eRank)i
			);

			DrawCharacter
			(
				Sw / 5,
				BoxPos_H * i + Sh / 4,
				cCommonData::Ranking[i],
				FALSE
			);
		}
	}

	//　４人対戦
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, AlphaBlend);
		// 一位
		DrawBox
		(
			0,
			0,
			BoxPos_W * 2,
			BoxPos_H,
			PlayerColor[cCommonData::Ranking[(int)eRank::ONE]],
			TRUE
		);

		DrawStatus
		(
			Sh - Sh / 4 + 60,
			50,
			2,
			cCommonData::GetIcon_Bullet(cCommonData::PlayerInitData[cCommonData::Ranking[(int)eRank::ONE]].bullet),
			cCommonData::GetIcon_Skill(cCommonData::PlayerInitData[cCommonData::Ranking[(int)eRank::ONE]].skill),
			cCommonData::GetIcon_Ability(cCommonData::PlayerInitData[cCommonData::Ranking[(int)eRank::ONE]].ability),
			eRank::ONE
		);

		DrawCharacter(Sw / 5, Sh / 4, (int)eRank::ONE, FALSE);
		// それ以外
		for (auto i = 1; i < 4; ++i)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, AlphaBlend);

			DrawBox
			(
				Sw / 3 * (i - 1),
				BoxPos_H, Sw / 3 * i,
				BoxPos_H * 2,
				PlayerColor[cCommonData::Ranking[i]],
				TRUE
			);

			DrawStatus
			(
				Sw / 3 * (i - 1) + (Sw / 24) + 30,
				BoxPos_H + (BoxPos_H / 2),
				1.5,
				cCommonData::GetIcon_Bullet(cCommonData::PlayerInitData[cCommonData::Ranking[i]].bullet),
				cCommonData::GetIcon_Skill(cCommonData::PlayerInitData[cCommonData::Ranking[i]].skill),
				cCommonData::GetIcon_Ability(cCommonData::PlayerInitData[cCommonData::Ranking[i]].ability),
				(eRank)i
			);

			DrawCharacter
			(
				(Sw / 3 * i) - Sw / 9,
				BoxPos_H + Sw / 18,
				cCommonData::Ranking[i],
				TRUE
			);
		}

	}

	DrawBox(0, Sw, 0, Sh, 0xffffffff, TRUE);
	DrawString(0, 0, "[Result]", 0xffffff);
}
