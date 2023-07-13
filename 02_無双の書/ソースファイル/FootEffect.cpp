// プリプロセス
#include "FootEffect.h"
#include "DxLib.h"

// 定数の宣言
#define MAX_TIME	(30)

// 静的メンバ変数の宣言
bool cFootEffect::m_LoadedResourceFlag(false);
int cFootEffect::m_Image_FootEffect[3]{};

// リソースの読み込み
void cFootEffect::LoadResource()
{
	if (m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = true;
	LoadDivGraph("data\\effect_foot.png", 3, 3, 1, 8, 8, m_Image_FootEffect);
}

// リソースの解放
void cFootEffect::DeleteResource()
{
	if (!m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = false;
	for (int i = 0; i < 3; ++i)	DeleteGraph(m_Image_FootEffect[i]);
}

// コンストラクタ
cFootEffect::cFootEffect(VEC2F pos)
	: cBaseEffect(pos)
	, m_Timer(0)
	, m_Angle(DX_PI / 180.0 * GetRand(359))
{
}

// 更新
void cFootEffect::Update()
{
	if (m_Timer < MAX_TIME)
		++m_Timer;
}

// 描画
void cFootEffect::Draw() const
{
	DrawRotaGraph(
		(int)m_Pos.x,
		(int)m_Pos.y,
		4.0, m_Angle, m_Image_FootEffect[m_Timer * 3 / MAX_TIME], TRUE);
}

// アクティブ状態
bool cFootEffect::IsActive() const
{
	return m_Timer < MAX_TIME;
}
