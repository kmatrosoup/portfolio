//////////////////////
//   プリプロセス   //
//////////////////////
#include "Field.h"
#include "DxLib.h"
#include "Game.h"
#include <fstream>
#include <string>
using namespace std;

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 公開変数 */
const int cField::TileSize(32);
const vector<pair<VEC2, int>>& cField::LineU(m_LineU);	// 判定辺・上 <first>:座標 <second>:長さ
const vector<pair<VEC2, int>>& cField::LineD(m_LineD);	// 判定辺・下
const vector<pair<VEC2, int>>& cField::LineL(m_LineL);	// 判定辺・左
const vector<pair<VEC2, int>>& cField::LineR(m_LineR);	// 判定辺・右
const vector<VEC2>& cField::CornerRD(m_CornerRD);		// 判定角・右下
const vector<VEC2>& cField::CornerLD(m_CornerLD);		// 判定角・左下
const vector<VEC2>& cField::CornerLU(m_CornerLU);		// 判定角・左上
const vector<VEC2>& cField::CornerRU(m_CornerRU);		// 判定角・右上

/* 非公開変数 */
const vector<string> cField::m_MapDataFileName	// フィールドデータのファイル名
{
	"data\\mapdata\\map1.txt",
	"data\\mapdata\\map2.txt",
	"data\\mapdata\\map3.txt",
	"data\\mapdata\\map4.txt",
	"data\\mapdata\\map5.txt",
	"data\\mapdata\\map6.txt",
	"data\\mapdata\\map7.txt",
	"data\\mapdata\\map8.txt",
	"data\\mapdata\\map9.txt",
	"data\\mapdata\\map10.txt",
};
vector<vector<eTILE>> cField::m_Field;	// タイル情報
VEC2 cField::m_FieldSize;				// フィールドの大きさ
VEC2 cField::m_PlayerStartPos[4];		// プレイヤーの初期位置
VEC2 cField::m_CenterPos;				// フィールドの中心点
VEC2 cField::m_LightPos_Up[2];			// ライトアップの位置・上
VEC2 cField::m_LightPos_Down[2];		// ライトアップの位置・下

vector<pair<VEC2, int>> cField::m_LineU;	// 判定辺・上 <first>:座標 <second>:長さ
vector<pair<VEC2, int>> cField::m_LineD;	// 判定辺・下
vector<pair<VEC2, int>> cField::m_LineL;	// 判定辺・左
vector<pair<VEC2, int>> cField::m_LineR;	// 判定辺・右
vector<VEC2> cField::m_CornerRU;			// 判定角・右下
vector<VEC2> cField::m_CornerRD;			// 判定角・左下
vector<VEC2> cField::m_CornerLD;			// 判定角・左上
vector<VEC2> cField::m_CornerLU;			// 判定角・右上

int cField::m_Image_Floor(-1);		// 画像ハンドル・床
int cField::m_Image_Wall[8]{};		// 画像ハンドル・壁
int cField::m_Image_LightUp(-1);	// 画像ハンドル・ライトアップ

//////////////////
//   公開関数   //
//////////////////

/* 初期化 */
void cField::Init(int FieldID)
{
	m_Image_Floor = LoadGraph("data\\image\\tile\\floor.png");
	LoadDivGraph("data\\image\\tile\\wall.png", 8, 8, 1, 8, 8, m_Image_Wall);
	m_Image_LightUp = LoadGraph("data\\image\\game\\lightup.png");

	(FieldID >= 0 && FieldID < (int)m_MapDataFileName.size()) ?
		m_LoadMapData(m_MapDataFileName[FieldID].c_str()) :
		m_LoadMapData(0);
}

/* 終了 */
void cField::Fin()
{
	DeleteGraph(m_Image_Floor);		m_Image_Floor = -1;
	for (int i = 0; i < 8; ++i)
		DeleteGraph(m_Image_Wall[i]), m_Image_Wall[i] = -1;
	DeleteGraph(m_Image_LightUp);	m_Image_LightUp = -1;

	m_Field.clear();
	m_Field.shrink_to_fit();

	m_LineU.clear();
	m_LineD.clear();
	m_LineL.clear();
	m_LineR.clear();
	m_CornerRD.clear();
	m_CornerLD.clear();
	m_CornerLU.clear();
	m_CornerRU.clear();
}

/* 更新 */
void cField::Update()
{
}

