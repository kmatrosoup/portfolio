//////////////////////
//   プリプロセス   //
//////////////////////
#include "MineTrap.h"
#include "DxLib.h"
#include "Player.h"
#include "Field.h"
#include "Game.h"
#include "SoundData.h"
#include "AtkobjMgr.h"
#include "CommonData.h"
#include "Easing.h"

//////////////////////////////
//   静的メンバ変数の宣言   //
//////////////////////////////

/* 非公開変数 */
const float cMineTrap::m_Radius(10.0f);			// 半径
const float cMineTrap::m_ActiveRadius(50.0f);	// 感知半径
const int cMineTrap::m_MaxTime(3600);			// 起爆までの最大時間
const int cMineTrap::m_PlaceTime(30);			// 設置時間
const int cMineTrap::m_DetonateTime(36);		// 起爆時間
const int cMineTrap::m_MaxHP(3);				// 最大体力

std::list<cMineTrap*> cMineTrap::m_PlayerMineTrap[4];	// 設置した地雷

bool cMineTrap::m_LoadedFlag(false);	// リソースの読み込みフラグ
int cMineTrap::m_Image_MineTrap(-1);	// 画像ハンドル・地雷

//////////////////
//   公開関数   //
//////////////////

/* コンストラクタ */
cMineTrap::cMineTrap(cPlayer* Owner, VEC2F Pos, VEC2F Motion)
	: cBaseAtkobj(Owner)
	, m_Pos(Pos)
	, m_Motion(Motion)
	, m_HP(m_MaxHP)
	, m_Timer(m_MaxTime + m_PlaceTime)
	, m_ExplosionFlag(false)
{
	if (m_Owner)
		cSoundData::GetInstance().Play(eSOUND::BULLET_MISSILE);
	else
		cAtkobjMgr::GetInstance().Create_Wind(nullptr, m_Pos, 80.0f, 4.0f, 1);

	if (m_Owner)
		if (m_Owner->PlayerID >= 0 && m_Owner->PlayerID < 4) {
			m_PlayerMineTrap[m_Owner->PlayerID].push_back(this);
			if ((int)m_PlayerMineTrap[m_Owner->PlayerID].size() > m_Owner->MaxMineTrapNum) {
				int detonate_num = (int)m_PlayerMineTrap[m_Owner->PlayerID].size() - m_Owner->MaxMineTrapNum;
				for (auto& i : m_PlayerMineTrap[m_Owner->PlayerID]) {
					i->Detonate();
					if (--detonate_num <= 0)
						break;
				}
			}
		}

	cGame::ShakeEffect(20);
}

/* デストラクタ */
cMineTrap::~cMineTrap()
{
	if (m_ExplosionFlag) {
		cAtkobjMgr::GetInstance().Create_Blast(m_Owner, 3, m_Pos, 120.0f);
		cGame::ShakeEffect(50);
	}

	if (m_Owner)
		if (m_Owner->PlayerID >= 0 && m_Owner->PlayerID < 4) {
			auto finded = std::find(m_PlayerMineTrap[m_Owner->PlayerID].begin(), m_PlayerMineTrap[m_Owner->PlayerID].end(), this);
			if (finded != m_PlayerMineTrap[m_Owner->PlayerID].end())
				m_PlayerMineTrap[m_Owner->PlayerID].erase(finded);
		}
}

