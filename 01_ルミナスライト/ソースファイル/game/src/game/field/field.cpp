#include "field.h"
#include "../common_data/common_data.h"
#include "../sound_manager/sound_manager.h"
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
	, m_CanControl(true)
	, m_IsSuccess(false)
	, m_TileNumX(0)
	, m_TileNumY(0)
	, m_DispTileSize(0.0f)
	, m_EdgeSpace(m_first_edge_space)
	, m_TargetEdgeSpace(0.0f)
	, m_CursorLocate()
	, m_pCursoredTile(nullptr)
	, m_pTiles()
	, m_LaserChain()
{
}

void CField::Initialize()
{
	// データの解放
	Finalize();

	// ゲームオブジェクトの取得
	m_pCommonData = (CCommonData*)aqua::FindGameObject("CommonData");
	m_pSoundManager = (CSoundManager*)aqua::FindGameObject("SoundManager");

	// リソースの読み込み
	m_LaserTrailSprite.Create("data\\laser_trail.ass");
	m_LaserTrailSprite.anchor.x = m_LaserTrailSprite.GetFrameWidth() / 2.0f;
	m_LaserTrailSprite.anchor.y = m_LaserTrailSprite.GetFrameHeight() / 2.0f;
	m_LaserTrailSprite.blend_mode = aqua::ALPHABLEND::ADD;

	m_OutAreaTileSprite.Create("data\\tile_out_area.png");

	// 初期化処理
	m_CursorHorizontalBox.Setup(aqua::CVector2::ZERO, (float)aqua::GetWindowWidth(), 0.0f, 0xff202020, true, 0.0f, aqua::ALPHABLEND::ADD);
	m_CursorVerticalBox.Setup(aqua::CVector2::ZERO, 0.0f, (float)aqua::GetWindowHeight(), 0xff202020, true, 0.0f, aqua::ALPHABLEND::ADD);
	m_CursorHorizontalBox.visible = false;
	m_CursorVerticalBox.visible = false;
	m_TileNumX = 0;
	m_TileNumY = 0;
	LoadMapData("data\\field_data\\level_" + std::to_string(m_pCommonData->selectedLevel) + ".txt");
}

void CField::Update()
{
	Update_01_TileDrawSettings();
	Update_02_MovementTile();
	Update_03_Tiles();
	Update_04_CalcLaser();
	Update_05_CheckSuccess();
	Update_06_CursorDrawSettings();
	Update_07_Animations();
}

void CField::Draw()
{
	// 範囲外のタイル
	{
		int sx = (int)floorf((m_DispTileSize * m_TileNumX - (float)aqua::GetWindowWidth()) / 2.0f / m_DispTileSize);
		int sy = (int)floorf((m_DispTileSize * m_TileNumY - (float)aqua::GetWindowHeight()) / 2.0f / m_DispTileSize);
		int ex = m_TileNumX + (int)ceilf(((float)aqua::GetWindowWidth() - m_DispTileSize * m_TileNumX) / 2.0f / m_DispTileSize);
		int ey = m_TileNumY + (int)ceilf(((float)aqua::GetWindowHeight() - m_DispTileSize * m_TileNumY) / 2.0f / m_DispTileSize);
		for (int x = sx; x < ex; ++x)
			for (int y = sy; y < ey; ++y)
				if (x < 0 || y < 0 || x >= m_TileNumX || y >= m_TileNumY)
				{
					m_OutAreaTileSprite.position = aqua::CVector2((float)aqua::GetWindowWidth() - m_DispTileSize * m_TileNumX, (float)aqua::GetWindowHeight() - m_DispTileSize * m_TileNumY) * 0.5f + m_DispTileSize * aqua::CVector2((float)x, (float)y);
					m_OutAreaTileSprite.Draw();
				}
	}

	// タイル
	for (int y = 0; y < m_TileNumY; ++y)
		for (int x = 0; x < m_TileNumX; ++x)
			if (m_pTiles[y][x])
				m_pTiles[y][x]->Draw();

	// カーソル
	if (m_CanControl && !m_IsSuccess)
	{
		if (m_pCursoredTile)
		{
			if (m_pCursoredTile->IsMovable())
				m_pCursoredTile->DrawOutLine();
		}
		else if (m_CursorLocate.x >= 0 && m_CursorLocate.y >= 0 && m_CursorLocate.x < m_TileNumX && m_CursorLocate.y < m_TileNumY)
		{
			if (m_pTiles[m_CursorLocate.y][m_CursorLocate.x])
				if (m_pTiles[m_CursorLocate.y][m_CursorLocate.x]->IsMovable())
					m_pTiles[m_CursorLocate.y][m_CursorLocate.x]->DrawOutLine();
		}
	}
	m_CursorHorizontalBox.Draw();
	m_CursorVerticalBox.Draw();
}