/* 描画 */
void cField::Draw()
{
	// タイル
	for (int y = 0; y < m_FieldSize.y; ++y) {
		for (int x = 0; x < m_FieldSize.x; ++x) {
			//switch (m_Field[y][x]) {
			//case eTILE::FLOOR:
			DrawExtendGraph(
				cGame::ScreenDif.x + TileSize * x,
				cGame::ScreenDif.y + TileSize * y,
				cGame::ScreenDif.x + TileSize * (x + 1),
				cGame::ScreenDif.y + TileSize * (y + 1),
				m_Image_Floor, FALSE);
			//	break;
			//}
		}
	}
	for (int y = 0; y < m_FieldSize.y; ++y) {
		for (int x = 0; x < m_FieldSize.x; ++x) {
			switch (m_Field[y][x]) {
			case eTILE::WALL:
				// 左上
				DrawExtendGraph(
					cGame::ScreenDif.x + TileSize * x + TileSize / 2,
					cGame::ScreenDif.y + TileSize * y + TileSize / 2,
					cGame::ScreenDif.x + TileSize * x,
					cGame::ScreenDif.y + TileSize * y,
					m_Image_Wall[IsWall(x - 1, y) * 1 + IsWall(x, y - 1) * 2 + IsWall(x - 1, y - 1) * 4], TRUE);
				// 右上
				DrawExtendGraph(
					cGame::ScreenDif.x + TileSize * x + TileSize / 2,
					cGame::ScreenDif.y + TileSize * y + TileSize / 2,
					cGame::ScreenDif.x + TileSize * x + TileSize,
					cGame::ScreenDif.y + TileSize * y,
					m_Image_Wall[IsWall(x + 1, y) * 1 + IsWall(x, y - 1) * 2 + IsWall(x + 1, y - 1) * 4], TRUE);
				// 左下
				DrawExtendGraph(
					cGame::ScreenDif.x + TileSize * x + TileSize / 2,
					cGame::ScreenDif.y + TileSize * y + TileSize / 2,
					cGame::ScreenDif.x + TileSize * x,
					cGame::ScreenDif.y + TileSize * y + TileSize,
					m_Image_Wall[IsWall(x - 1, y) * 1 + IsWall(x, y + 1) * 2 + IsWall(x - 1, y + 1) * 4], TRUE);
				// 右下
				DrawExtendGraph(
					cGame::ScreenDif.x + TileSize * x + TileSize / 2,
					cGame::ScreenDif.y + TileSize * y + TileSize / 2,
					cGame::ScreenDif.x + TileSize * x + TileSize,
					cGame::ScreenDif.y + TileSize * y + TileSize,
					m_Image_Wall[IsWall(x + 1, y) * 1 + IsWall(x, y + 1) * 2 + IsWall(x + 1, y + 1) * 4], TRUE);
				break;
			}
		}
	}

	// ライトアップ
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	for (int i = 0; i < 2; ++i) {
		DrawRotaGraph(cGame::ScreenDif.x + m_LightPos_Up[i].x, cGame::ScreenDif.y + m_LightPos_Up[i].y, 1.0, 0.0, m_Image_LightUp, TRUE);
		DrawRotaGraph(cGame::ScreenDif.x + m_LightPos_Down[i].x, cGame::ScreenDif.y + m_LightPos_Down[i].y, 1.0, 0.0, m_Image_LightUp, TRUE, FALSE, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/* マップデータの数を取得 */
int cField::GetMapDataNum()
{
	return (int)m_MapDataFileName.size();
}

/* フィールドの中心座標を取得 */
VEC2 cField::GetCenterPos()
{
	return m_CenterPos;
}

/* プレイヤーの開始点を取得 */
VEC2 cField::GetPlayerStartPos(int Player)
{
	if (Player < 0 || Player >= 4)
		return VEC2::ZERO;

	return m_PlayerStartPos[Player] * TileSize + VEC2(TileSize, TileSize) / 2;
}

/* 壁内判定 */
bool cField::InWall(VEC2F Pos)
{
	int fx = (int)floorf(Pos.x / TileSize);
	int fy = (int)floorf(Pos.y / TileSize);
	int cx = (int)ceilf(Pos.x / TileSize) - 1;
	int cy = (int)ceilf(Pos.y / TileSize) - 1;

	// 範囲外なら<true>を返す
	if (cx < 0 || cy < 0 || fx >= m_FieldSize.x || fy >= m_FieldSize.y)
		return true;

	return
		m_Field[fy][fx] == eTILE::WALL &&
		m_Field[fy][cx] == eTILE::WALL &&
		m_Field[cy][fx] == eTILE::WALL &&
		m_Field[cy][cx] == eTILE::WALL;
}

/* 壁判定 */
bool cField::IsWall(int tx, int ty)
{
	if (tx < 0 || ty < 0 || tx >= m_FieldSize.x || ty >= m_FieldSize.y)
		return true;

	return m_Field[ty][tx] == eTILE::WALL;
}

////////////////////
//   非公開変数   //
////////////////////

/* マップデータ読み込み */
void cField::m_LoadMapData(const char* FileName)
{
	// ファイルを開く
	ifstream ifs(FileName);
	if (ifs.fail())
		return;

	// マップデータを読み込む
	for (int i = 0; i < 4; ++i)
		m_PlayerStartPos[i] = VEC2::ZERO;
	m_LineU.clear();
	m_LineD.clear();
	m_LineL.clear();
	m_LineR.clear();
	m_CornerRD.clear();
	m_CornerLD.clear();
	m_CornerLU.clear();
	m_CornerRU.clear();
	string buf;
	getline(ifs, buf);	m_FieldSize.x = atoi(buf.c_str());
	getline(ifs, buf);	m_FieldSize.y = atoi(buf.c_str());
	m_Field.assign(m_FieldSize.y, vector<eTILE>(m_FieldSize.x, eTILE::FLOOR));
	m_CenterPos = m_FieldSize * TileSize / 2;
	for (int y = 0; y < m_FieldSize.y; ++y)
	{
		getline(ifs, buf);
		for (int x = 0; x < m_FieldSize.x; ++x)
		{
			switch (buf[x])
			{
			case '#':
				m_Field[y][x] = eTILE::WALL;
				break;
			case '1':
				m_PlayerStartPos[0] = VEC2(x, y);
				break;
			case '2':
				m_PlayerStartPos[1] = VEC2(x, y);
				break;
			case '3':
				m_PlayerStartPos[2] = VEC2(x, y);
				break;
			case '4':
				m_PlayerStartPos[3] = VEC2(x, y);
				break;
			}
		}
	}

	// ファイルを閉じる
	ifs.close();

	// 判定角・線を初期化
	{
		auto f_CheckWall = [&](int x, int y) -> bool
		{
			if (x < 0 || y < 0 || x >= m_FieldSize.x || y >= m_FieldSize.y)
				return true;
			return m_Field[y][x] == eTILE::WALL;
		};

		for (int y = -1; y <= m_FieldSize.y; ++y) {
			for (int x = -1; x <= m_FieldSize.x; ++x) {
				if (f_CheckWall(x, y)) {
					// 線・上
					if (!f_CheckWall(x, y - 1)) {
						auto itr = find_if(m_LineU.begin(), m_LineU.end(), [&](const pair<VEC2, int>& n) { return (y == n.first.y && x == n.first.x + n.second); });
						if (itr == m_LineU.end())
							m_LineU.push_back(make_pair<VEC2, int>(VEC2(x, y), 1));
						else
							++itr->second;
					}

					// 線・下
					if (!f_CheckWall(x, y + 1)) {
						auto itr = find_if(m_LineD.begin(), m_LineD.end(), [&](const pair<VEC2, int>& n) { return (y == n.first.y - 1 && x == n.first.x + n.second); });
						if (itr == m_LineD.end())
							m_LineD.push_back(make_pair<VEC2, int>(VEC2(x, y + 1), 1));
						else
							++itr->second;
					}

					// 線・左
					if (!f_CheckWall(x - 1, y)) {
						auto itr = find_if(m_LineL.begin(), m_LineL.end(), [&](const pair<VEC2, int>& n) { return (y == n.first.y + n.second && x == n.first.x); });
						if (itr == m_LineL.end())
							m_LineL.push_back(make_pair<VEC2, int>(VEC2(x, y), 1));
						else
							++itr->second;
					}

					// 線・右
					if (!f_CheckWall(x + 1, y)) {
						auto itr = find_if(m_LineR.begin(), m_LineR.end(), [&](const pair<VEC2, int>& n) { return (y == n.first.y + n.second && x == n.first.x - 1); });
						if (itr == m_LineR.end())
							m_LineR.push_back(make_pair<VEC2, int>(VEC2(x + 1, y), 1));
						else
							++itr->second;
					}

					// 角・右下
					if (!f_CheckWall(x + 1, y) && !f_CheckWall(x, y + 1) && !f_CheckWall(x + 1, y + 1))
						m_CornerRD.push_back(VEC2(x + 1, y + 1));

					// 角・左下
					if (!f_CheckWall(x - 1, y) && !f_CheckWall(x, y + 1) && !f_CheckWall(x - 1, y + 1))
						m_CornerLD.push_back(VEC2(x, y + 1));

					// 角・左上
					if (!f_CheckWall(x - 1, y) && !f_CheckWall(x, y - 1) && !f_CheckWall(x - 1, y - 1))
						m_CornerLU.push_back(VEC2(x, y));

					// 角・右上
					if (!f_CheckWall(x + 1, y) && !f_CheckWall(x, y - 1) && !f_CheckWall(x + 1, y - 1))
						m_CornerRU.push_back(VEC2(x + 1, y));
				}
			}
		}
	}

	// ライトアップの位置を設定
	m_LightPos_Up[0] = VEC2(TileSize * m_FieldSize.x / 4, -TileSize);
	m_LightPos_Up[1] = VEC2(TileSize * m_FieldSize.x / 4 * 3, -TileSize);
	m_LightPos_Down[0] = VEC2(TileSize * m_FieldSize.x / 4, TileSize * (m_FieldSize.y + 1));
	m_LightPos_Down[1] = VEC2(TileSize * m_FieldSize.x / 4 * 3, TileSize * (m_FieldSize.y + 1));
}
