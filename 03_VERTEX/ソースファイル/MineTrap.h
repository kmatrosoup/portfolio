#pragma once

#include "BaseAtkobj.h"
#include <list>
class cPlayer;

class cMineTrap : public cBaseAtkobj
{
	/* 公開関数 */
public:
	cMineTrap(cPlayer* Owner, VEC2F Pos, VEC2F Motion);	// コンストラクタ
	~cMineTrap() override;								// デストラクタ
	void Update() override;								// 更新
	void Draw_Default() const override;					// 描画・DEFAULT

	bool Active() const override;	// アクティブ状態
	void Detonate();				// 起動

	static void LoadResource();		// リソースの読み込み
	static void DeleteResource();	// リソースの解放

	/* 非公開変数 */
private:
	static const float m_Radius;		// 半径
	static const float m_ActiveRadius;	// 感知半径
	static const int m_MaxTime;			// 起爆までの最大時間
	static const int m_PlaceTime;		// 設置時間
	static const int m_DetonateTime;	// 起爆時間
	static const int m_MaxHP;			// 最大体力

	VEC2F m_Pos;			// 位置
	VEC2F m_Motion;			// 移動量
	int m_HP;				// 体力
	int m_Timer;			// 生存時間
	bool m_ExplosionFlag;	// 爆発フラグ

	static std::list<cMineTrap*> m_PlayerMineTrap[4];	// 設置した地雷

	static bool m_LoadedFlag;		// リソースの読み込みフラグ
	static int m_Image_MineTrap;	// 画像ハンドル・地雷
};