void CField::Draw_Lit()
{
	// レーザー
	m_LaserTrailSprite.scale.x = m_DispTileSize / (float)m_LaserTrailSprite.GetFrameWidth();
	m_LaserTrailSprite.scale.y = m_DispTileSize / (float)m_LaserTrailSprite.GetFrameHeight();
	for (const auto& i : m_LaserChain)
	{
		m_LaserTrailSprite.position.x = ((int)aqua::GetWindowWidth() - m_DispTileSize * m_TileNumX) / 2.0f + m_DispTileSize * i.first.x + m_DispTileSize / 2.0f - m_LaserTrailSprite.anchor.x;
		m_LaserTrailSprite.position.y = ((int)aqua::GetWindowHeight() - m_DispTileSize * m_TileNumY) / 2.0f + m_DispTileSize * i.first.y + m_DispTileSize / 2.0f - m_LaserTrailSprite.anchor.y;
		m_LaserTrailSprite.rotation = aqua::DegToRad(90.0f * (int)i.second.direction);
		m_LaserTrailSprite.Change((int)i.second.color, false);

		if (i.first.x >= 0 && i.first.y >= 0 && i.first.x < m_TileNumX && i.first.y < m_TileNumY && m_pTiles[i.first.y][i.first.x])
			m_pTiles[i.first.y][i.first.x]->DrawLaserTrail(m_LaserTrailSprite, i.second);
		else
			m_LaserTrailSprite.Draw();
	}

	// タイル
	for (int y = 0; y < m_TileNumY; ++y)
		for (int x = 0; x < m_TileNumX; ++x)
			if (m_pTiles[y][x])
				(m_pTiles[y][x])->Draw_Lit();
}

void CField::Finalize()
{
	m_LaserTrailSprite.Delete();
	m_OutAreaTileSprite.Delete();

	for (auto& y : m_pTiles)
		for (auto& x : y)
			if (x)
			{
				x->Finalize();
				delete x;
			}
	m_pTiles.clear();
}

TILE_ID CField::GetTileID(const aqua::CPoint& locate) const
{
	if (locate.x < 0 || locate.y < 0 || locate.x >= m_TileNumX || locate.y >= m_TileNumY)
		return TILE_ID::MAX;

	if (!m_pTiles[locate.y][locate.x])
		return TILE_ID::EMPTY;

	return m_pTiles[locate.y][locate.x]->GetTileID();
}

bool CField::IsSuccess() const
{
	return m_IsSuccess;
}

float CField::GetTileSize() const
{
	return m_DispTileSize;
}

void CField::SetTargetEdgeSpace(float space)
{
	m_TargetEdgeSpace = space;
}

