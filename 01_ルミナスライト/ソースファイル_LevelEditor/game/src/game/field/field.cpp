#include "field.h"
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

const float CField::m_edge_space = 200.0f;
const int CField::m_tile_menu_num = 13;

const std::map<std::string, DIRECTION_ID> CField::m_direction_table_to_id =
{
	{ "left",	DIRECTION_ID::LEFT },
	{ "right",	DIRECTION_ID::RIGHT  },
	{ "up",		DIRECTION_ID::UP },
	{ "down",	DIRECTION_ID::DOWN },
};

const std::map<DIRECTION_ID, std::string> CField::m_direction_table_to_name =
{
	{ DIRECTION_ID::LEFT,	"left" },
	{ DIRECTION_ID::RIGHT,	"right" },
	{ DIRECTION_ID::UP,		"up" },
	{ DIRECTION_ID::DOWN,	"down" },
};

const std::map<std::string, COLOR_ID> CField::m_color_table_to_id =
{
	{ "white",	COLOR_ID::WHITE },
	{ "red",	COLOR_ID::RED },
	{ "green",	COLOR_ID::GREEN },
	{ "blue",	COLOR_ID::BLUE },
};

const std::map<COLOR_ID, std::string> CField::m_color_table_to_name =
{
	{ COLOR_ID::WHITE,	"white", },
	{ COLOR_ID::RED,	"red", },
	{ COLOR_ID::GREEN,	"green", },
	{ COLOR_ID::BLUE,	"blue", },
};

const std::vector<std::pair<TILE_ID, COLOR_ID>> CField::m_tile_menu =
{
	{ TILE_ID::WALL,	COLOR_ID::WHITE },
	{ TILE_ID::GLASS,	COLOR_ID::WHITE },
	{ TILE_ID::LASER,	COLOR_ID::WHITE },
	{ TILE_ID::TARGET,	COLOR_ID::WHITE },
	{ TILE_ID::TARGET,	COLOR_ID::RED },
	{ TILE_ID::TARGET,	COLOR_ID::GREEN },
	{ TILE_ID::TARGET,	COLOR_ID::BLUE },
	{ TILE_ID::MIRROR,	COLOR_ID::WHITE },
	{ TILE_ID::WAY,		COLOR_ID::WHITE },
	{ TILE_ID::CRYSTAL,	COLOR_ID::WHITE },
	{ TILE_ID::CRYSTAL,	COLOR_ID::RED },
	{ TILE_ID::CRYSTAL,	COLOR_ID::GREEN },
	{ TILE_ID::CRYSTAL,	COLOR_ID::BLUE },
};

CField::CField(IGameObject* parent)
	: IGameObject(parent, "Field")
	, m_pSoundManager(nullptr)
	, m_TileNumX(0)
	, m_TileNumY(0)
	, m_SelectedTileMenu(0)
	, m_DispTileSize(0.0f)
	, m_SelectedDirection(DIRECTION_ID::RIGHT)
	, m_CursorLocate()
	, m_CursorLocate_prev()
	, m_LoadFilePath("")
	, m_pTiles()
	, m_LaserChain()
{
}

void CField::Initialize()
{
	// データの解放
	Finalize();

	// ゲームオブジェクトの取得
	m_pSoundManager = (CSoundManager*)aqua::FindGameObject("SoundManager");

	// リソースの読み込み
	m_LaserTrailSprite.Create("data\\laser_trail.ass");
	m_LaserTrailSprite.anchor.x = m_LaserTrailSprite.GetFrameWidth() / 2.0f;
	m_LaserTrailSprite.anchor.y = m_LaserTrailSprite.GetFrameHeight() / 2.0f;
	m_LaserTrailSprite.blend_mode = aqua::ALPHABLEND::ADD;

	m_OutAreaTileSprite.Create("data\\tile_out_area.png");

	m_TileMenuSprite.Create("data\\tile_menu.png");
	m_TileMenuSprite.anchor.x = m_TileMenuSprite.GetTextureWidth() / 2.0f / m_tile_menu_num;
	m_TileMenuSprite.anchor.y = m_TileMenuSprite.GetTextureHeight() / 2.0f;
	m_TileMenuSprite.scale.x = 4.0f;
	m_TileMenuSprite.scale.y = 4.0f;
	m_TileMenuSprite.position = m_TileMenuSprite.anchor * m_TileMenuSprite.scale - m_TileMenuSprite.anchor;

	// 初期化処理
	m_CursorHorizontalBox.Setup(aqua::CVector2::ZERO, (float)aqua::GetWindowWidth(), 0.0f, 0xff202020, true, 0.0f, aqua::ALPHABLEND::ADD);
	m_CursorVerticalBox.Setup(aqua::CVector2::ZERO, 0.0f, (float)aqua::GetWindowHeight(), 0xff202020, true, 0.0f, aqua::ALPHABLEND::ADD);
	m_TileNumX = 0;
	m_TileNumY = 0;
	ResizeField(8, 6);
}

