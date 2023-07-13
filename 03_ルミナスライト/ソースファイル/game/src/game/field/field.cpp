#include "field.h"
#include "../common_data/common_data.h"
#include "../sound_manager/sound_manager.h"
#include "../glow_screen/glow_screen.h"
#include <fstream>
#include <algorithm>

#include "../tile/tile.h"
#include "../tile/wall_tile/wall_tile.h"
#include "../tile/glass_tile/glass_tile.h"
#include "../tile/target_tile/target_tile.h"
#include "../tile/laser_tile/laser_tile.h"
#include "../tile/mirror_tile/mirror_tile.h"
#include "../tile/way_tile/way_tile.h"
#include "../tile/crystal_tile/crystal_tile.h"

const float CField::m_first_edge_space = -400.0f;
const float CField::m_laser_trail_time = 0.6f;
const int CField::m_laser_trail_frame_num = 12;

const std::map<std::string, DIRECTION_ID> CField::m_direction_table =
{
	{ "left",	DIRECTION_ID::LEFT },
	{ "right",	DIRECTION_ID::RIGHT },
	{ "up",		DIRECTION_ID::UP },
	{ "down",	DIRECTION_ID::DOWN },
};

const std::map<std::string, COLOR_ID> CField::m_color_table =
{
	{ "white",	COLOR_ID::WHITE },
	{ "red",	COLOR_ID::RED },
	{ "green",	COLOR_ID::GREEN },
	{ "blue",	COLOR_ID::BLUE },
};

CField::CField(IGameObject* parent)
	: IGameObject(parent, "Field")
	, m_pCommonData(nullptr)
	, m_pSoundManager(nullptr)
	, m_pGlowScreen(nullptr)
	, m_FieldWidth(0)
	, m_FieldHeight(0)
	, m_TileSize(0.0f)
	, m_EdgeSpace(m_first_edge_space)
	, m_TargetEdgeSpace(0.0f)
	, m_pCursorTile(nullptr)
	, m_CanControlFlag(true)
	, m_SuccessFlag(false)
{
}

void CField::Initialize()
{
	// データの解放
	Finalize();

	// ゲームオブジェクトの取得
	m_pCommonData = (CCommonData*)aqua::FindGameObject("CommonData");
	m_pSoundManager = (CSoundManager*)aqua::FindGameObject("SoundManager");
	m_pGlowScreen = (CGlowScreen*)aqua::FindGameObject("GlowScreen");

	// リソースの読み込み
	m_LaserTrailSprite.Create("data\\laser_trail.png");
	m_LaserTrailSprite.anchor.x = m_LaserTrailSprite.GetTextureWidth() / 2.0f / (int)COLOR_ID::MAX;
	m_LaserTrailSprite.anchor.y = m_LaserTrailSprite.GetTextureHeight() / 2.0f / m_laser_trail_frame_num;
	m_LaserTrailSprite.blend_mode = aqua::ALPHABLEND::ADD;

	m_OutAreaTileSprite.Create("data\\tile_out_area.png");

	m_GlowMaskSurface.Create((int)aqua::GetWindowWidth(), (int)aqua::GetWindowHeight(), true);
	m_GlowMaskSprite.Create(m_GlowMaskSurface);
	m_GlowMaskSprite.color = 0xff000000;

	// 初期化処理
	m_CursorHorizontalBox.Setup(aqua::CVector2::ZERO, (float)aqua::GetWindowWidth(), 0.0f, 0xff202020, true, 0.0f, aqua::ALPHABLEND::ADD);
	m_CursorVerticalBox.Setup(aqua::CVector2::ZERO, 0.0f, (float)aqua::GetWindowHeight(), 0xff202020, true, 0.0f, aqua::ALPHABLEND::ADD);
	m_CursorHorizontalBox.visible = false;
	m_CursorVerticalBox.visible = false;
	m_FieldWidth = 0;
	m_FieldHeight = 0;
	m_LaserTrailTimer.Setup(m_laser_trail_time);
	LoadMapData("data\\field_data\\level_" + std::to_string(m_pCommonData->GetSelectedLevel()) + ".txt");
}

