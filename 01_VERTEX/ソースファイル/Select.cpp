//////////////////////
//   プリプロセス   //
//////////////////////
#include "Select.h"
#include "DxLib.h"
#include "System.h"
#include "SceneMgr.h"
#include "Game.h"
#include "CommonData.h"
#include "SoundData.h"
#include "Easing.h"
#include <cmath>

#include "Input.h"
using namespace std;

////////////////////
//   定数の定義   //
////////////////////

#define MAX_MENU_HCOUNT			(2)		// メニュー縦の最大数
#define MAX_CELL_NUM			(10)	// ポイントの最大値
#define LONGHOLD_START			(16)	// 長押し判定時間
#define LONGHOLD_INTERVAL		(4)		// 長押し判定間隔
#define CUSTOM_AREA_WIDTH		(220)	// カスタマイズ領域・幅
#define CUSTOM_AREA_HEIGHT		(200)	// カスタマイズ領域・高さ
#define CUSTOM_AREA_SPACE		(20)	// カスタマイズ領域・間隔
#define LORE_AREA_WIDTH			(456)	// アイコン説明領域・幅
#define LORE_AREA_HEIGHT_2		(120)	// アイコン説明領域・高さ(2人モード)
#define LORE_AREA_HEIGHT_4		(56)	// アイコン説明領域・高さ(4人モード)
#define GAUGE_WIDTH_HP			(15)	// ゲージ・幅・体力
#define GAUGE_WIDTH_ATK			(50)	// ゲージ・幅・攻撃力
#define GAUGE_WIDTH_INTERVAL	(30)	// ゲージ・幅・発射間隔
#define GAUGE_WIDTH_BULLET		(15)	// ゲージ・幅・弾数
#define GAUGE_SPACE				(2)		// ゲージ・間隔
#define ITEM_X					(102)	// 項目・始点のX座標
#define ITEM_SIZE				(54)	// 項目・大きさ
#define MAX_START_TIME			(200)	// 最大開始時間
#define MAX_BRIGHT_TIME			(60)	// 最大明滅時間
#define MAX_ENTRY_TIME			(60)	// 最大入場時間

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const vector<cSelect::sStatus> cSelect::m_Menu[(int)eMENU_TYPE::MAX]	// メニュー
#if NEEDLESS_PPOINT_MODE
{
	{
		sStatus(vector<int>({ 0 }),			10,	"耐久力",				"お前をタフにしてやる"),
		sStatus(vector<int>({ 0 }),			3,	"攻撃力",				"弾の威力を上げてやるぜ"),
		sStatus(vector<int>({ 0 }),			5,	"発射間隔",				"連射したいならおオススメだ"),
		sStatus(vector<int>({ 0 }),			10,	"弾数",					"弾の数を増やしてやるぜ"),
	},
	{
		sStatus(vector<int>({ 0 }),			1,	"通常弾",				"エネルギーを凝縮した一般的な弾だ"),
		sStatus(vector<int>({ 0 }),			1,	"チャージショット",		"強さには代償が付き物だ"),
		sStatus(vector<int>({ 0 }),			1,	"レーザー",				"敵に風穴を開けてやれ"),
		sStatus(vector<int>({ 0 }),			1,	"ミサイル",				"周囲に爆風を起こし敵を一掃する"),
	},
	{
		sStatus(vector<int>({ 0 }),			1,	"ブースト",				"敵の隙をついてスタイリッシュに決めろ"),
		sStatus(vector<int>({ 0 }),			1,	"リフレクト",			"防御こそ最大の攻撃だ"),
		sStatus(vector<int>({ 0 }),			1,	"電気トラップ",			"動き回る奴にはこいつがおすすめだぜ"),
		sStatus(vector<int>({ 0 }),			1,	"地雷トラップ",			"真のハンターは足元を警戒する"),
		sStatus(vector<int>({ 0 }),			1,	"リペア",				"戦場も休憩が必要だな"),
		sStatus(vector<int>({ 0 }),			1,	"フルバースト",			"下手な鉄砲も数撃ちゃ当たるってな"),
		sStatus(vector<int>({ 0 }),			1,	"クイックリロード",		"一瞬で弾を補充する（弾は無料）"),
		sStatus(vector<int>({ 0 }),			1,	"ランダム",				"何が出るかはお楽しみだな"),
	},
	{
		sStatus(vector<int>({ 0 }),			1,	"跳弾",					"敵の死角を突く一撃を"),
		sStatus(vector<int>({ 0 }),			1,	"加護",					"ダメージからお前を護る"),
		sStatus(vector<int>({ 0 }),			1,	"逆境",					"ピンチの時こそ本来の力が出せるんだよ"),
		sStatus(vector<int>({ 0 }),			1,	"バーサーカー",			"連続で攻撃を当てろ（被弾に注意）"),
		sStatus(vector<int>({ 0 }),			1,	"リベンジャーオーブ",	"反撃の意志を形に変える"),
		sStatus(vector<int>({ 0 }),			1,	"デス",					"最強で最恐の一撃をあなたに"),
	}
};
#else
{
	{
		sStatus(vector<int>({ 1 }),			10,	"耐久力",				"お前をタフにしてやる"),
		sStatus(vector<int>({ 2, 3, 5 }),	3,	"攻撃力",				"弾の威力を上げてやるぜ"),
		sStatus(vector<int>({ 2 }),			5,	"発射間隔",				"連射したいならおオススメだ"),
		sStatus(vector<int>({ 1 }),			10,	"弾数",					"弾の数を増やしてやるぜ"),
	},
	{
		sStatus(vector<int>({ 0 }),			1,	"通常弾",				"エネルギーを凝縮した一般的な弾だ"),
		sStatus(vector<int>({ 4 }),			1,	"チャージショット",		"強さには代償が付き物だ"),
		sStatus(vector<int>({ 4 }),			1,	"レーザー",				"敵に風穴を開けてやれ"),
		sStatus(vector<int>({ 5 }),			1,	"ミサイル",				"周囲に爆風を起こし敵を一掃する"),
	},
	{
		sStatus(vector<int>({ 2 }),			1,	"ブースト",				"敵の隙をついてスタイリッシュに決めろ"),
		sStatus(vector<int>({ 3 }),			1,	"リフレクト",			"防御こそ最大の攻撃だ"),
		sStatus(vector<int>({ 4 }),			1,	"電気トラップ",			"動き回る奴にはこいつがおすすめだぜ"),
		sStatus(vector<int>({ 4 }),			1,	"地雷トラップ",			"真のハンターは足元を警戒する"),
		sStatus(vector<int>({ 4 }),			1,	"リペア",				"戦場も休憩が必要だな"),
		sStatus(vector<int>({ 4 }),			1,	"フルバースト",			"下手な鉄砲も数撃ちゃ当たるってな"),
		sStatus(vector<int>({ 2 }),			1,	"クイックリロード",		"一瞬で弾を補充する（弾は無料）"),
		sStatus(vector<int>({ 3 }),			1,	"ランダム",				"何が出るかはお楽しみだな"),
	},
	{
		sStatus(vector<int>({ 4 }),			1,	"跳弾",					"敵の死角を突く一撃を"),
		sStatus(vector<int>({ 3 }),			1,	"加護",					"ダメージからお前を護る"),
		sStatus(vector<int>({ 4 }),			1,	"逆境",					"ピンチの時こそ本来の力が出せるんだよ"),
		sStatus(vector<int>({ 5 }),			1,	"バーサーカー",			"連続で攻撃を当てろ（被弾に注意）"),
		sStatus(vector<int>({ 4 }),			1,	"リベンジャーオーブ",	"反撃の意志を形に変える"),
		sStatus(vector<int>({ 10 }),		1,	"デス",					"最強で最恐の一撃をあなたに"),
	}
};
#endif
const eINPUT_BUTTON cSelect::m_ButtonHandle[(int)eACTIVE_BUTTON::MAX]	// 対応するボタン
{
	eINPUT_BUTTON::STICK_UP,
	eINPUT_BUTTON::STICK_DOWN,
	eINPUT_BUTTON::STICK_LEFT,
	eINPUT_BUTTON::STICK_RIGHT,
	eINPUT_BUTTON::BUTTON_2,
	eINPUT_BUTTON::BUTTON_1,
};

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cSelect::cSelect()
	: m_RestCell{ MAX_CELL_NUM, MAX_CELL_NUM, MAX_CELL_NUM, MAX_CELL_NUM }
	, m_CursorType{ eMENU_TYPE::STATUS, eMENU_TYPE::STATUS, eMENU_TYPE::STATUS, eMENU_TYPE::STATUS }
	, m_CursorPos{ 0, 0, 0, 0 }
	, m_SelectedBullet{ eBULLET::NORMAL, eBULLET::NORMAL, eBULLET::NORMAL, eBULLET::NORMAL }
	, m_SelectedSkill{ eSKILL::MAX, eSKILL::MAX, eSKILL::MAX, eSKILL::MAX }
	, m_SelectedAbility{ eABILITY::MAX, eABILITY::MAX, eABILITY::MAX, eABILITY::MAX }
	, m_HoldButton{ eACTIVE_BUTTON::MAX, eACTIVE_BUTTON::MAX, eACTIVE_BUTTON::MAX, eACTIVE_BUTTON::MAX }
	, m_HoldTime{ -1, -1, -1, -1 }
	, m_Ready{ false, false, false, false }
	, m_BrightTimer(0)
	, m_StartTimer(0)
	, m_EntryTimer(-1)
{
	// 画像の読み込み
	m_Image_Cell = LoadGraph("data\\image\\cell.png");
	m_Image_IconFrame = LoadGraph("data\\image\\icon\\icon_frame.png");
	m_Image_IconCursor = LoadGraph("data\\image\\icon\\icon_cursor.png");
	m_Image_Entry = LoadGraph("data\\image\\select\\entry.png");

	// フォントの生成
	m_Font_Name = CreateFontToHandle("ＭＳ ゴシック", 32, 0);
	m_Font_Lore = CreateFontToHandle("ＭＳ ゴシック", 24, 0);
}