void CField::Update()
{
	Update_01_LoadDragFileMapData();
	Update_02_ChangeTileMenu();
	Update_03_ChangeTileNum();
	Update_04_TileDrawSettings();
	Update_05_PlaceAndEraseTile();
	Update_06_SaveMapData();
	Update_07_Tiles();
	Update_08_CalcLaser();
	Update_09_CursorDrawSettings();
	Update_10_Animations();
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
	m_CursorHorizontalBox.Draw();
	m_CursorVerticalBox.Draw();

	// 設置タイル
	m_TileMenuSprite.Draw();

	// テキスト
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetDrawBright(255, 255, 255);
	DrawFormatString(16, (int)aqua::GetWindowHeight() - 208, 0xffffffff, "左クリック : タイルの設置");
	DrawFormatString(16, (int)aqua::GetWindowHeight() - 192, 0xffffffff, "右クリック : タイルの削除");
	DrawFormatString(16, (int)aqua::GetWindowHeight() - 160, 0xffffffff, "![Shift]+MouseWheel : 設置タイルの変更");
	DrawFormatString(16, (int)aqua::GetWindowHeight() - 144, 0xffffffff, "[Shift]+MouseWheel  : 設置タイルの回転");
	DrawFormatString(16, (int)aqua::GetWindowHeight() - 112, 0xffffffff, "[→] : 横タイル数+1");
	DrawFormatString(16, (int)aqua::GetWindowHeight() - 96, 0xffffffff, "[←] : 横タイル数-1");
	DrawFormatString(16, (int)aqua::GetWindowHeight() - 80, 0xffffffff, "[↓] : 縦タイル数+1");
	DrawFormatString(16, (int)aqua::GetWindowHeight() - 64, 0xffffffff, "[↑] : 縦タイル数-1");
	DrawFormatString(16, (int)aqua::GetWindowHeight() - 32, 0xffffffff, "[Ctrl]+[S] : セーブ");
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
	m_TileMenuSprite.Delete();

	for (auto& y : m_pTiles)
		for (auto& x : y)
			if (x)
			{
				x->Finalize();
				AQUA_SAFE_DELETE(x);
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

float CField::GetTileSize() const
{
	return m_DispTileSize;
}

void CField::ResizeField(int field_width, int field_height)
{
	// 範囲外のタイルを削除
	for (int y = field_height; y < m_TileNumY; ++y)
		for (int x = field_width; x < m_TileNumX; ++x)
			if (m_pTiles[y][x])
			{
				m_pTiles[y][x]->Finalize();
				m_ChildObjectList.erase(std::find(m_ChildObjectList.begin(), m_ChildObjectList.end(), m_pTiles[y][x]));
				AQUA_SAFE_DELETE(m_pTiles[y][x]);
			}

	// 変更後のフィールドを生成
	std::vector<std::vector<ITile*>> new_tile_locate(field_height, std::vector<ITile*>(field_width, nullptr));
	for (int y = 0; y < field_height; ++y)
		for (int x = 0; x < field_width; ++x)
			if (x < m_TileNumX && y < m_TileNumY)
				new_tile_locate[y][x] = m_pTiles[y][x];

	// 変更後のフィールドを適用
	m_TileNumX = field_width;
	m_TileNumY = field_height;
	m_pTiles = new_tile_locate;
}

void CField::DeleteTile(const aqua::CPoint& locate)
{
	if (locate.x < 0 || locate.y < 0 || locate.x >= m_TileNumX || locate.y >= m_TileNumY)
		return;

	if (m_pTiles[locate.y][locate.x])
	{
		m_pTiles[locate.y][locate.x]->Finalize();
		m_ChildObjectList.erase(std::find(m_ChildObjectList.begin(), m_ChildObjectList.end(), m_pTiles[locate.y][locate.x]));
		AQUA_SAFE_DELETE(m_pTiles[locate.y][locate.x]);
	}
}

void CField::ReplaceTile(const aqua::CPoint& locate)
{
	if (locate.x < 0 || locate.y < 0 || locate.x >= m_TileNumX || locate.y >= m_TileNumY)
		return;

	DeleteTile(locate);

	switch (m_tile_menu[m_SelectedTileMenu].first)
	{
	case TILE_ID::WALL:
		m_pTiles[locate.y][locate.x] = aqua::CreateGameObject<CWallTile>(this);
		((CWallTile*)m_pTiles[locate.y][locate.x])->Initialize();
		break;
	case TILE_ID::GLASS:
		m_pTiles[locate.y][locate.x] = aqua::CreateGameObject<CGlassTile>(this);
		((CGlassTile*)m_pTiles[locate.y][locate.x])->Initialize();
		break;
	case TILE_ID::LASER:
		m_pTiles[locate.y][locate.x] = aqua::CreateGameObject<CLaserTile>(this);
		((CLaserTile*)m_pTiles[locate.y][locate.x])->Initialize(m_SelectedDirection);
		break;
	case TILE_ID::TARGET:
		m_pTiles[locate.y][locate.x] = aqua::CreateGameObject<CTargetTile>(this);
		((CTargetTile*)m_pTiles[locate.y][locate.x])->Initialize(m_SelectedDirection, m_tile_menu[m_SelectedTileMenu].second);
		break;
	case TILE_ID::MIRROR:
		m_pTiles[locate.y][locate.x] = aqua::CreateGameObject<CMirrorTile>(this);
		((CMirrorTile*)m_pTiles[locate.y][locate.x])->Initialize((DIRECTION_ID)(((int)m_SelectedDirection % 2) * 2));
		break;
	case TILE_ID::WAY:
		m_pTiles[locate.y][locate.x] = aqua::CreateGameObject<CWayTile>(this);
		((CWayTile*)m_pTiles[locate.y][locate.x])->Initialize(m_SelectedDirection);
		break;
	case TILE_ID::CRYSTAL:
		m_pTiles[locate.y][locate.x] = aqua::CreateGameObject<CCrystalTile>(this);
		((CCrystalTile*)m_pTiles[locate.y][locate.x])->Initialize(m_tile_menu[m_SelectedTileMenu].second);
		break;
	}

	m_pTiles[locate.y][locate.x]->SetLocate(locate);
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

	// タイルデータの解放
	for (auto& y : m_pTiles)
		for (auto& x : y)
			if (x)
			{
				x->Finalize();
				m_ChildObjectList.erase(std::find(m_ChildObjectList.begin(), m_ChildObjectList.end(), x));
				AQUA_SAFE_DELETE(x);
			}

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
			((CTargetTile*)i.second)->Initialize(m_direction_table_to_id.at(split_buf[0]), m_color_table_to_id.at(split_buf[1]));
			break;
		case 'l':
			((CLaserTile*)i.second)->Initialize(m_direction_table_to_id.at(split_buf[0]));
			break;
		case 'm':
			((CMirrorTile*)i.second)->Initialize(m_direction_table_to_id.at(split_buf[0]));
			break;
		case 'w':
			((CWayTile*)i.second)->Initialize(m_direction_table_to_id.at(split_buf[0]));
			break;
		case 'c':
			((CCrystalTile*)i.second)->Initialize(m_color_table_to_id.at(split_buf[0]));
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

void CField::SaveMapData(const std::string& file_name) const
{
	// ファイルを上書き
	std::ofstream ofs(file_name);

	// フィールドの大きさを書き込む
	ofs << m_TileNumX << '\n';
	ofs << m_TileNumY << '\n';

	// タイルの配置を書き込む
	for (int y = 0; y < m_TileNumY; ++y)
	{
		for (int x = 0; x < m_TileNumX; ++x)
		{
			if (m_pTiles[y][x])
			{
				switch (m_pTiles[y][x]->GetTileID())
				{
				case TILE_ID::WALL:
					ofs << '#';
					break;
				case TILE_ID::GLASS:
					ofs << '-';
					break;
				case TILE_ID::LASER:
					ofs << 'l';
					break;
				case TILE_ID::TARGET:
					ofs << 't';
					break;
				case TILE_ID::MIRROR:
					ofs << 'm';
					break;
				case TILE_ID::WAY:
					ofs << 'w';
					break;
				case TILE_ID::CRYSTAL:
					ofs << 'c';
					break;
				}
			}
			else
			{
				ofs << '.';
			}
		}
		ofs << '\n';
	}

	// 特殊タイルの詳細を書き込む
	for (int y = 0; y < m_TileNumY; ++y)
		for (int x = 0; x < m_TileNumX; ++x)
			if (m_pTiles[y][x])
				switch (m_pTiles[y][x]->GetTileID())
				{
				case TILE_ID::LASER:
					ofs <<
						m_direction_table_to_name.at(((CLaserTile*)m_pTiles[y][x])->GetDirectionID()) << '\n';
					break;
				case TILE_ID::TARGET:
					ofs <<
						m_direction_table_to_name.at(((CTargetTile*)m_pTiles[y][x])->GetDirectionID()) << ',' <<
						m_color_table_to_name.at(((CTargetTile*)m_pTiles[y][x])->GetColorID()) << '\n';
					break;
				case TILE_ID::MIRROR:
					ofs <<
						m_direction_table_to_name.at(((CMirrorTile*)m_pTiles[y][x])->GetDirectionID()) << '\n';
					break;
				case TILE_ID::WAY:
					ofs <<
						m_direction_table_to_name.at(((CWayTile*)m_pTiles[y][x])->GetDirectionID()) << '\n';
					break;
				case TILE_ID::CRYSTAL:
					ofs <<
						m_color_table_to_name.at(((CCrystalTile*)m_pTiles[y][x])->GetColorID()) << '\n';
					break;
				}

	// ファイルを閉じる
	ofs.close();
}

void CField::Update_01_LoadDragFileMapData()
{
	while (DxLib::GetDragFileNum() > 0)
	{
		char buf[1024];
		DxLib::GetDragFilePath(buf);
		m_LoadFilePath = buf;
		LoadMapData(m_LoadFilePath.string());
	}
}

void CField::Update_02_ChangeTileMenu()
{
	int mouse_wheel = std::clamp(aqua::mouse::GetWheel(), -1, 1);
	if (!aqua::keyboard::Button(aqua::keyboard::KEY_ID::LSHIFT))
	{
		int next_selected_menu = m_SelectedTileMenu - mouse_wheel;
		next_selected_menu = (next_selected_menu + m_tile_menu_num) % m_tile_menu_num;
		if (next_selected_menu != m_SelectedTileMenu)
		{
			m_SelectedTileMenu = next_selected_menu;
			m_pSoundManager->Play(SOUND_ID::BUTTON_CURSOR_SE);
		}
	}
	else
	{
		DIRECTION_ID next_direction = (DIRECTION_ID)((int)m_SelectedDirection - mouse_wheel);
		next_direction = (DIRECTION_ID)(((int)next_direction + (int)DIRECTION_ID::MAX) % (int)DIRECTION_ID::MAX);
		if (next_direction != m_SelectedDirection)
		{
			m_SelectedDirection = next_direction;
			m_pSoundManager->Play(SOUND_ID::BUTTON_CURSOR_SE);
		}
	}

	m_TileMenuSprite.rect.left = (int)m_TileMenuSprite.GetTextureWidth() / m_tile_menu_num * m_SelectedTileMenu;
	m_TileMenuSprite.rect.right = m_TileMenuSprite.rect.left + (int)m_TileMenuSprite.GetTextureWidth() / m_tile_menu_num;
	switch (m_tile_menu[m_SelectedTileMenu].first)
	{
	case TILE_ID::WALL:
		m_TileMenuSprite.rotation = 0.0f;
		break;
	case TILE_ID::GLASS:
		m_TileMenuSprite.rotation = 0.0f;
		break;
	case TILE_ID::LASER:
		m_TileMenuSprite.rotation = (float)(aqua::PI / 2.0f) * (int)m_SelectedDirection;
		break;
	case TILE_ID::TARGET:
		m_TileMenuSprite.rotation = (float)(aqua::PI / 2.0f) * (int)m_SelectedDirection;
		break;
	case TILE_ID::MIRROR:
		m_TileMenuSprite.rotation = (float)(aqua::PI / 2.0f) * (((int)m_SelectedDirection % 2) == 1);
		break;
	case TILE_ID::WAY:
		m_TileMenuSprite.rotation = (float)(aqua::PI / 2.0f) * (int)m_SelectedDirection;
		break;
	case TILE_ID::CRYSTAL:
		m_TileMenuSprite.rotation = 0.0f;
		break;
	}
}

void CField::Update_03_ChangeTileNum()
{
	int next_w = max(m_TileNumX + aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::RIGHT) - aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::LEFT), 1);
	int next_h = max(m_TileNumY + aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::DOWN) - aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::UP), 1);

	if (next_w != m_TileNumX || next_h != m_TileNumY)
	{
		ResizeField(next_w, next_h);
		m_pSoundManager->Play(SOUND_ID::TILE_MOVE_SE);
	}
}

