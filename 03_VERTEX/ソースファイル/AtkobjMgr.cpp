//////////////////////
//   プリプロセス   //
//////////////////////
#include "AtkobjMgr.h"
#include "Player.h"
#include "SceneMgr.h"
#include "Game.h"

#include "NormalBullet.h"
#include "ChargeBullet.h"
#include "Laser.h"
#include "Missile.h"
#include "DeathBullet.h"
#include "Orb.h"
#include "Blast.h"
#include "ElectricTrap.h"
#include "MineTrap.h"
#include "Wind.h"
using namespace std;

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
cAtkobjMgr* cAtkobjMgr::m_pInstance(nullptr);

//////////////////
//   公開関数   //
//////////////////

/* インスタンスの生成 */
void cAtkobjMgr::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cAtkobjMgr;
}

/* インスタンスの解放 */
void cAtkobjMgr::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

/* インスタンスの取得 */
cAtkobjMgr& cAtkobjMgr::GetInstance()
{
	return *m_pInstance;
}

/* コンストラクタ */
cAtkobjMgr::cAtkobjMgr()
{
	// リソース読み込み
	cNormalBullet::LoadResource();
	cMineTrap::LoadResource();
}

/* デストラクタ */
cAtkobjMgr::~cAtkobjMgr()
{
	// リソース解放
	cNormalBullet::DeleteResource();
	cMineTrap::DeleteResource();

	// メモリの解放
	for (auto& i : m_pAtkobjList) {
		delete i;
		i = nullptr;
	}
	for (auto& i : m_pNewAtkobjList) {
		delete i;
		i = nullptr;
	}
}

/* 更新 */
void cAtkobjMgr::Update()
{
	// 新しいインスタンスがあれば追加する
	for (auto& i : m_pNewAtkobjList)
		m_pAtkobjList.push_back(i);
	m_pNewAtkobjList.clear();

	// インスタンスを更新し、非アクティブなら削除する
	for (auto i = m_pAtkobjList.begin(); i != m_pAtkobjList.end();)
	{
		(*i)->Update();

		if ((*i)->Active())
			++i;
		else {
			delete (*i);
			(*i) = nullptr;
			i = m_pAtkobjList.erase(i);
		}
	}
}

/* 描画 */
void cAtkobjMgr::Draw()
{
	// インスタンスの描画・FADE_ADD
	cSceneMgr::GetInstance().ChangeScreen(eSCREEN::FADE_ADD);
	for (const auto& i : m_pAtkobjList)
		i->Draw_FadeAdd();

	// インスタンスの描画・DEFAULT
	cSceneMgr::GetInstance().ChangeScreen(eSCREEN::DEFAULT);
	for (const auto& i : m_pAtkobjList)
		i->Draw_Default();
}

/* 攻撃オブジェクトを生成・通常弾 */
void cAtkobjMgr::Create_NormalBullet(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet)
{
	m_pNewAtkobjList.push_back(new cNormalBullet(Owner, AtkDamage, Pos, Angle, Ricochet));
}

/* 攻撃オブジェクトを生成・チャージ弾 */
void cAtkobjMgr::Create_ChargeBullet(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet, int ChargePower)
{
	m_pNewAtkobjList.push_back(new cChargeBullet(Owner, AtkDamage, Pos, Angle, Ricochet, ChargePower));
}

/* 攻撃オブジェクトを生成・レーザー */
void cAtkobjMgr::Create_Laser(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet)
{
	m_pNewAtkobjList.push_back(new cLaser(Owner, AtkDamage, Pos, Angle, Ricochet));
}

/* 攻撃オブジェクトを生成・ミサイル */
void cAtkobjMgr::Create_Misslie(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet)
{
	m_pNewAtkobjList.push_back(new cMissile(Owner, AtkDamage, Pos, Angle, Ricochet));
}

/* 攻撃オブジェクトを生成・デス */
void cAtkobjMgr::Create_DeathBullet(cPlayer* Owner, VEC2F Pos, float Angle, int Ricochet)
{
	m_pNewAtkobjList.push_back(new cDeathBullet(Owner, Pos, Angle, Ricochet));
}

/* 攻撃オブジェクトを生成・オーブ */
void cAtkobjMgr::Create_Orb(cPlayer* Owner, int AtkDamage, int id)
{
	m_pNewAtkobjList.push_back(new cOrb(Owner, AtkDamage, id));
}

/* 攻撃オブジェクトを生成・静電トラップ */
void cAtkobjMgr::Create_ElectricTrap(cPlayer* Owner, VEC2F Pos)
{
	m_pNewAtkobjList.push_back(new cElectricTrap(Owner, Pos));
}

/* 攻撃オブジェクトを生成・地雷トラップ */
void cAtkobjMgr::Create_MineTrap(cPlayer* Owner, VEC2F Pos, VEC2F Motion)
{
	m_pNewAtkobjList.push_back(new cMineTrap(Owner, Pos, Motion));
}

/* 攻撃オブジェクトを生成・風圧 */
void cAtkobjMgr::Create_Wind(cPlayer* Owner, VEC2F Pos, float Radius, float Power, int Time)
{
	m_pNewAtkobjList.push_back(new cWind(Owner, Pos, Radius, Power, Time));
}

/* 攻撃オブジェクトを生成・爆風 */
void cAtkobjMgr::Create_Blast(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Radius)
{
	m_pNewAtkobjList.push_back(new cBlast(Owner, AtkDamage, Pos, Radius));
}