/* デストラクタ */
cSelect::~cSelect()
{
	// 画像の解放
	DeleteGraph(m_Image_Cell);
	DeleteGraph(m_Image_IconFrame);
	DeleteGraph(m_Image_IconCursor);
	DeleteGraph(m_Image_Entry);

	// フォントの解放
	DeleteFontToHandle(m_Font_Name);
	DeleteFontToHandle(m_Font_Lore);
}

/* 更新 */
cBaseScene* cSelect::Update()
{
	// タイマー制御
	++m_BrightTimer %= MAX_BRIGHT_TIME;
	if (m_StartTimer < MAX_START_TIME)
		++m_StartTimer;
	if (m_EntryTimer != -1 && m_EntryTimer < MAX_ENTRY_TIME)
		++m_EntryTimer;

	// 操作
	if (m_EntryTimer == -1) {
		for (int pi = 0; pi < (cCommonData::Player4Flag ? 4 : 2); ++pi)
		{
			// 長押し判定
			{
				for (int bi = 0; bi < (int)eACTIVE_BUTTON::MAX; ++bi) {
					if (cInputMgr::In(cCommonData::InputDevice[pi], m_ButtonHandle[bi])) {
						m_HoldButton[pi] = (eACTIVE_BUTTON)bi;
						m_HoldTime[pi] = -1;
					}
				}
				if (m_HoldButton[pi] != eACTIVE_BUTTON::MAX) {
					if (cInputMgr::Keep(cCommonData::InputDevice[pi], m_ButtonHandle[(int)m_HoldButton[pi]]))
						++m_HoldTime[pi];
					if (cInputMgr::Out(cCommonData::InputDevice[pi], m_ButtonHandle[(int)m_HoldButton[pi]])) {
						m_HoldButton[pi] = eACTIVE_BUTTON::MAX;
						m_HoldTime[pi] = -1;
					}
				}
			}

			// カーソル移動
			{
				bool move_flag = false;
				// 右移動
				if (m_HoldButton[pi] == eACTIVE_BUTTON::RIGHT && (m_HoldTime[pi] == 0 || (m_HoldTime[pi] >= LONGHOLD_START && (m_HoldTime[pi] - LONGHOLD_START) % LONGHOLD_INTERVAL == 0)))
				{
					m_CursorPos[pi] += MAX_MENU_HCOUNT;
					if (m_CursorPos[pi] / MAX_MENU_HCOUNT >= ((int)m_Menu[(int)m_CursorType[pi]].size() + MAX_MENU_HCOUNT - 1) / MAX_MENU_HCOUNT) {
						m_CursorPos[pi] %= MAX_MENU_HCOUNT;
						m_CursorType[pi] = (eMENU_TYPE)(((int)m_CursorType[pi] + 1) % (int)eMENU_TYPE::MAX);
					}
					if (m_CursorPos[pi] >= (int)m_Menu[(int)m_CursorType[pi]].size())
						m_CursorPos[pi] = (int)m_Menu[(int)m_CursorType[pi]].size() - 1;
					move_flag = true;
				}
				// 左移動
				if (m_HoldButton[pi] == eACTIVE_BUTTON::LEFT && (m_HoldTime[pi] == 0 || (m_HoldTime[pi] >= LONGHOLD_START && (m_HoldTime[pi] - LONGHOLD_START) % LONGHOLD_INTERVAL == 0)))
				{
					m_CursorPos[pi] -= MAX_MENU_HCOUNT;
					if (m_CursorPos[pi] < 0) {
						m_CursorType[pi] = (eMENU_TYPE)(((int)m_CursorType[pi] + (int)eMENU_TYPE::MAX - 1) % (int)eMENU_TYPE::MAX);
						m_CursorPos[pi] += (((int)m_Menu[(int)m_CursorType[pi]].size() + MAX_MENU_HCOUNT - 1) / MAX_MENU_HCOUNT) * MAX_MENU_HCOUNT;
					}
					if (m_CursorPos[pi] >= (int)m_Menu[(int)m_CursorType[pi]].size())
						m_CursorPos[pi] = (int)m_Menu[(int)m_CursorType[pi]].size() - 1;
					move_flag = true;
				}
				// 下移動
				if (m_HoldButton[pi] == eACTIVE_BUTTON::DOWN && (m_HoldTime[pi] == 0 || (m_HoldTime[pi] >= LONGHOLD_START && (m_HoldTime[pi] - LONGHOLD_START) % LONGHOLD_INTERVAL == 0)))
				{
					(m_CursorPos[pi] % MAX_MENU_HCOUNT == MAX_MENU_HCOUNT - 1 || m_CursorPos[pi] == (int)m_Menu[(int)m_CursorType[pi]].size() - 1) ?
						m_CursorPos[pi] = (m_CursorPos[pi] - 1) / MAX_MENU_HCOUNT * MAX_MENU_HCOUNT :
						m_CursorPos[pi] = (m_CursorPos[pi] + 1) % (int)m_Menu[(int)m_CursorType[pi]].size();
					move_flag = true;
				}
				// 上移動
				if (m_HoldButton[pi] == eACTIVE_BUTTON::UP && (m_HoldTime[pi] == 0 || (m_HoldTime[pi] >= LONGHOLD_START && (m_HoldTime[pi] - LONGHOLD_START) % LONGHOLD_INTERVAL == 0)))
				{
					(m_CursorPos[pi] % MAX_MENU_HCOUNT == 0) ?
						m_CursorPos[pi] = (m_CursorPos[pi] / MAX_MENU_HCOUNT) * MAX_MENU_HCOUNT + (MAX_MENU_HCOUNT - 1) :
						m_CursorPos[pi] = (m_CursorPos[pi] + (int)m_Menu[(int)m_CursorType[pi]].size() - 1) % (int)m_Menu[(int)m_CursorType[pi]].size();
					if (m_CursorPos[pi] >= (int)m_Menu[(int)m_CursorType[pi]].size())
						m_CursorPos[pi] = (int)m_Menu[(int)m_CursorType[pi]].size() - 1;
					move_flag = true;
				}

				if (move_flag)
					cSoundData::GetInstance().Play(eSOUND::CURSOR);
			}

			// 決定
			if (m_HoldButton[pi] == eACTIVE_BUTTON::DECIDE && (m_HoldTime[pi] == 0 || (m_HoldTime[pi] >= LONGHOLD_START && (m_HoldTime[pi] - LONGHOLD_START) % LONGHOLD_INTERVAL == 0)))
			{
				switch (m_CursorType[pi]) {
				case eMENU_TYPE::STATUS:
				{
					int tmp_count = (int)count(m_SelectedStatus[pi].begin(), m_SelectedStatus[pi].end(), m_CursorPos[pi]);
					if (tmp_count < m_Menu[(int)m_CursorType[pi]][m_CursorPos[pi]].MaxCount &&
						m_RestCell[pi] - m_Menu[(int)m_CursorType[pi]][m_CursorPos[pi]].NeedCell[__min(tmp_count, (int)m_Menu[(int)m_CursorType[pi]][m_CursorPos[pi]].NeedCell.size() - 1)] >= 0)
					{
						m_RestCell[pi] -= m_Menu[(int)m_CursorType[pi]][m_CursorPos[pi]].NeedCell[__min(tmp_count, (int)m_Menu[(int)m_CursorType[pi]][m_CursorPos[pi]].NeedCell.size() - 1)];
						m_SelectedStatus[pi].push_back(m_CursorPos[pi]);
						m_Ready[pi] = false;
						cSoundData::GetInstance().Play(eSOUND::DECIDE);
					}
					else if (m_HoldTime[pi] == 0)
						cSoundData::GetInstance().Play(eSOUND::FAIL);
				}
				break;
				case eMENU_TYPE::BULLET:
					if (m_CursorPos[pi] != (int)m_SelectedBullet[pi])
					{
						int cell_prev = m_RestCell[pi] + m_Menu[(int)m_CursorType[pi]][(int)m_SelectedBullet[pi]].NeedCell[0] - m_Menu[(int)m_CursorType[pi]][m_CursorPos[pi]].NeedCell[0];
						if (cell_prev >= 0) {
							m_RestCell[pi] = cell_prev;
							m_SelectedBullet[pi] = (eBULLET)m_CursorPos[pi];
							m_Ready[pi] = false;
							cSoundData::GetInstance().Play(eSOUND::DECIDE);
						}
						else if (m_HoldTime[pi] == 0)
							cSoundData::GetInstance().Play(eSOUND::FAIL);
					}
					else if (m_HoldTime[pi] == 0)
						cSoundData::GetInstance().Play(eSOUND::FAIL);
					break;
				case eMENU_TYPE::SKILL:
					if (m_CursorPos[pi] != (int)m_SelectedSkill[pi])
					{
						int cell_prev = m_RestCell[pi] - m_Menu[(int)m_CursorType[pi]][m_CursorPos[pi]].NeedCell[0];
						if (m_SelectedSkill[pi] != eSKILL::MAX)
							cell_prev += m_Menu[(int)m_CursorType[pi]][(int)m_SelectedSkill[pi]].NeedCell[0];
						if (cell_prev >= 0) {
							m_RestCell[pi] = cell_prev;
							m_SelectedSkill[pi] = (eSKILL)m_CursorPos[pi];
							m_Ready[pi] = false;
							cSoundData::GetInstance().Play(eSOUND::DECIDE);
						}
						else if (m_HoldTime[pi] == 0)
							cSoundData::GetInstance().Play(eSOUND::FAIL);
					}
					else if (m_HoldTime[pi] == 0)
						cSoundData::GetInstance().Play(eSOUND::FAIL);
					break;
				case eMENU_TYPE::ABILITY:
					if (m_CursorPos[pi] != (int)m_SelectedAbility[pi])
					{
						int cell_prev = m_RestCell[pi] - m_Menu[(int)m_CursorType[pi]][m_CursorPos[pi]].NeedCell[0];
						if (m_SelectedAbility[pi] != eABILITY::MAX)
							cell_prev += m_Menu[(int)m_CursorType[pi]][(int)m_SelectedAbility[pi]].NeedCell[0];
						if (cell_prev >= 0) {
							m_RestCell[pi] = cell_prev;
							m_SelectedAbility[pi] = (eABILITY)m_CursorPos[pi];
							m_Ready[pi] = false;
							cSoundData::GetInstance().Play(eSOUND::DECIDE);
						}
						else if (m_HoldTime[pi] == 0)
							cSoundData::GetInstance().Play(eSOUND::FAIL);
					}
					else if (m_HoldTime[pi] == 0)
						cSoundData::GetInstance().Play(eSOUND::FAIL);
					break;
				}
			}

			// キャンセル
			if (m_HoldButton[pi] == eACTIVE_BUTTON::CANCEL && (m_HoldTime[pi] == 0 || (m_HoldTime[pi] >= LONGHOLD_START && (m_HoldTime[pi] - LONGHOLD_START) % LONGHOLD_INTERVAL == 0)))
			{
				if (!m_Ready[pi]) {
					switch (m_CursorType[pi]) {
					case eMENU_TYPE::STATUS:
						if (!m_SelectedStatus[pi].empty())
						{
							int tmp_count = (int)count(m_SelectedStatus[pi].begin(), m_SelectedStatus[pi].end(), m_SelectedStatus[pi].back());
							m_RestCell[pi] += m_Menu[(int)m_CursorType[pi]][m_SelectedStatus[pi].back()].NeedCell[__min(__max(tmp_count - 1, 0), (int)m_Menu[(int)m_CursorType[pi]][m_SelectedStatus[pi].back()].NeedCell.size() - 1)];
							m_SelectedStatus[pi].pop_back();
							m_Ready[pi] = false;
							cSoundData::GetInstance().Play(eSOUND::CANCEL);
						}
						break;
					case eMENU_TYPE::BULLET:
						break;
					case eMENU_TYPE::SKILL:
						if (m_SelectedSkill[pi] != eSKILL::MAX)
						{
							m_RestCell[pi] += m_Menu[(int)m_CursorType[pi]][(int)m_SelectedSkill[pi]].NeedCell[0];
							m_SelectedSkill[pi] = eSKILL::MAX;
							m_Ready[pi] = false;
							cSoundData::GetInstance().Play(eSOUND::CANCEL);
						}
						break;
					case eMENU_TYPE::ABILITY:
						if (m_SelectedAbility[pi] != eABILITY::MAX)
						{
							m_RestCell[pi] += m_Menu[(int)m_CursorType[pi]][(int)m_SelectedAbility[pi]].NeedCell[0];
							m_SelectedAbility[pi] = eABILITY::MAX;
							m_Ready[pi] = false;
							cSoundData::GetInstance().Play(eSOUND::CANCEL);
						}
						break;
					}
				}
				else {
					m_Ready[pi] = false;
					cSoundData::GetInstance().Play(eSOUND::CANCEL);
				}
			}

			// 準備完了
			if (cInputMgr::In(cCommonData::InputDevice[pi], eINPUT_BUTTON::BUTTON_R))
			{
				if (!m_Ready[pi]) {
					m_Ready[pi] = true;
					cSoundData::GetInstance().Play(eSOUND::READY);
					bool flag = true;
					for (int pi_other = 0; pi_other < 2 + cCommonData::Player4Flag * 2; ++pi_other)
						flag &= m_Ready[pi_other];
					if (flag)
						cSoundData::GetInstance().Play(eSOUND::GAMESTART);
				}
				else {
					m_Ready[pi] = false;
					cSoundData::GetInstance().Play(eSOUND::CANCEL);
				}
			}
		}
		bool flag = true;
		for (int pi = 0; pi < (cCommonData::Player4Flag ? 4 : 2); ++pi)
			flag &= m_Ready[pi];
		if (flag)
			m_EntryTimer = 0;
	}

	// 戦闘シーンへ移行
	if (m_EntryTimer >= MAX_ENTRY_TIME || Input::Keyboard::In(KEY_INPUT_Z)) {
		bool next_flag = true;
		for (int i = 0; i < (cCommonData::Player4Flag ? 4 : 2); ++i)
			next_flag &= m_Ready[i];
		if (Input::Keyboard::In(KEY_INPUT_Z))
			next_flag = true;
		if (next_flag) {
			for (int i = 0; i < 2 + cCommonData::Player4Flag * 2; ++i) {
				cCommonData::PlayerInitData[i].hp = (int)count(m_SelectedStatus[i].begin(), m_SelectedStatus[i].end(), 0);
				cCommonData::PlayerInitData[i].atk = (int)count(m_SelectedStatus[i].begin(), m_SelectedStatus[i].end(), 1);
				cCommonData::PlayerInitData[i].interval = (int)count(m_SelectedStatus[i].begin(), m_SelectedStatus[i].end(), 2);
				cCommonData::PlayerInitData[i].bulletnum = (int)count(m_SelectedStatus[i].begin(), m_SelectedStatus[i].end(), 3);
				cCommonData::PlayerInitData[i].bullet = m_SelectedBullet[i];
				cCommonData::PlayerInitData[i].skill = m_SelectedSkill[i];
				cCommonData::PlayerInitData[i].ability = m_SelectedAbility[i];
			}
			return new cGame();
		}
	}

	return this;
}