void CField::Update_04_TileDrawSettings()
{
	m_DispTileSize = min(((float)aqua::GetWindowWidth() - m_edge_space) / m_TileNumX, ((float)aqua::GetWindowHeight() - m_edge_space) / m_TileNumY);
	m_OutAreaTileSprite.scale.x = m_DispTileSize / (float)m_OutAreaTileSprite.GetTextureWidth();
	m_OutAreaTileSprite.scale.y = m_DispTileSize / (float)m_OutAreaTileSprite.GetTextureHeight();
}

void CField::Update_05_PlaceAndEraseTile()
{
	m_CursorLocate_prev = m_CursorLocate;
	m_CursorLocate.x = (int)floorf((float)(aqua::mouse::GetCursorPos().x - ((int)aqua::GetWindowWidth() - m_DispTileSize * m_TileNumX) / 2) / (float)m_DispTileSize);
	m_CursorLocate.y = (int)floorf((float)(aqua::mouse::GetCursorPos().y - ((int)aqua::GetWindowHeight() - m_DispTileSize * m_TileNumY) / 2) / (float)m_DispTileSize);

	if (m_CursorLocate.x >= 0 &&
		m_CursorLocate.y >= 0 &&
		m_CursorLocate.x < m_TileNumX &&
		m_CursorLocate.y < m_TileNumY)
	{
		if (aqua::mouse::Trigger(aqua::mouse::BUTTON_ID::LEFT) ||
			m_CursorLocate_prev != m_CursorLocate && aqua::mouse::Button(aqua::mouse::BUTTON_ID::LEFT))
		{
			ReplaceTile(m_CursorLocate);
			m_pSoundManager->Play(SOUND_ID::TILE_PICK_SE);
		}
		if (aqua::mouse::Trigger(aqua::mouse::BUTTON_ID::RIGHT) ||
			m_CursorLocate_prev != m_CursorLocate && aqua::mouse::Button(aqua::mouse::BUTTON_ID::RIGHT))
		{
			DeleteTile(m_CursorLocate);
			m_pSoundManager->Play(SOUND_ID::TILE_PUT_SE);
		}
	}
}

