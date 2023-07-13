// プリプロセス
#include "BaseBook.h"
#include "Game.h"
#include "DxLib.h"

// 定数の宣言
#define ACTIVE_TIME (60 * 15)

// 静的メンバ変数の宣言
bool cBaseBook::m_LoadedResourceFlag(false);
int cBaseBook::m_Image_Book[(int)eSPELL::MAX]{};

// リソースの読み込み
void cBaseBook::LoadResource()
{
	if (m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = true;
	m_Image_Book[(int)eSPELL::FIRE_BALL] = LoadGraph("data\\book_fireball.png");
	m_Image_Book[(int)eSPELL::EXPLOSION] = LoadGraph("data\\book_explosion.png");
}

// リソースの解放
void cBaseBook::DeleteResource()
{
	if (!m_LoadedResourceFlag)
		return;

	m_LoadedResourceFlag = false;
	for (int i = 0; i < (int)eSPELL::MAX; ++i)
		DeleteGraph(m_Image_Book[i]);
}

// コンストラクタ
cBaseBook::cBaseBook(eSPELL spell, VEC2F pos, VEC2F motion, float angle, int count, bool no_erase_flag)
	: m_Spell(spell)
	, m_Pos(pos)
	, m_Motion(motion)
	, m_Angle(angle)
	, m_InversionFlag((m_Motion.x == 0.0f) ? GetRand(1) : (m_Motion.x < 0.0f))
	, m_RestCount(count)
	, m_ActiveTimer(ACTIVE_TIME)
	, m_NoEraseFlag(no_erase_flag)
{
}

// デストラクタ
cBaseBook::~cBaseBook()
{
}

// 更新
void cBaseBook::Update()
{
	m_Motion *= 0.95f;
	m_Pos += m_Motion;
	if (m_Motion.x < 0.0f && m_Pos.x < (float)FLOOR_X1)
	{
		m_Pos.x = (float)FLOOR_X1;
		m_Motion.x *= -1.0f;
		m_InversionFlag = false;
	}
	if (m_Motion.y < 0.0f && m_Pos.y < (float)FLOOR_Y1)
	{
		m_Pos.y = (float)FLOOR_Y1;
		m_Motion.y *= -1.0f;
	}
	if (m_Motion.x > 0.0f && m_Pos.x > (float)FLOOR_X2)
	{
		m_Pos.x = (float)FLOOR_X2;
		m_Motion.x *= -1.0f;
		m_InversionFlag = true;
	}
	if (m_Motion.y > 0.0f && m_Pos.y > (float)FLOOR_Y2)
	{
		m_Pos.y = (float)FLOOR_Y2;
		m_Motion.y *= -1.0f;
	}
	m_Angle += m_Motion.Length() * 0.01f;

	if (!m_NoEraseFlag)
		--m_ActiveTimer;
}

// 描画
void cBaseBook::Draw() const
{
	DrawRotaGraph(
		(int)m_Pos.x,
		(int)m_Pos.y,
		4.0, (double)m_Angle, m_Image_Book[(int)m_Spell], TRUE, m_InversionFlag);
}

// 魔法を発動
void cBaseBook::RunMagic()
{
	m_RunMagicProcess();
	--m_RestCount;
}

// アクティブ状態
bool cBaseBook::IsActive()
{
	return m_RestCount > 0 && m_ActiveTimer > 0;
}

// 座標を取得
VEC2F cBaseBook::GetPos() const
{
	return m_Pos;
}

// 座標を設定
void cBaseBook::SetPos(VEC2F pos)
{
	m_Pos = pos;
}

// 移動量を取得
VEC2F cBaseBook::GetMotion() const
{
	return m_Motion;
}

// 移動量を設定
void cBaseBook::SetMotion(VEC2F motion)
{
	m_Motion = motion;
}

// 角度を取得
float cBaseBook::GetAngle() const
{
	return m_Angle;
}

// 角度を設定
void cBaseBook::SetAngle(float angle)
{
	m_Angle = angle;
}
// 向きを取得
bool cBaseBook::GetInversion() const
{
	return m_InversionFlag;
}

// 向きを設定
void cBaseBook::SetInversion(bool inversion)
{
	m_InversionFlag = inversion;
}

// アクティブ時間を取得
int cBaseBook::GetActiveTime() const
{
	return m_ActiveTimer;
}