void CField::SetControlFlag(bool flag)
{
	m_CanControl = flag;
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
	std::getline(ifs, buf);	m_TileNumX = atoi(buf.c_str());
	std::getline(ifs, buf);	m_TileNumY = atoi(buf.c_str());
	m_pTiles.assign(m_TileNumY, std::vector<ITile*>(m_TileNumX, nullptr));

	// テキストファイルからタイル配置
	std::list<std::pair<char, ITile*>> special_tile_list;
	for (int y = 0; y < m_TileNumY; ++y)
	{
		std::getline(ifs, buf);
		for (int x = 0; x < m_TileNumX; ++x)
		{
			switch (buf[x])
			{
			case '#':
				m_pTiles[y][x] = aqua::CreateGameObject<CWallTile>(this);
				m_pTiles[y][x]->Initialize();
				break;
			case '-':
				m_pTiles[y][x] = aqua::CreateGameObject<CGlassTile>(this);
				m_pTiles[y][x]->Initialize();
				break;
			case 't':
				m_pTiles[y][x] = aqua::CreateGameObject<CTargetTile>(this);
				special_tile_list.push_back({ buf[x], m_pTiles[y][x] });
				break;
			case 'l':
				m_pTiles[y][x] = aqua::CreateGameObject<CLaserTile>(this);
				special_tile_list.push_back({ buf[x], m_pTiles[y][x] });
				break;
			case 'm':
				m_pTiles[y][x] = aqua::CreateGameObject<CMirrorTile>(this);
				special_tile_list.push_back({ buf[x], m_pTiles[y][x] });
				break;
			case 'w':
				m_pTiles[y][x] = aqua::CreateGameObject<CWayTile>(this);
				special_tile_list.push_back({ buf[x], m_pTiles[y][x] });
				break;
			case 'c':
				m_pTiles[y][x] = aqua::CreateGameObject<CCrystalTile>(this);
				special_tile_list.push_back({ buf[x], m_pTiles[y][x] });
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
	for (int y = 0; y < m_TileNumY; ++y)
		for (int x = 0; x < m_TileNumX; ++x)
			if (m_pTiles[y][x])
				m_pTiles[y][x]->SetLocate(aqua::CPoint(x, y));

	// ファイルを閉じる
	ifs.close();
}

void CField::Update_01_TileDrawSettings()
{
	m_EdgeSpace += (m_TargetEdgeSpace - m_EdgeSpace) * 0.04f;
	m_DispTileSize = min(((float)aqua::GetWindowWidth() - m_EdgeSpace) / m_TileNumX, ((float)aqua::GetWindowHeight() - m_EdgeSpace) / m_TileNumY);
	m_OutAreaTileSprite.scale.x = m_DispTileSize / (float)m_OutAreaTileSprite.GetTextureWidth();
	m_OutAreaTileSprite.scale.y = m_DispTileSize / (float)m_OutAreaTileSprite.GetTextureHeight();
}

void CField::Update_02_MovementTile()
{
	if (!m_IsSuccess)
	{
		m_CursorLocate.x = (int)floorf((float)(aqua::mouse::GetCursorPos().x - ((int)aqua::GetWindowWidth() - m_DispTileSize * m_TileNumX) / 2) / (float)m_DispTileSize);
		m_CursorLocate.y = (int)floorf((float)(aqua::mouse::GetCursorPos().y - ((int)aqua::GetWindowHeight() - m_DispTileSize * m_TileNumY) / 2) / (float)m_DispTileSize);

		if (aqua::mouse::Trigger(aqua::mouse::BUTTON_ID::LEFT) && m_CanControl)
		{
			if (m_CursorLocate.x >= 0 &&
				m_CursorLocate.y >= 0 &&
				m_CursorLocate.x < m_TileNumX &&
				m_CursorLocate.y < m_TileNumY)
				if (m_pTiles[m_CursorLocate.y][m_CursorLocate.x])
					if (m_pTiles[m_CursorLocate.y][m_CursorLocate.x]->IsMovable())
					{
						m_pCursoredTile = m_pTiles[m_CursorLocate.y][m_CursorLocate.x];
						m_pSoundManager->Play(SOUND_ID::TILE_PICK_SE);
					}
		}

		if (aqua::mouse::Button(aqua::mouse::BUTTON_ID::LEFT) && m_pCursoredTile)
		{
			std::list<aqua::CPoint> next_position_list;
			aqua::CPoint dif = m_CursorLocate - m_pCursoredTile->GetLocate();
			if (dif != aqua::CPoint(0, 0))
			{
				dif.x = std::clamp(dif.x, -1, 1);
				dif.y = std::clamp(dif.y, -1, 1);

				if (abs(dif.x) > abs(dif.y))
				{
					next_position_list.push_back(m_pCursoredTile->GetLocate() + aqua::CPoint(dif.x, 0));
					if (dif.y != 0)
						next_position_list.push_back(m_pCursoredTile->GetLocate() + aqua::CPoint(0, dif.y));
				}
				else
				{
					next_position_list.push_back(m_pCursoredTile->GetLocate() + aqua::CPoint(0, dif.y));
					if (dif.x != 0)
						next_position_list.push_back(m_pCursoredTile->GetLocate() + aqua::CPoint(dif.x, 0));
				}
			}

			while (!next_position_list.empty())
			{
				if (next_position_list.front().x < 0 ||
					next_position_list.front().y < 0 ||
					next_position_list.front().x >= m_TileNumX ||
					next_position_list.front().y >= m_TileNumY)
				{
					next_position_list.pop_front();
					continue;
				}
				if (m_pTiles[next_position_list.front().y][next_position_list.front().x])
				{
					next_position_list.pop_front();
					continue;
				}
				{
					m_pTiles[m_pCursoredTile->GetLocate().y][m_pCursoredTile->GetLocate().x] = nullptr;
					m_pTiles[next_position_list.front().y][next_position_list.front().x] = m_pCursoredTile;
					m_pCursoredTile->SetLocate(next_position_list.front());
					m_pCursoredTile->SetPosition(aqua::CVector2((float)aqua::GetWindowWidth() - m_DispTileSize * m_TileNumX, (float)aqua::GetWindowHeight() - m_DispTileSize * m_TileNumY) * 0.5f + m_DispTileSize * aqua::CVector2((float)next_position_list.front().x, (float)next_position_list.front().y));
					m_pSoundManager->Play(SOUND_ID::TILE_MOVE_SE);
					break;
				}
			}
		}

		if (aqua::mouse::Released(aqua::mouse::BUTTON_ID::LEFT) && m_pCursoredTile)
		{
			m_pCursoredTile = nullptr;
			m_pSoundManager->Play(SOUND_ID::TILE_PUT_SE);
		}
	}
}

void CField::Update_03_Tiles()
{
	for (int y = 0; y < m_TileNumY; ++y)
		for (int x = 0; x < m_TileNumX; ++x)
			if (m_pTiles[y][x])
			{
				m_pTiles[y][x]->SetDispSize(m_DispTileSize);
				m_pTiles[y][x]->SetPosition(aqua::CVector2((float)aqua::GetWindowWidth() - m_DispTileSize * m_TileNumX, (float)aqua::GetWindowHeight() - m_DispTileSize * m_TileNumY) * 0.5f + m_DispTileSize * aqua::CVector2((float)x, (float)y));
				m_pTiles[y][x]->Update();
			}
}

void CField::Update_04_CalcLaser()
{
	if (!m_IsSuccess)
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

			if (next_point.x < 0 || next_point.y < 0 || next_point.x >= m_TileNumX || next_point.y >= m_TileNumY)
				return;

			if (m_pTiles[next_point.y][next_point.x])
			{
				if (m_pTiles[next_point.y][next_point.x]->GetTileID() == TILE_ID::TARGET)
					((CTargetTile*)m_pTiles[next_point.y][next_point.x])->IrradiateLaser(laser_data.second);

				for (auto& i : m_pTiles[next_point.y][next_point.x]->GetConvertedLaser(laser_data.second))
					fn(fn, { next_point, i });
			}
			else
			{
				fn(fn, { next_point, laser_data.second });
			}
		};

		// 全てのターゲットの照射フラグを下げる
		for (int y = 0; y < m_TileNumY; ++y)
			for (int x = 0; x < m_TileNumX; ++x)
				if (m_pTiles[y][x])
					if (m_pTiles[y][x]->GetTileID() == TILE_ID::TARGET)
						((CTargetTile*)m_pTiles[y][x])->SetLitFlag(false);

		// レーザーの軌道処理、ターゲットへの照射処理
		for (int y = 0; y < m_TileNumY; ++y)
			for (int x = 0; x < m_TileNumX; ++x)
				if (m_pTiles[y][x])
					if (m_pTiles[y][x]->GetTileID() == TILE_ID::LASER)
						for (const auto& i : ((CLaserTile*)m_pTiles[y][x])->GetGeneratedLaser())
							f_set_chain_laser(f_set_chain_laser, { aqua::CPoint(x, y), i });
	}
}

void CField::Update_05_CheckSuccess()
{
	m_IsSuccess = true;
	for (int y = 0; y < m_TileNumY; ++y)
		for (int x = 0; x < m_TileNumX; ++x)
			if (m_pTiles[y][x])
				if (m_pTiles[y][x]->GetTileID() == TILE_ID::TARGET)
					m_IsSuccess &= ((CTargetTile*)m_pTiles[y][x])->IsLit();
	if (m_IsSuccess)
		if (m_pCursoredTile)
		{
			m_pCursoredTile = nullptr;
			m_pSoundManager->Play(SOUND_ID::TILE_PUT_SE);
		}
}

void CField::Update_06_CursorDrawSettings()
{
	m_CursorHorizontalBox.visible = m_pCursoredTile;
	m_CursorVerticalBox.visible = m_pCursoredTile;
	if (!m_IsSuccess)
	{
		m_CursorHorizontalBox.height = m_DispTileSize;
		m_CursorVerticalBox.width = m_DispTileSize;
		m_CursorHorizontalBox.position.y = (aqua::GetWindowHeight() - m_DispTileSize * m_TileNumY) / 2.0f + m_DispTileSize * m_CursorLocate.y;
		m_CursorVerticalBox.position.x = (aqua::GetWindowWidth() - m_DispTileSize * m_TileNumX) / 2.0f + m_DispTileSize * m_CursorLocate.x;
	}
}

void CField::Update_07_Animations()
{
	m_LaserTrailSprite.Update();
}
