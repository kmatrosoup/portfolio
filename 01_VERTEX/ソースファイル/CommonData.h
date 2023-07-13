#pragma once

#include "System.h"

enum class eINPUT_DEVICE;
struct sPlayerInitData;
enum class eSTATUS;
enum class eBULLET;
enum class eSKILL;
enum class eABILITY;

class cCommonData
{
	/* 公開関数 */
public:
	static void Init();	// 初期化
	static void Fin();	// 終了

	static int GetIcon_Status(eSTATUS type);	// アイコンの画像ハンドルを取得・ステータス
	static int GetIcon_Bullet(eBULLET type);	// アイコンの画像ハンドルを取得・弾
	static int GetIcon_Skill(eSKILL type);		// アイコンの画像ハンドルを取得・スキル
	static int GetIcon_Ability(eABILITY type);	// アイコンの画像ハンドルを取得・能力

	/* 非公開関数 */
private:
	cCommonData() = delete;
	~cCommonData() = delete;
	::DISALLOW_COPY_CONSTRUCTOR(cCommonData);
	::DISALLOW_MOVE_CONSTRUCTOR(cCommonData);

	/* 公開変数 */
public:
	static eINPUT_DEVICE InputDevice[4];		// 入力デバイスID
	static int FieldID;							// フィールドID
	static int Ranking[4];						// プレイヤー順位
	static bool Player4Flag;					// 4人モードフラグ
	static sPlayerInitData PlayerInitData[4];	// プレイヤー初期化情報

	/* 非公開変数 */
private:
	static int m_Image_Icon_Status[];	// 画像ハンドル・アイコン・ステータス
	static int m_Image_Icon_Bullet[];	// 画像ハンドル・アイコン・弾
	static int m_Image_Icon_Skill[];	// 画像ハンドル・アイコン・スキル
	static int m_Image_Icon_Ability[];	// 画像ハンドル・アイコン・能力
};
