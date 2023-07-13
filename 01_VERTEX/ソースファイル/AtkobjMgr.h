#pragma once

#include "System.h"
#include "BaseAtkobj.h"
#include <list>
class cPlayer;

class cAtkobjMgr
{
	/* 公開関数 */
public:
	static void CreateInstance();		// インスタンスの生成
	static void DestroyInstance();		// インスタンスの解放
	static cAtkobjMgr& GetInstance();	// インスタンスの取得

	void Update();	// 更新
	void Draw();	// 描画

	void Create_NormalBullet(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet);					// 攻撃オブジェクトを生成・通常弾
	void Create_ChargeBullet(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet, int ChargePower);	// 攻撃オブジェクトを生成・チャージ弾
	void Create_Laser(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet);							// 攻撃オブジェクトを生成・レーザー
	void Create_Misslie(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet);						// 攻撃オブジェクトを生成・ミサイル
	void Create_DeathBullet(cPlayer* Owner, VEC2F Pos, float Angle, int Ricochet);									// 攻撃オブジェクトを生成・デス
	void Create_Orb(cPlayer* Owner, int AtkDamage, int id);															// 攻撃オブジェクトを生成・オーブ
	void Create_ElectricTrap(cPlayer* Owner, VEC2F Pos);															// 攻撃オブジェクトを生成・静電トラップ
	void Create_MineTrap(cPlayer* Owner, VEC2F Pos, VEC2F Motion);													// 攻撃オブジェクトを生成・地雷トラップ
	void Create_Blast(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Radius);										// 攻撃オブジェクトを生成・爆風
	void Create_Wind(cPlayer* Owner, VEC2F Pos, float Radius, float Power, int Time);								// 攻撃オブジェクトを生成・風圧

	/* 非公開関数 */
private:
	::DISALLOW_COPY_CONSTRUCTOR(cAtkobjMgr);
	::DISALLOW_MOVE_CONSTRUCTOR(cAtkobjMgr);

	cAtkobjMgr();	// コンストラクタ
	~cAtkobjMgr();	// デストラクタ

	/* 非公開変数 */
private:
	static cAtkobjMgr* m_pInstance;	// インスタンスのポインタ

	std::list<cBaseAtkobj*> m_pAtkobjList;		// 攻撃オブジェクトリスト
	std::list<cBaseAtkobj*> m_pNewAtkobjList;	// 新規攻撃オブジェクトリスト
};
