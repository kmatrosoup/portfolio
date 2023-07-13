#pragma once

#include "System.h"
#include "VEC2.h"
#include <vector>
#include <string>
#include <utility>

/* タイル */
enum class eTILE
{
	FLOOR,	// 床
	WALL,	// 壁
};

/* フィールドクラス */
class cField
{
	/* 公開関数 */
public:
	static void Init(int FieldID);	// 初期化
	static void Fin();				// 終了
	static void Update();			// 更新
	static void Draw();				// 描画

	static int GetMapDataNum();					// マップデータの数を取得
	static VEC2 GetCenterPos();					// フィールドの中心座標を取得
	static VEC2 GetPlayerStartPos(int Player);	// プレイヤーの開始点を取得
	static bool InWall(VEC2F Pos);				// 壁内判定
	static bool IsWall(int tx, int ty);			// 壁判定

	/* 非公開関数 */
private:
	cField() = delete;
	~cField() = delete;
	::DISALLOW_COPY_CONSTRUCTOR(cField);
	::DISALLOW_MOVE_CONSTRUCTOR(cField);

	static void m_LoadMapData(const char* FileName);	// マップデータ読み込み

	/* 公開変数 */
public:
	static const int TileSize;	// タイルの大きさ

	// 外部からの参照用
	static const std::vector<std::pair<VEC2, int>>& LineU;	// 判定辺・上 <first>:座標 <second>:長さ
	static const std::vector<std::pair<VEC2, int>>& LineD;	// 判定辺・下
	static const std::vector<std::pair<VEC2, int>>& LineL;	// 判定辺・左
	static const std::vector<std::pair<VEC2, int>>& LineR;	// 判定辺・右
	static const std::vector<VEC2>& CornerRD;				// 判定角・右下
	static const std::vector<VEC2>& CornerLD;				// 判定角・左下
	static const std::vector<VEC2>& CornerLU;				// 判定角・左上
	static const std::vector<VEC2>& CornerRU;				// 判定角・右上

	/* 非公開変数 */
private:
	static const std::vector<std::string> m_MapDataFileName;	// マップデータのファイル名
	static std::vector<std::vector<eTILE>> m_Field;				// タイル情報
	static VEC2 m_FieldSize;									// フィールドの大きさ
	static VEC2 m_PlayerStartPos[4];							// プレイヤーの初期位置
	static VEC2 m_CenterPos;									// フィールドの中心点
	static VEC2 m_LightPos_Up[2];								// ライトアップの位置・上
	static VEC2 m_LightPos_Down[2];								// ライトアップの位置・下

	static std::vector<std::pair<VEC2, int>> m_LineU;	// 判定辺・上 <first>:座標 <second>:長さ
	static std::vector<std::pair<VEC2, int>> m_LineD;	// 判定辺・下
	static std::vector<std::pair<VEC2, int>> m_LineL;	// 判定辺・左
	static std::vector<std::pair<VEC2, int>> m_LineR;	// 判定辺・右
	static std::vector<VEC2> m_CornerRD;				// 判定角・右下
	static std::vector<VEC2> m_CornerLD;				// 判定角・左下
	static std::vector<VEC2> m_CornerLU;				// 判定角・左上
	static std::vector<VEC2> m_CornerRU;				// 判定角・右上

	static int m_Image_Floor;	// 画像ハンドル・床
	static int m_Image_Wall[8];	// 画像ハンドル・壁
	static int m_Image_LightUp;	// 画像ハンドル・ライトアップ
};
