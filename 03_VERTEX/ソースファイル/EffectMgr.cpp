//////////////////////
//   プリプロセス   //
//////////////////////
#include "EffectMgr.h"
#include "DxLib.h"
#include "SceneMgr.h"

#include "Line.h"
#include "Circle.h"
#include "MuzzleFlash.h"
#include "Aura.h"
#include "Confetti.h"
#include "Dead.h"
#include "NormalBullet_Trajectory.h"
#include "Death.h"
using namespace std;

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
cEffectMgr* cEffectMgr::m_pInstance(nullptr);

//////////////////
//   公開関数   //
//////////////////

/* インスタンスの生成 */
void cEffectMgr::CreateInstance()
{
	if (m_pInstance)
		return;
	m_pInstance = new cEffectMgr;
}

/* インスタンスの解放 */
void cEffectMgr::DestroyInstance()
{
	if (!m_pInstance)
		return;
	delete m_pInstance;
	m_pInstance = nullptr;
}

/* インスタンスの取得 */
cEffectMgr& cEffectMgr::GetInstance()
{
	return *m_pInstance;
}

/* コンストラクタ */
cEffectMgr::cEffectMgr()
{
	// リソースの読み込み
	m_Image_Particle[(int)ePARTICLE::LINE] = LoadGraph("data\\image\\effect\\line.png");
	m_Image_Particle[(int)ePARTICLE::RHOMBUS] = LoadGraph("data\\image\\effect\\rhombus.png");
	m_Image_Particle[(int)ePARTICLE::RING_IN] = LoadGraph("data\\image\\effect\\ring_in.png");
	m_Image_Particle[(int)ePARTICLE::RING_OUT] = LoadGraph("data\\image\\effect\\ring_out.png");
	m_Image_Particle[(int)ePARTICLE::MUZZLE_FLASH] = LoadGraph("data\\image\\effect\\muzzle_flash.png");
	m_Image_Particle[(int)ePARTICLE::SHIELD] = LoadGraph("data\\image\\effect\\shield.png");
	m_Image_Particle[(int)ePARTICLE::AURA] = LoadGraph("data\\image\\effect\\aura_2.png");
	m_Image_Particle[(int)ePARTICLE::CONFETTI_1] = LoadGraph("data\\image\\effect\\confetti_1.png");
	m_Image_Particle[(int)ePARTICLE::CONFETTI_2] = LoadGraph("data\\image\\effect\\confetti_2.png");
	m_Image_Particle[(int)ePARTICLE::CONFETTI_3] = LoadGraph("data\\image\\effect\\confetti_3.png");
}

/* デストラクタ */
cEffectMgr::~cEffectMgr()
{
	// リソースの解放
	for (int i = 0; i < (int)ePARTICLE::MAX; ++i)
		DeleteGraph(m_Image_Particle[i]);

	// エフェクトリストの解放
	for (auto& i : m_pEffectList) {
		delete i;
		i = nullptr;
	}
	for (auto& i : m_pNewEffectList) {
		delete i;
		i = nullptr;
	}
	m_pEffectList.clear();
	m_pNewEffectList.clear();
}

/* 更新 */
void cEffectMgr::Update()
{
	// 新規エフェクトの追加
	for (auto& i : m_pNewEffectList)
		m_pEffectList.push_back(i);
	m_pNewEffectList.clear();

	// 非アクティブエフェクトの削除
	for (auto i = m_pEffectList.begin(); i != m_pEffectList.end();)
	{
		if ((*i)->Active())
			++i;
		else {
			delete (*i);
			(*i) = nullptr;
			i = m_pEffectList.erase(i);
		}
	}

	// エフェクトの更新
	for (auto& i : m_pEffectList)
		i->Update();
}

/* 描画 */
void cEffectMgr::Draw() const
{
	// エフェクトの描画・FADE_ADD
	cSceneMgr::GetInstance().ChangeScreen(eSCREEN::FADE_ADD);
	for (const auto& i : m_pEffectList)
		i->Draw_FadeAdd();

	// エフェクトの描画・DEFAULT
	cSceneMgr::GetInstance().ChangeScreen(eSCREEN::DEFAULT);
	for (const auto& i : m_pEffectList)
		i->Draw_Default();
}

/* エフェクトを生成・線 */
void cEffectMgr::Create_Line(VEC2F Pos, float Angle, float Motion, unsigned int Color)
{
	m_pNewEffectList.push_back(new cLine(Pos, Angle, Motion, Color));
}

/* エフェクトを生成・円 */
void cEffectMgr::Create_Circle(VEC2F Pos, float Radius, float Motion, unsigned int Color)
{
	m_pNewEffectList.push_back(new cCircle(Pos, Radius, Motion, Color));
}

/* エフェクトを生成・マズルフラッシュ */
void cEffectMgr::Create_MuzzleFlash(VEC2F Pos, float Scale, float Angle)
{
	m_pNewEffectList.push_back(new cMuzzleFlash(Pos, Scale, Angle));
}

/* エフェクトを生成・オーラ */
void cEffectMgr::Create_Aura(VEC2F Pos, float Scale, float Angle, float Motion, unsigned int Color)
{
	m_pNewEffectList.push_back(new cAura(Pos, Color, Scale, Angle, Motion));
}

/* エフェクトを生成・紙吹雪 */
void cEffectMgr::Create_Confetti(VEC2F Pos, float Angle, float Motion, unsigned int Color)
{
	m_pNewEffectList.push_back(new cConfetti(Pos, Color, Angle, Motion));
}

/* エフェクトを生成・死亡 */
void cEffectMgr::Create_Dead(VEC2F Pos, unsigned int Color)
{
	m_pNewEffectList.push_back(new cDead(Pos, Color));
}

/* エフェクトを生成・通常弾の軌跡 */
void cEffectMgr::Create_NormalBullet_Trajectory(VEC2F Pos, float Angle, unsigned int Color, double MaxScale)
{
	m_pNewEffectList.push_back(new cNormalBullet_Trajectory(Pos, Angle, Color, MaxScale));
}

/* エフェクトを生成・デスの被弾 */
void cEffectMgr::Create_DeathBullet_Hit(VEC2F Pos)
{
	m_pNewEffectList.push_back(new cDeath(Pos));
}

/* パーティクルの画像ハンドルを取得 */
int cEffectMgr::GetImageHandle(ePARTICLE particle) const
{
	if ((int)particle < 0 || (int)particle >= (int)ePARTICLE::MAX)
		return -1;

	return m_Image_Particle[(int)particle];
}