void CField::Update_06_SaveMapData()
{
	if (aqua::keyboard::Button(aqua::keyboard::KEY_ID::LCONTROL) && aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::S))
	{
		std::string tmp_str;
		char tmp_initial_dir[1024];
		char tmp_file[1024];

		tmp_str = m_LoadFilePath.parent_path().string() + "/";
		std::memcpy(tmp_initial_dir, tmp_str.c_str(), tmp_str.size());
		tmp_initial_dir[tmp_str.size()] = '\0';

		tmp_str = m_LoadFilePath.string();
		std::memcpy(tmp_file, tmp_str.c_str(), tmp_str.size());
		tmp_file[tmp_str.size()] = '\0';

		OPENFILENAME f_data = {};
		f_data.lStructSize = sizeof(OPENFILENAME);
		f_data.hwndOwner = aqua::GetWindowHandle();
		f_data.lpstrFilter = TEXT("TXTファイル(*.txt)\0*.txt\0\0");
		f_data.lpstrFile = tmp_file;
		f_data.nMaxFile = 1024;
		f_data.lpstrInitialDir = tmp_initial_dir;
		f_data.lpstrTitle = TEXT("マップデータの保存");
		f_data.Flags =
			OFN_OVERWRITEPROMPT |
			OFN_NOCHANGEDIR |
			OFN_FILEMUSTEXIST;
		f_data.lpstrDefExt = TEXT("*.txt");

		if (GetSaveFileName(&f_data) == IDOK)
		{
			SaveMapData(tmp_file);
			m_LoadFilePath = f_data.lpstrFile;
		}
	}
}

void CField::Update_07_Tiles()
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

void CField::Update_08_CalcLaser()
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

void CField::Update_09_CursorDrawSettings()
{
	m_CursorHorizontalBox.height = m_DispTileSize;
	m_CursorVerticalBox.width = m_DispTileSize;
	m_CursorHorizontalBox.position.y = (aqua::GetWindowHeight() - m_DispTileSize * m_TileNumY) / 2.0f + m_DispTileSize * m_CursorLocate.y;
	m_CursorVerticalBox.position.x = (aqua::GetWindowWidth() - m_DispTileSize * m_TileNumX) / 2.0f + m_DispTileSize * m_CursorLocate.x;
}

void CField::Update_10_Animations()
{
	m_LaserTrailSprite.Update();
}