/* 描画 */
void cSelect::Draw() const
{
	// メニューの描画
	{
		int menu_start[4];
		menu_start[0] = 0;
		menu_start[1] = menu_start[0] + 1 + ((int)m_Menu[(int)eMENU_TYPE::STATUS].size() + MAX_MENU_HCOUNT - 1) / MAX_MENU_HCOUNT;
		menu_start[2] = menu_start[1] + 1 + ((int)m_Menu[(int)eMENU_TYPE::BULLET].size() + MAX_MENU_HCOUNT - 1) / MAX_MENU_HCOUNT;
		menu_start[3] = menu_start[2] + 1 + ((int)m_Menu[(int)eMENU_TYPE::SKILL].size() + MAX_MENU_HCOUNT - 1) / MAX_MENU_HCOUNT;
		for (int i = 0; i < (int)eMENU_TYPE::MAX; ++i)
		{
			// 文字
			DrawString(
				ITEM_X + menu_start[i] * ITEM_SIZE,
				260,
				(i != (int)eMENU_TYPE::ABILITY) ? (i != (int)eMENU_TYPE::SKILL) ? (i != (int)eMENU_TYPE::BULLET) ? "ステータス" : "弾" : "スキル" : "アビリティ", 0xffffff);
			for (int j = 0; j < (int)m_Menu[i].size(); ++j)
			{
				int x = ITEM_X + ITEM_SIZE / 2 + (menu_start[i] + j / MAX_MENU_HCOUNT) * ITEM_SIZE + (int)(Easing::Quart::In((j * 3 + i * 15 - m_StartTimer + 100) / 60.0f) * 1000.0f);
				int y = 306 + (j % MAX_MENU_HCOUNT) * ITEM_SIZE;

				// アイコン
				m_DrawIcon(x, y, (eMENU_TYPE)i, j, true);

				// カーソル
				for (int k = 0; k < 2 + cCommonData::Player4Flag * 2; ++k) {
					if (m_CursorType[k] == (eMENU_TYPE)i && m_CursorPos[k] == j) {
						switch (k) {
						case 0:
							SetDrawAddColor(200, 0, 0);
							DrawRotaGraph(x, y, 2.0, 0.0, m_Image_IconCursor, TRUE);
							break;
						case 1:
							SetDrawAddColor(0, 0, 200);
							DrawRotaGraph(x, y, 2.0, 0.0, m_Image_IconCursor, TRUE, TRUE, TRUE);
							break;
						case 2:
							SetDrawAddColor(0, 200, 0);
							DrawRotaGraph(x, y, 2.0, 0.0, m_Image_IconCursor, TRUE, TRUE);
							break;
						case 3:
							SetDrawAddColor(200, 200, 0);
							DrawRotaGraph(x, y, 2.0, 0.0, m_Image_IconCursor, TRUE, FALSE, TRUE);
							break;
						}
					}
				}
				SetDrawAddColor(0, 0, 0);
			}
		}
	}

	// カスタマイズ領域の描画
	if (!cCommonData::Player4Flag) {
		for (int i = 0; i < 2; ++i) {
			m_DrawCustomArea(
				(Sw - CUSTOM_AREA_SPACE) / 2 - CUSTOM_AREA_WIDTH + (CUSTOM_AREA_WIDTH + CUSTOM_AREA_SPACE) * i,
				(int)(Easing::Bounce::Out((float)(m_StartTimer - i * 3) / 60) * 260.0f) - 220,
				i,
				i % 2);
		}
	}
	else {
		for (int i = 0; i < 4; ++i) {
			m_DrawCustomArea(
				(Sw - CUSTOM_AREA_SPACE * 3) / 2 - CUSTOM_AREA_WIDTH * 2 + (CUSTOM_AREA_WIDTH + CUSTOM_AREA_SPACE) * i,
				(int)(Easing::Bounce::Out((float)(m_StartTimer - i * 3) / 60) * 260.0f) - 220,
				i,
				i % 2);
		}
	}

	// アイコン説明領域の描画
	if (!cCommonData::Player4Flag)
	{
		m_DrawLoreArea(Sw / 2 - LORE_AREA_WIDTH / 2 - 4, Sh - LORE_AREA_HEIGHT_2 / 2 - 20, 0);	// 1P
		m_DrawLoreArea(Sw / 2 + LORE_AREA_WIDTH / 2 + 4, Sh - LORE_AREA_HEIGHT_2 / 2 - 20, 1);	// 2P
	}
	else
	{
		m_DrawLoreArea(Sw / 2 - LORE_AREA_WIDTH / 2 - 4, Sh - LORE_AREA_HEIGHT_4 / 2 - 20 - LORE_AREA_HEIGHT_4 - 8, 0);	// 1P
		m_DrawLoreArea(Sw / 2 + LORE_AREA_WIDTH / 2 + 4, Sh - LORE_AREA_HEIGHT_4 / 2 - 20, 1);							// 2P
		m_DrawLoreArea(Sw / 2 + LORE_AREA_WIDTH / 2 + 4, Sh - LORE_AREA_HEIGHT_4 / 2 - 20 - LORE_AREA_HEIGHT_4 - 8, 2);	// 3P
		m_DrawLoreArea(Sw / 2 - LORE_AREA_WIDTH / 2 - 4, Sh - LORE_AREA_HEIGHT_4 / 2 - 20, 3);							// 4P
	}

	// 入場演出
	int param = (m_EntryTimer * 510 / MAX_ENTRY_TIME) - 255;
	SetDrawAddColor(param, param, param);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawGraph(0, 0, m_Image_Entry, FALSE);
	SetDrawAddColor(0, 0, 0);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "[Select]", 0xffffff);
}