void CField::Update()
{
	// タイルの表示設定
	m_EdgeSpace += (m_TargetEdgeSpace - m_EdgeSpace) * 0.04f;
	m_TileSize = min(((float)aqua::GetWindowWidth() - m_EdgeSpace) / m_FieldWidth, ((float)aqua::GetWindowHeight() - m_EdgeSpace) / m_FieldHeight);
	m_OutAreaTileSprite.scale.x = m_TileSize / (float)m_OutAreaTileSprite.GetTextureWidth();
	m_OutAreaTileSprite.scale.y = m_TileSize / (float)m_OutAreaTileSprite.GetTextureHeight();
	for (int y = 0; y < m_FieldHeight; ++y)
		for (int x = 0; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
			{
				m_pTileLocate[y][x]->SetDispSize(m_TileSize);
				m_pTileLocate[y][x]->SetPosition(aqua::CVector2((float)aqua::GetWindowWidth() - m_TileSize * m_FieldWidth, (float)aqua::GetWindowHeight() - m_TileSize * m_FieldHeight) * 0.5f + m_TileSize * aqua::CVector2((float)x, (float)y));
			}

	// カーソルの表示設定
	m_CursorHorizontalBox.height = m_TileSize;
	m_CursorVerticalBox.width = m_TileSize;

	if (!m_SuccessFlag)
	{
		// タイルの移動処理
		m_CursorLocate.x = (int)floorf((float)(aqua::mouse::GetCursorPos().x - ((int)aqua::GetWindowWidth() - m_TileSize * m_FieldWidth) / 2) / (float)m_TileSize);
		m_CursorLocate.y = (int)floorf((float)(aqua::mouse::GetCursorPos().y - ((int)aqua::GetWindowHeight() - m_TileSize * m_FieldHeight) / 2) / (float)m_TileSize);

		if (aqua::mouse::Trigger(aqua::mouse::BUTTON_ID::LEFT) && m_CanControlFlag)
		{
			if (m_CursorLocate.x >= 0 &&
				m_CursorLocate.y >= 0 &&
				m_CursorLocate.x < m_FieldWidth &&
				m_CursorLocate.y < m_FieldHeight)
				if (m_pTileLocate[m_CursorLocate.y][m_CursorLocate.x])
					if (m_pTileLocate[m_CursorLocate.y][m_CursorLocate.x]->IsMovable())
					{
						m_pCursorTile = m_pTileLocate[m_CursorLocate.y][m_CursorLocate.x];
						m_pSoundManager->Play(SOUND_ID::TILE_PICK_SE);
					}
		}

		if (aqua::mouse::Button(aqua::mouse::BUTTON_ID::LEFT) && m_pCursorTile)
		{
			std::list<aqua::CPoint> next_position_list;
			aqua::CPoint dif = m_CursorLocate - m_pCursorTile->GetLocate();
			if (dif != aqua::CPoint(0, 0))
			{
				dif.x = std::clamp(dif.x, -1, 1);
				dif.y = std::clamp(dif.y, -1, 1);

				if (abs(dif.x) > abs(dif.y))
				{
					next_position_list.push_back(m_pCursorTile->GetLocate() + aqua::CPoint(dif.x, 0));
					if (dif.y != 0)
						next_position_list.push_back(m_pCursorTile->GetLocate() + aqua::CPoint(0, dif.y));
				}
				else
				{
					next_position_list.push_back(m_pCursorTile->GetLocate() + aqua::CPoint(0, dif.y));
					if (dif.x != 0)
						next_position_list.push_back(m_pCursorTile->GetLocate() + aqua::CPoint(dif.x, 0));
				}
			}

			while (!next_position_list.empty())
			{
				if (next_position_list.front().x < 0 ||
					next_position_list.front().y < 0 ||
					next_position_list.front().x >= m_FieldWidth ||
					next_position_list.front().y >= m_FieldHeight)
				{
					next_position_list.pop_front();
					continue;
				}
				if (m_pTileLocate[next_position_list.front().y][next_position_list.front().x])
				{
					next_position_list.pop_front();
					continue;
				}
				{
					m_pTileLocate[m_pCursorTile->GetLocate().y][m_pCursorTile->GetLocate().x] = nullptr;
					m_pTileLocate[next_position_list.front().y][next_position_list.front().x] = m_pCursorTile;
					m_pCursorTile->SetLocate(next_position_list.front());
					m_pCursorTile->SetPosition(aqua::CVector2((float)aqua::GetWindowWidth() - m_TileSize * m_FieldWidth, (float)aqua::GetWindowHeight() - m_TileSize * m_FieldHeight) * 0.5f + m_TileSize * aqua::CVector2((float)next_position_list.front().x, (float)next_position_list.front().y));
					m_pSoundManager->Play(SOUND_ID::TILE_MOVE_SE);
					break;
				}
			}
		}

		if (aqua::mouse::Released(aqua::mouse::BUTTON_ID::LEFT) && m_pCursorTile)
		{
			m_pCursorTile = nullptr;
			m_pSoundManager->Play(SOUND_ID::TILE_PUT_SE);
		}
	}
	else
	{
	}

	// タイルの更新
	for (int y = 0; y < m_FieldHeight; ++y)
		for (int x = 0; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
				m_pTileLocate[y][x]->Update();

	// レーザーの軌道処理
	if (!m_SuccessFlag)
	{
		m_LaserChain.clear();

		// 軌道計算のラムダ式
		auto f_set_chain_laser = [&](auto fn, const std::pair<aqua::CPoint, SLaserData>& laser_data) -> void
		{
			aqua::CPoint next_point = laser_data.first;
			switch (laser_data.second.direction)
			{
			case DIRECTION_ID::RIGHT:	++next_point.x;	break;
			case DIRECTION_ID::DOWN:	++next_point.y;	break;
			case DIRECTION_ID::LEFT:	--next_point.x;	break;
			case DIRECTION_ID::UP:		--next_point.y;	break;
			}

			m_LaserChain.push_back({ next_point ,laser_data.second });

			if (next_point.x < 0 || next_point.y < 0 || next_point.x >= m_FieldWidth || next_point.y >= m_FieldHeight)
				return;

			if (m_pTileLocate[next_point.y][next_point.x])
			{
				if (m_pTileLocate[next_point.y][next_point.x]->GetTileID() == TILE_ID::TARGET)
					((CTargetTile*)m_pTileLocate[next_point.y][next_point.x])->IrradiateLaser(laser_data.second);

				for (auto& i : m_pTileLocate[next_point.y][next_point.x]->GetConvertedLaser(laser_data.second))
					fn(fn, { next_point, i });
			}
			else
			{
				fn(fn, { next_point, laser_data.second });
			}
		};

		// 全てのターゲットの照射フラグを下げる
		for (int y = 0; y < m_FieldHeight; ++y)
			for (int x = 0; x < m_FieldWidth; ++x)
				if (m_pTileLocate[y][x])
					if (m_pTileLocate[y][x]->GetTileID() == TILE_ID::TARGET)
						((CTargetTile*)m_pTileLocate[y][x])->SetLitFlag(false);

		// レーザーの軌道処理、ターゲットへの照射処理
		for (int y = 0; y < m_FieldHeight; ++y)
			for (int x = 0; x < m_FieldWidth; ++x)
				if (m_pTileLocate[y][x])
					if (m_pTileLocate[y][x]->GetTileID() == TILE_ID::LASER)
						for (const auto& i : ((CLaserTile*)m_pTileLocate[y][x])->GetGeneratedLaser())
							f_set_chain_laser(f_set_chain_laser, { aqua::CPoint(x, y), i });
	}

	// 成功判定
	m_SuccessFlag = true;
	for (int y = 0; y < m_FieldHeight; ++y)
		for (int x = 0; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
				if (m_pTileLocate[y][x]->GetTileID() == TILE_ID::TARGET)
					m_SuccessFlag &= ((CTargetTile*)m_pTileLocate[y][x])->GetLitFlag();
	if (m_SuccessFlag)
		if (m_pCursorTile)
		{
			m_pCursorTile = nullptr;
			m_pSoundManager->Play(SOUND_ID::TILE_PUT_SE);
		}

	// タイマー処理
	m_LaserTrailTimer.Update();
	if (m_LaserTrailTimer.Finished())
		m_LaserTrailTimer.Reset();

	// カーソル更新
	m_CursorHorizontalBox.visible = m_pCursorTile;
	m_CursorVerticalBox.visible = m_pCursorTile;
	if (!m_SuccessFlag)
	{
		m_CursorHorizontalBox.position.y = (aqua::GetWindowHeight() - m_TileSize * m_FieldHeight) / 2.0f + m_TileSize * m_CursorLocate.y;
		m_CursorVerticalBox.position.x = (aqua::GetWindowWidth() - m_TileSize * m_FieldWidth) / 2.0f + m_TileSize * m_CursorLocate.x;
	}

	// グロー効果付与の描画
	m_pGlowScreen->Begin();
	{
		// レーザー
		m_LaserTrailSprite.scale.x = m_TileSize / (float)m_LaserTrailSprite.GetTextureWidth() * (int)COLOR_ID::MAX;
		m_LaserTrailSprite.scale.y = m_TileSize / (float)(m_LaserTrailSprite.GetTextureHeight() / m_laser_trail_frame_num);
		m_LaserTrailSprite.rect.top = (m_LaserTrailSprite.GetTextureHeight() / m_laser_trail_frame_num) * (int)(m_LaserTrailTimer.GetTime() * m_laser_trail_frame_num / m_laser_trail_time);
		m_LaserTrailSprite.rect.bottom = m_LaserTrailSprite.rect.top + m_LaserTrailSprite.GetTextureHeight() / m_laser_trail_frame_num;
		for (const auto& i : m_LaserChain)
		{
			m_LaserTrailSprite.rect.left = m_LaserTrailSprite.GetTextureWidth() / (int)COLOR_ID::MAX * (int)i.second.color;
			m_LaserTrailSprite.rect.right = m_LaserTrailSprite.rect.left + m_LaserTrailSprite.GetTextureWidth() / (int)COLOR_ID::MAX;
			m_LaserTrailSprite.position.x = ((int)aqua::GetWindowWidth() - m_TileSize * m_FieldWidth) / 2.0f + m_TileSize * i.first.x + m_TileSize / 2.0f - m_LaserTrailSprite.anchor.x;
			m_LaserTrailSprite.position.y = ((int)aqua::GetWindowHeight() - m_TileSize * m_FieldHeight) / 2.0f + m_TileSize * i.first.y + m_TileSize / 2.0f - m_LaserTrailSprite.anchor.y;
			m_LaserTrailSprite.rotation = aqua::DegToRad(90.0f * (int)i.second.direction);

			if (i.first.x >= 0 && i.first.y >= 0 && i.first.x < m_FieldWidth && i.first.y < m_FieldHeight)
				m_pTileLocate[i.first.y][i.first.x] ?
				m_pTileLocate[i.first.y][i.first.x]->DrawLaserTrail(m_LaserTrailSprite, i.second) :
				m_LaserTrailSprite.Draw();
		}

		// 照射中のターゲット
		for (int y = 0; y < m_FieldHeight; ++y)
			for (int x = 0; x < m_FieldWidth; ++x)
				if (m_pTileLocate[y][x])
					if (m_pTileLocate[y][x]->GetTileID() == TILE_ID::TARGET)
						((CTargetTile*)m_pTileLocate[y][x])->DrawLit();
	}
	m_pGlowScreen->End();
}

void CField::Draw()
{
	// 範囲外のタイルの描画
	{
		int sx = (int)floorf((m_TileSize * m_FieldWidth - (float)aqua::GetWindowWidth()) / 2.0f / m_TileSize);
		int sy = (int)floorf((m_TileSize * m_FieldHeight - (float)aqua::GetWindowHeight()) / 2.0f / m_TileSize);
		int ex = m_FieldWidth + (int)ceilf(((float)aqua::GetWindowWidth() - m_TileSize * m_FieldWidth) / 2.0f / m_TileSize);
		int ey = m_FieldHeight + (int)ceilf(((float)aqua::GetWindowHeight() - m_TileSize * m_FieldHeight) / 2.0f / m_TileSize);
		for (int x = sx; x < ex; ++x)
			for (int y = sy; y < ey; ++y)
				if (x < 0 || y < 0 || x >= m_FieldWidth || y >= m_FieldHeight)
				{
					m_OutAreaTileSprite.position = aqua::CVector2((float)aqua::GetWindowWidth() - m_TileSize * m_FieldWidth, (float)aqua::GetWindowHeight() - m_TileSize * m_FieldHeight) * 0.5f + m_TileSize * aqua::CVector2((float)x, (float)y);
					m_OutAreaTileSprite.Draw();
				}
	}

	// 各タイルの描画
	for (int y = 0; y < m_FieldHeight; ++y)
		for (int x = 0; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
				m_pTileLocate[y][x]->Draw();

	// カーソル
	if (m_CanControlFlag && !m_SuccessFlag)
	{
		if (m_pCursorTile)
		{
			if (m_pCursorTile->IsMovable())
				m_pCursorTile->DrawOutLine();
		}
		else if (m_CursorLocate.x >= 0 && m_CursorLocate.y >= 0 && m_CursorLocate.x < m_FieldWidth && m_CursorLocate.y < m_FieldHeight)
		{
			if (m_pTileLocate[m_CursorLocate.y][m_CursorLocate.x])
				if (m_pTileLocate[m_CursorLocate.y][m_CursorLocate.x]->IsMovable())
					m_pTileLocate[m_CursorLocate.y][m_CursorLocate.x]->DrawOutLine();
		}
	}
	m_CursorHorizontalBox.Draw();
	m_CursorVerticalBox.Draw();
}

void CField::Finalize()
{
	m_LaserTrailSprite.Delete();
	m_OutAreaTileSprite.Delete();
	m_GlowMaskSurface.Delete();
	m_GlowMaskSprite.Delete();

	for (auto& y : m_pTileLocate)
		for (auto& x : y)
			if (x)
			{
				x->Finalize();
				delete x;
			}
	m_pTileLocate.clear();
}

TILE_ID CField::GetTileID(const aqua::CPoint& locate) const
{
	if (locate.x < 0 || locate.y < 0 || locate.x >= m_FieldWidth || locate.y >= m_FieldHeight)
		return TILE_ID::MAX;

	if (!m_pTileLocate[locate.y][locate.x])
		return TILE_ID::EMPTY;

	return m_pTileLocate[locate.y][locate.x]->GetTileID();
}

bool CField::CheckSuccess() const
{
	return m_SuccessFlag;
}

float CField::GetTileSize() const
{
	return m_TileSize;
}

void CField::SetTargetEdgeSpace(float space)
{
	m_TargetEdgeSpace = space;
}

void CField::SetControlFlag(bool flag)
{
	m_CanControlFlag = flag;
}

void CField::LoadMapData(const std::string& file_name)
{
	// カンマ区切りで文字列を分割するラムダ式
	auto f_split = [](std::string str) -> std::vector<std::string>
	{
		std::vector<std::string> str_list;

		for (int i = 0; i < (int)str.size();)
			if (str[i] == ',')
			{
				str_list.push_back(str.substr(0, i));
				str.erase(0, i + 1);
				i = 0;
			}
			else
				++i;
		str_list.push_back(str);

		return str_list;
	};

	// ファイルを開く
	std::ifstream ifs(file_name);
	if (ifs.fail())
		return;
	std::string buf;

	// フィールドの大きさを取得
	std::getline(ifs, buf);	m_FieldWidth = atoi(buf.c_str());
	std::getline(ifs, buf);	m_FieldHeight = atoi(buf.c_str());
	m_pTileLocate.assign(m_FieldHeight, std::vector<ITile*>(m_FieldWidth, nullptr));

	// テキストファイルからタイル配置
	std::list<std::pair<char, ITile*>> special_tile_list;
	for (int y = 0; y < m_FieldHeight; ++y)
	{
		std::getline(ifs, buf);
		for (int x = 0; x < m_FieldWidth; ++x)
		{
			switch (buf[x])
			{
			case '#':
				m_pTileLocate[y][x] = aqua::CreateGameObject<CWallTile>(this);
				m_pTileLocate[y][x]->Initialize();
				break;
			case '-':
				m_pTileLocate[y][x] = aqua::CreateGameObject<CGlassTile>(this);
				m_pTileLocate[y][x]->Initialize();
				break;
			case 't':
				m_pTileLocate[y][x] = aqua::CreateGameObject<CTargetTile>(this);
				special_tile_list.push_back({ buf[x], m_pTileLocate[y][x] });
				break;
			case 'l':
				m_pTileLocate[y][x] = aqua::CreateGameObject<CLaserTile>(this);
				special_tile_list.push_back({ buf[x], m_pTileLocate[y][x] });
				break;
			case 'm':
				m_pTileLocate[y][x] = aqua::CreateGameObject<CMirrorTile>(this);
				special_tile_list.push_back({ buf[x], m_pTileLocate[y][x] });
				break;
			case 'w':
				m_pTileLocate[y][x] = aqua::CreateGameObject<CWayTile>(this);
				special_tile_list.push_back({ buf[x], m_pTileLocate[y][x] });
				break;
			case 'c':
				m_pTileLocate[y][x] = aqua::CreateGameObject<CCrystalTile>(this);
				special_tile_list.push_back({ buf[x], m_pTileLocate[y][x] });
				break;
			}
		}
	}

	// 特殊タイルの初期化
	for (auto& i : special_tile_list)
	{
		std::getline(ifs, buf);
		std::vector<std::string> split_buf = f_split(buf);

		switch (i.first)
		{
		case 't':
			((CTargetTile*)i.second)->Initialize(m_direction_table.at(split_buf[0]), m_color_table.at(split_buf[1]));
			break;
		case 'l':
			((CLaserTile*)i.second)->Initialize(m_direction_table.at(split_buf[0]));
			break;
		case 'm':
			((CMirrorTile*)i.second)->Initialize(m_direction_table.at(split_buf[0]));
			break;
		case 'w':
			((CWayTile*)i.second)->Initialize(m_direction_table.at(split_buf[0]));
			break;
		case 'c':
			((CCrystalTile*)i.second)->Initialize(m_color_table.at(split_buf[0]));
			break;
		}
	}

	// タイルの位置設定
	for (int y = 0; y < m_FieldHeight; ++y)
		for (int x = 0; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
				m_pTileLocate[y][x]->SetLocate(aqua::CPoint(x, y));

	// ファイルを閉じる
	ifs.close();
}