/* 更新 */
void cMineTrap::Update()
{
	// タイマー制御
	if (--m_Timer < 0)
		m_ExplosionFlag = true;

	// 移動処理
	VEC2F pos_prev = m_Pos + m_Motion;
	{
		float lowest_param = 1.0f;
		bool ricochetX_flag = false;
		bool ricochetY_flag = false;
		if (m_Motion.y > 0.0f) {
			for (const auto& i : cField::LineU) {
				float param = (i.first.y * cField::TileSize - m_Pos.y) / m_Motion.y;
				if (param >= 0.0f && param < 1.0f && lowest_param > param) {
					float tmpX = m_Pos.x + m_Motion.x * param;
					if (tmpX >= (float)(i.first.x * cField::TileSize) && tmpX <= (float)((i.first.x + i.second) * cField::TileSize)) {
						lowest_param = param;
						pos_prev = m_Pos + m_Motion * param;
						ricochetX_flag = false;
						ricochetY_flag = true;
					}
				}
			}
		}
		if (m_Motion.y < 0.0f) {
			for (const auto& i : cField::LineD) {
				float param = (i.first.y * cField::TileSize - m_Pos.y) / m_Motion.y;
				if (param >= 0.0f && param < 1.0f && lowest_param > param) {
					float tmpX = m_Pos.x + m_Motion.x * param;
					if (tmpX >= (float)(i.first.x * cField::TileSize) && tmpX <= (float)((i.first.x + i.second) * cField::TileSize)) {
						lowest_param = param;
						pos_prev = m_Pos + m_Motion * param;
						ricochetX_flag = false;
						ricochetY_flag = true;
					}
				}
			}
		}
		if (m_Motion.x > 0.0f) {
			for (const auto& i : cField::LineL) {
				float param = (i.first.x * cField::TileSize - m_Pos.x) / m_Motion.x;
				if (param >= 0.0f && param < 1.0f && lowest_param > param) {
					float tmpY = m_Pos.y + m_Motion.y * param;
					if (tmpY >= (float)(i.first.y * cField::TileSize) && tmpY <= (float)((i.first.y + i.second) * cField::TileSize)) {
						lowest_param = param;
						pos_prev = m_Pos + m_Motion * param;
						ricochetX_flag = true;
						ricochetY_flag = false;
					}
				}
			}
		}
		if (m_Motion.x < 0.0f) {
			for (const auto& i : cField::LineR) {
				float param = (i.first.x * cField::TileSize - m_Pos.x) / m_Motion.x;
				if (param >= 0.0f && param < 1.0f && lowest_param > param) {
					float tmpY = m_Pos.y + m_Motion.y * param;
					if (tmpY >= (float)(i.first.y * cField::TileSize) && tmpY <= (float)((i.first.y + i.second) * cField::TileSize)) {
						lowest_param = param;
						pos_prev = m_Pos + m_Motion * param;
						ricochetX_flag = true;
						ricochetY_flag = false;
					}
				}
			}
		}
		if (ricochetX_flag)
			m_Motion.x *= -1.0f;
		if (ricochetY_flag)
			m_Motion.y *= -1.0f;
	}
	m_Pos = pos_prev;
	m_Motion *= 0.9f;

	// 衝突判定
	if (m_Timer <= m_DetonateTime)
		return;

	for (auto& i : cGame::pVecPlayer)
	{
		if (m_Owner)
			if (i->PlayerID == m_Owner->PlayerID)
				continue;
		if (i->IsDead() || i->IsRepair())
			continue;

		if ((i->Pos.x - m_Pos.x) * (i->Pos.x - m_Pos.x) + (i->Pos.y - m_Pos.y) * (i->Pos.y - m_Pos.y) <= (m_ActiveRadius * m_ActiveRadius))
		{
			m_Timer = m_DetonateTime;
			break;
		}
	}
}

/* 描画・DEFAULT */
void cMineTrap::Draw_Default() const
{
	float radius = m_Radius * 0.15f;
	if (m_Timer < m_DetonateTime) {
		if ((m_Timer / 2) % 2)
			SetDrawAddColor(100, 100, 100);
		radius *= 1.0f + Easing::Quint::In((float)(m_DetonateTime - m_Timer) / (float)m_DetonateTime) * 0.5f;
	}

	DrawRotaGraph(
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		radius * 1.25, 0.0, m_Image_MineTrap, TRUE);

	if (m_Owner) {
		switch (m_Owner->PlayerID)
		{
		break;	case 0:	SetDrawBright(255, 60, 60);
		break;	case 1:	SetDrawBright(60, 60, 255);
		break;	case 2:	SetDrawBright(60, 255, 60);
		break;	case 3:	SetDrawBright(255, 255, 60);
		}
	}
	else
		SetDrawBright(60, 60, 60);

	DrawRotaGraph(
		cGame::ScreenDif.x + (int)m_Pos.x,
		cGame::ScreenDif.y + (int)m_Pos.y,
		radius, 0.0, m_Image_MineTrap, TRUE);
	SetDrawBright(255, 255, 255);
	SetDrawAddColor(0, 0, 0);
}

/* アクティブ状態 */
bool cMineTrap::Active() const
{
	return m_Timer >= 0;
}

/* 起動 */
void cMineTrap::Detonate()
{
	if (m_Timer <= m_DetonateTime)
		return;

	m_Timer = m_DetonateTime;
}

/* リソースの読み込み */
void cMineTrap::LoadResource()
{
	if (m_LoadedFlag)
		return;

	m_LoadedFlag = true;

	m_Image_MineTrap = LoadGraph("data\\image\\bullet\\normal.png");
}

/* リソースの解放 */
void cMineTrap::DeleteResource()
{
	if (!m_LoadedFlag)
		return;

	m_LoadedFlag = false;

	DeleteGraph(m_Image_MineTrap);	m_Image_MineTrap = -1;
}