////////////////////
//   非公開関数   //
////////////////////

/* アイコンの描画 */
void cSelect::m_DrawIcon(int x, int y, eMENU_TYPE menu, int id, bool calm_flag) const
{
	// フレーム
	if (calm_flag)
		SetDrawBright(100, 100, 100);
	DrawRotaGraph(x, y, 2.0, 0.0, m_Image_IconFrame, FALSE);
	SetDrawBright(255, 255, 255);

	// アイコン
	switch (menu) {
	break;	case eMENU_TYPE::STATUS:	DrawRotaGraph(x, y, 2.0, 0.0, cCommonData::GetIcon_Status((eSTATUS)id), TRUE);
	break;	case eMENU_TYPE::BULLET:	DrawRotaGraph(x, y, 2.0, 0.0, cCommonData::GetIcon_Bullet((eBULLET)id), TRUE);
	break;	case eMENU_TYPE::SKILL:		DrawRotaGraph(x, y, 2.0, 0.0, cCommonData::GetIcon_Skill((eSKILL)id), TRUE);
	break;	case eMENU_TYPE::ABILITY:	DrawRotaGraph(x, y, 2.0, 0.0, cCommonData::GetIcon_Ability((eABILITY)id), TRUE);
	}
}

/* カスタマイズ領域の描画 */
void cSelect::m_DrawCustomArea(int x, int y, int player, bool inversion) const
{
	int param = (int)(sinf(DX_PI_F * 2.0f * m_BrightTimer / MAX_BRIGHT_TIME) * 64.0f);

	// 枠
	{
		SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
		DrawBox(x, y, x + CUSTOM_AREA_WIDTH, y + CUSTOM_AREA_HEIGHT, 0x808080, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(x - 2, y - 2, x + CUSTOM_AREA_WIDTH + 2, y + CUSTOM_AREA_HEIGHT + 2,
			(player == 0) ? 0xff4040 : (player == 1) ? 0x4040ff : (player == 2) ? 0x40ff40 : (player == 3) ? 0xffff40 : 0xffffff, FALSE);
		DrawBox(x - 1, y - 1, x + CUSTOM_AREA_WIDTH + 1, y + CUSTOM_AREA_HEIGHT + 1, 0xffffff, FALSE);
	}

	// 残りポイント
	{
		int prev_num = m_RestCell[player];
		switch (m_CursorType[player]) {
		case eMENU_TYPE::STATUS:
			((size_t)count(m_SelectedStatus[player].begin(), m_SelectedStatus[player].end(), m_CursorPos[player]) < m_Menu[(int)eMENU_TYPE::STATUS][m_CursorPos[player]].NeedCell.size()) ?
				prev_num -= m_Menu[(int)eMENU_TYPE::STATUS][m_CursorPos[player]].NeedCell[count(m_SelectedStatus[player].begin(), m_SelectedStatus[player].end(), m_CursorPos[player])] :
				prev_num -= m_Menu[(int)eMENU_TYPE::STATUS][m_CursorPos[player]].NeedCell.back();
			break;
		case eMENU_TYPE::BULLET:
			(m_SelectedBullet[player] == eBULLET::MAX) ?
				prev_num -= m_Menu[(int)eMENU_TYPE::BULLET][m_CursorPos[player]].NeedCell[0] :
				prev_num -= m_Menu[(int)eMENU_TYPE::BULLET][m_CursorPos[player]].NeedCell[0] - m_Menu[(int)eMENU_TYPE::BULLET][(int)m_SelectedBullet[player]].NeedCell[0];
			break;
		case eMENU_TYPE::SKILL:
			(m_SelectedSkill[player] == eSKILL::MAX) ?
				prev_num -= m_Menu[(int)eMENU_TYPE::SKILL][m_CursorPos[player]].NeedCell[0] :
				prev_num -= m_Menu[(int)eMENU_TYPE::SKILL][m_CursorPos[player]].NeedCell[0] - m_Menu[(int)eMENU_TYPE::SKILL][(int)m_SelectedSkill[player]].NeedCell[0];
			break;
		case eMENU_TYPE::ABILITY:
			(m_SelectedAbility[player] == eABILITY::MAX) ?
				prev_num -= m_Menu[(int)eMENU_TYPE::ABILITY][m_CursorPos[player]].NeedCell[0] :
				prev_num -= m_Menu[(int)eMENU_TYPE::ABILITY][m_CursorPos[player]].NeedCell[0] - m_Menu[(int)eMENU_TYPE::ABILITY][(int)m_SelectedAbility[player]].NeedCell[0];
			break;
		}
		if (inversion) {
			DrawBox(x + 7, y + 7, x + 45, y + 13 + 18 * MAX_CELL_NUM, 0xffffff, TRUE);
			DrawBox(x + 9, y + 9, x + 43, y + 11 + 18 * MAX_CELL_NUM, 0x000000, TRUE);
			for (int i = 0; i < MAX_CELL_NUM; ++i) {
				if (prev_num < 0 && i < m_RestCell[player])
					SetDrawAddColor(160 + param, -80, -80);
				else if (i >= __min(prev_num, m_RestCell[player]) && i < __max(prev_num, m_RestCell[player]))
					SetDrawAddColor(param + 64, param + 64, param + 64);
				DrawBox(
					x + 11,
					y + 173 - 18 * i,
					x + 41,
					y + 173 - 18 * i + 8,
					(i < m_RestCell[player]) ? 0x00ffff : 0x404040, TRUE);
				DrawBox(
					x + 11,
					y + 173 - 18 * i + 8,
					x + 41,
					y + 173 - 18 * i + 16,
					(i < m_RestCell[player]) ? 0x00a0e0 : 0x202020, TRUE);
				SetDrawAddColor(0, 0, 0);
			}
		}
		else {
			DrawBox(x + CUSTOM_AREA_WIDTH - 45, y + 7, x + CUSTOM_AREA_WIDTH - 7, y + 13 + 18 * MAX_CELL_NUM, 0xffffff, TRUE);
			DrawBox(x + CUSTOM_AREA_WIDTH - 43, y + 9, x + CUSTOM_AREA_WIDTH - 9, y + 11 + 18 * MAX_CELL_NUM, 0x000000, TRUE);
			for (int i = 0; i < MAX_CELL_NUM; ++i) {
				if (prev_num < 0 && i < m_RestCell[player])
					SetDrawAddColor(160 + param, -80, -80);
				else if (i >= __min(prev_num, m_RestCell[player]) && i < __max(prev_num, m_RestCell[player]))
					SetDrawAddColor(param + 64, param + 64, param + 64);
				DrawBox(
					x + CUSTOM_AREA_WIDTH - 41,
					y + 173 - 18 * i,
					x + CUSTOM_AREA_WIDTH - 11,
					y + 173 - 18 * i + 16,
					(i < m_RestCell[player]) ? 0x00ffff : 0x404040, TRUE);
				DrawBox(
					x + CUSTOM_AREA_WIDTH - 41,
					y + 173 - 18 * i + 9,
					x + CUSTOM_AREA_WIDTH - 11,
					y + 173 - 18 * i + 16,
					(i < m_RestCell[player]) ? 0x00a0e0 : 0x202020, TRUE);
				SetDrawAddColor(0, 0, 0);
			}
		}
	}

	// 体力
	{
		int tmp_count = (int)count(m_SelectedStatus[player].begin(), m_SelectedStatus[player].end(), 0);
		DrawString(x + 4 + inversion * 48, y + 7, "体力", 0xffffff);
		DrawBox(x + 4 + inversion * 48, y + 24, x + 160 + inversion * 48, y + 38, 0xffffff, TRUE);
		DrawBox(x + 6 + inversion * 48, y + 26, x + 158 + inversion * 48, y + 36, 0x000000, TRUE);
		if (m_CursorType[player] == eMENU_TYPE::STATUS && m_CursorPos[player] == (int)eSTATUS::HP)
			SetDrawAddColor(param / 2 + 48, param / 2 + 48, param / 2 + 48);
		for (int i = 0; i < 10; ++i) {
			DrawBox(
				x + 8 + GAUGE_WIDTH_HP * i + inversion * 48,
				y + 28,
				x + 8 + GAUGE_WIDTH_HP * i + GAUGE_WIDTH_HP - GAUGE_SPACE + inversion * 48,
				y + 28 + 6,
				(i < tmp_count) ? 0xffff00 : 0x404040, TRUE);
		}
		SetDrawAddColor(0, 0, 0);
	}

	// 攻撃力
	{
		int tmp_count = (int)count(m_SelectedStatus[player].begin(), m_SelectedStatus[player].end(), 1);
		DrawString(x + 4 + inversion * 48, y + 41, "攻撃力", 0xffffff);
		DrawBox(x + 4 + inversion * 48, y + 58, x + 160 + inversion * 48, y + 72, 0xffffff, TRUE);
		DrawBox(x + 6 + inversion * 48, y + 60, x + 158 + inversion * 48, y + 70, 0x000000, TRUE);
		if (m_CursorType[player] == eMENU_TYPE::STATUS && m_CursorPos[player] == (int)eSTATUS::ATK)
			SetDrawAddColor(param / 2 + 48, param / 2 + 48, param / 2 + 48);
		for (int i = 0; i < 3; ++i) {
			DrawBox(
				x + 8 + GAUGE_WIDTH_ATK * i + inversion * 48,
				y + 62,
				x + 8 + GAUGE_WIDTH_ATK * i + GAUGE_WIDTH_ATK - GAUGE_SPACE + inversion * 48,
				y + 62 + 6,
				(i < tmp_count) ? 0xffff00 : 0x404040, TRUE);
		}
		SetDrawAddColor(0, 0, 0);
	}

	// 発射間隔
	{
		int tmp_count = (int)count(m_SelectedStatus[player].begin(), m_SelectedStatus[player].end(), 2);
		DrawString(x + 4 + inversion * 48, y + 75, "発射間隔", 0xffffff);
		DrawBox(x + 4 + inversion * 48, y + 92, x + 160 + inversion * 48, y + 106, 0xffffff, TRUE);
		DrawBox(x + 6 + inversion * 48, y + 94, x + 158 + inversion * 48, y + 104, 0x000000, TRUE);
		if (m_CursorType[player] == eMENU_TYPE::STATUS && m_CursorPos[player] == (int)eSTATUS::INTERVAL)
			SetDrawAddColor(param / 2 + 48, param / 2 + 48, param / 2 + 48);
		for (int i = 0; i < 5; ++i) {
			DrawBox(
				x + 8 + GAUGE_WIDTH_INTERVAL * i + inversion * 48,
				y + 96,
				x + 8 + GAUGE_WIDTH_INTERVAL * i + GAUGE_WIDTH_INTERVAL - GAUGE_SPACE + inversion * 48,
				y + 96 + 6,
				(i < tmp_count) ? 0xffff00 : 0x404040, TRUE);
		}
		SetDrawAddColor(0, 0, 0);
	}

	// 弾数
	{
		int tmp_count = (int)count(m_SelectedStatus[player].begin(), m_SelectedStatus[player].end(), 3);
		DrawString(x + 4 + inversion * 48, y + 109, "弾数", 0xffffff);
		DrawBox(x + 4 + inversion * 48, y + 126, x + 160 + inversion * 48, y + 140, 0xffffff, TRUE);
		DrawBox(x + 6 + inversion * 48, y + 128, x + 158 + inversion * 48, y + 138, 0x000000, TRUE);
		if (m_CursorType[player] == eMENU_TYPE::STATUS && m_CursorPos[player] == (int)eSTATUS::BULLET)
			SetDrawAddColor(param / 2 + 48, param / 2 + 48, param / 2 + 48);
		for (int i = 0; i < 10; ++i) {
			DrawBox(
				x + 8 + GAUGE_WIDTH_BULLET * i + inversion * 48,
				y + 130,
				x + 8 + GAUGE_WIDTH_BULLET * i + GAUGE_WIDTH_BULLET - GAUGE_SPACE + inversion * 48,
				y + 130 + 6,
				(i < tmp_count) ? 0xffff00 : 0x404040, TRUE);
		}
		SetDrawAddColor(0, 0, 0);
	}

	// 弾
	(m_CursorType[player] == eMENU_TYPE::BULLET) ?
		SetDrawAddColor(param / 2 + 48, param / 2 + 48, param / 2 + 48) :
		SetDrawAddColor(0, 0, 0);
	m_DrawIcon(x + 30 + inversion * 48, y + CUSTOM_AREA_HEIGHT - 30, eMENU_TYPE::BULLET, (int)m_SelectedBullet[player], false);

	// スキル
	(m_CursorType[player] == eMENU_TYPE::SKILL) ?
		SetDrawAddColor(param / 2 + 48, param / 2 + 48, param / 2 + 48) :
		SetDrawAddColor(0, 0, 0);
	m_DrawIcon(x + 86 + inversion * 48, y + CUSTOM_AREA_HEIGHT - 30, eMENU_TYPE::SKILL, (int)m_SelectedSkill[player], false);

	// アビリティ
	(m_CursorType[player] == eMENU_TYPE::ABILITY) ?
		SetDrawAddColor(param / 2 + 48, param / 2 + 48, param / 2 + 48) :
		SetDrawAddColor(0, 0, 0);
	m_DrawIcon(x + 142 + inversion * 48, y + CUSTOM_AREA_HEIGHT - 30, eMENU_TYPE::ABILITY, (int)m_SelectedAbility[player], false);
	SetDrawAddColor(0, 0, 0);

	// 準備完了
	if (m_Ready[player]) {
		SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
		DrawBox(x, y, x + CUSTOM_AREA_WIDTH, y + CUSTOM_AREA_HEIGHT, 0x808080, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawString(x + CUSTOM_AREA_WIDTH / 2 - 32, y + CUSTOM_AREA_HEIGHT / 2 - 8, "準備完了", 0x00ff00);
	}
}

/* アイコン説明領域の描画 */
void cSelect::m_DrawLoreArea(int x, int y, int player) const
{
	// 2人モード
	if (!cCommonData::Player4Flag)
	{
		// 枠
		DrawBox(x - LORE_AREA_WIDTH / 2, y - LORE_AREA_HEIGHT_2 / 2, x + LORE_AREA_WIDTH / 2, y + LORE_AREA_HEIGHT_2 / 2,
			(player == 0) ? 0x400000 : (player == 1) ? 0x000040 : (player == 2) ? 0x004000 : (player == 3) ? 0x404000 : 0x404040, TRUE);
		DrawBox(x - LORE_AREA_WIDTH / 2, y - LORE_AREA_HEIGHT_2 / 2, x + LORE_AREA_WIDTH / 2, y + LORE_AREA_HEIGHT_2 / 2,
			(player == 0) ? 0xff4040 : (player == 1) ? 0x4040ff : (player == 2) ? 0x40ff40 : (player == 3) ? 0xffff40 : 0xffffff, FALSE);

		// アイコン
		m_DrawIcon(x - LORE_AREA_WIDTH / 2 + 34, y - LORE_AREA_HEIGHT_2 / 2 + 34, m_CursorType[player], m_CursorPos[player], false);

		// 名前
		DrawStringToHandle(x - LORE_AREA_WIDTH / 2 + 67, y - LORE_AREA_HEIGHT_2 / 2 + 17, m_Menu[(int)m_CursorType[player]][(int)m_CursorPos[player]].Name.c_str(),
			(player == 0) ? 0xa00000 : (player == 1) ? 0x0000a0 : (player == 2) ? 0x00a000 : (player == 3) ? 0xa0a000 : 0xa0a0a0, m_Font_Name);
		DrawStringToHandle(x - LORE_AREA_WIDTH / 2 + 65, y - LORE_AREA_HEIGHT_2 / 2 + 15, m_Menu[(int)m_CursorType[player]][(int)m_CursorPos[player]].Name.c_str(), 0xffffff, m_Font_Name);

		// 説明文
		DrawStringToHandle(x - LORE_AREA_WIDTH / 2 + 8, y - LORE_AREA_HEIGHT_2 / 2 + 76, m_Menu[(int)m_CursorType[player]][(int)m_CursorPos[player]].Lore.c_str(), 0xffffff, m_Font_Lore);
	}
	// 4人モード
	else
	{
		// 枠
		DrawBox(x - LORE_AREA_WIDTH / 2, y - LORE_AREA_HEIGHT_4 / 2, x + LORE_AREA_WIDTH / 2, y + LORE_AREA_HEIGHT_4 / 2,
			(player == 0) ? 0x400000 : (player == 1) ? 0x000040 : (player == 2) ? 0x004000 : (player == 3) ? 0x404000 : 0x404040, TRUE);
		DrawBox(x - LORE_AREA_WIDTH / 2, y - LORE_AREA_HEIGHT_4 / 2, x + LORE_AREA_WIDTH / 2, y + LORE_AREA_HEIGHT_4 / 2,
			(player == 0) ? 0xff4040 : (player == 1) ? 0x4040ff : (player == 2) ? 0x40ff40 : (player == 3) ? 0xffff40 : 0xffffff, FALSE);

		// アイコン
		m_DrawIcon(x - LORE_AREA_WIDTH / 2 + 28, y, m_CursorType[player], m_CursorPos[player], false);

		// 名前
		DrawString(x - LORE_AREA_WIDTH / 2 + 61, y - LORE_AREA_HEIGHT_4 / 2 + 11, m_Menu[(int)m_CursorType[player]][(int)m_CursorPos[player]].Name.c_str(),
			(player == 0) ? 0xa00000 : (player == 1) ? 0x0000a0 : (player == 2) ? 0x00a000 : (player == 3) ? 0xa0a000 : 0xa0a0a0);
		DrawString(x - LORE_AREA_WIDTH / 2 + 60, y - LORE_AREA_HEIGHT_4 / 2 + 10, m_Menu[(int)m_CursorType[player]][(int)m_CursorPos[player]].Name.c_str(), 0xffffff);

		// 説明文
		DrawString(x - LORE_AREA_WIDTH / 2 + 60, y - LORE_AREA_HEIGHT_4 / 2 + 30, m_Menu[(int)m_CursorType[player]][(int)m_CursorPos[player]].Lore.c_str(), 0xffffff);
	}
}
