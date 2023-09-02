#include "field.h"
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

const float CField::m_laser_trail_time = 0.6f;
const int CField::m_laser_trail_frame_num = 12;
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
	, m_pGlowScreen(nullptr)
	, m_FieldWidth(0)
	, m_FieldHeight(0)
	, m_TileSize(0.0f)
	, m_SelectedTileMenu(0)
	, m_SelectedDirection(DIRECTION_ID::RIGHT)
{
}

void CField::Initialize()
{
	// データの解放
	Finalize();

	// ゲームオブジェクトの取得
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

	m_TileMenuSprite.Create("data\\tile_menu.png");
	m_TileMenuSprite.anchor.x = m_TileMenuSprite.GetTextureWidth() / 2.0f / m_tile_menu_num;
	m_TileMenuSprite.anchor.y = m_TileMenuSprite.GetTextureHeight() / 2.0f;
	m_TileMenuSprite.scale.x = 4.0f;
	m_TileMenuSprite.scale.y = 4.0f;
	m_TileMenuSprite.position = m_TileMenuSprite.anchor * m_TileMenuSprite.scale - m_TileMenuSprite.anchor;

	// 初期化処理
	m_CursorHorizontalBox.Setup(aqua::CVector2::ZERO, (float)aqua::GetWindowWidth(), 0.0f, 0xff202020, true, 0.0f, aqua::ALPHABLEND::ADD);
	m_CursorVerticalBox.Setup(aqua::CVector2::ZERO, 0.0f, (float)aqua::GetWindowHeight(), 0xff202020, true, 0.0f, aqua::ALPHABLEND::ADD);
	m_FieldWidth = 0;
	m_FieldHeight = 0;
	m_LaserTrailTimer.Setup(m_laser_trail_time);
	ResizeField(8, 6);
}

void CField::Update()
{
	// 外部タイルデータの読み込み
	while (DxLib::GetDragFileNum() > 0)
	{
		char buf[1024];
		DxLib::GetDragFilePath(buf);
		LoadMapData(buf);
	}

	// フィールドサイズの変更
	{
		int next_w = max(m_FieldWidth + aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::RIGHT) - aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::LEFT), 1);
		int next_h = max(m_FieldHeight + aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::DOWN) - aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::UP), 1);

		if (next_w != m_FieldWidth || next_h != m_FieldHeight)
		{
			ResizeField(next_w, next_h);
			m_pSoundManager->Play(SOUND_ID::TILE_MOVE_SE);
		}
	}

	// 設置タイルの更新
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

	// タイルのサイズ設定
	m_TileSize = min(((float)aqua::GetWindowWidth() - m_edge_space) / m_FieldWidth, ((float)aqua::GetWindowHeight() - m_edge_space) / m_FieldHeight);
	m_OutAreaTileSprite.scale.x = m_TileSize / (float)m_OutAreaTileSprite.GetTextureWidth();
	m_OutAreaTileSprite.scale.y = m_TileSize / (float)m_OutAreaTileSprite.GetTextureHeight();

	// タイルの設置処理
	m_CursorLocate_prev = m_CursorLocate;
	m_CursorLocate.x = (int)floorf((float)(aqua::mouse::GetCursorPos().x - ((int)aqua::GetWindowWidth() - m_TileSize * m_FieldWidth) / 2) / (float)m_TileSize);
	m_CursorLocate.y = (int)floorf((float)(aqua::mouse::GetCursorPos().y - ((int)aqua::GetWindowHeight() - m_TileSize * m_FieldHeight) / 2) / (float)m_TileSize);

	if (m_CursorLocate.x >= 0 &&
		m_CursorLocate.y >= 0 &&
		m_CursorLocate.x < m_FieldWidth &&
		m_CursorLocate.y < m_FieldHeight)
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

	// タイルデータの保存
	if (aqua::keyboard::Button(aqua::keyboard::KEY_ID::LCONTROL) && aqua::keyboard::Trigger(aqua::keyboard::KEY_ID::S))
	{
		char tmp_initial_dir[256] = "./";
		char tmp_file[256] = "";

		OPENFILENAME f_data = {};
		f_data.lStructSize = sizeof(OPENFILENAME);
		f_data.hwndOwner = aqua::GetWindowHandle();
		f_data.nMaxFile = 256;
		f_data.lpstrInitialDir = tmp_initial_dir;
		f_data.lpstrFile = tmp_file;
		f_data.lpstrDefExt = TEXT("*.txt");
		f_data.lpstrFilter = TEXT("TXTファイル(*.txt)\0*.txt\0");;
		f_data.lpstrTitle = TEXT("マップデータの保存");;
		f_data.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&f_data) == IDOK)
			SaveMapData(tmp_file);
	}

	// タイルの更新
	for (int y = 0; y < m_FieldHeight; ++y)
		for (int x = 0; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
			{
				m_pTileLocate[y][x]->SetDispSize(m_TileSize);
				m_pTileLocate[y][x]->SetPosition(aqua::CVector2((float)aqua::GetWindowWidth() - m_TileSize * m_FieldWidth, (float)aqua::GetWindowHeight() - m_TileSize * m_FieldHeight) * 0.5f + m_TileSize * aqua::CVector2((float)x, (float)y));
				m_pTileLocate[y][x]->Update();
			}

	// 全てのターゲットの照射フラグを下げる
	for (int y = 0; y < m_FieldHeight; ++y)
		for (int x = 0; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
				if (m_pTileLocate[y][x]->GetTileID() == TILE_ID::TARGET)
					((CTargetTile*)m_pTileLocate[y][x])->SetLitFlag(false);

	// レーザーの軌道処理、ターゲットへの照射処理
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
	m_LaserChain.clear();
	for (int y = 0; y < m_FieldHeight; ++y)
		for (int x = 0; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
				if (m_pTileLocate[y][x]->GetTileID() == TILE_ID::LASER)
					for (const auto& i : ((CLaserTile*)m_pTileLocate[y][x])->GetGeneratedLaser())
						f_set_chain_laser(f_set_chain_laser, { aqua::CPoint(x, y), i });

	// タイマー処理
	m_LaserTrailTimer.Update();
	if (m_LaserTrailTimer.Finished())
		m_LaserTrailTimer.Reset();

	// カーソル更新
	m_CursorHorizontalBox.height = m_TileSize;
	m_CursorVerticalBox.width = m_TileSize;
	m_CursorHorizontalBox.position.y = (aqua::GetWindowHeight() - m_TileSize * m_FieldHeight) / 2.0f + m_TileSize * m_CursorLocate.y;
	m_CursorVerticalBox.position.x = (aqua::GetWindowWidth() - m_TileSize * m_FieldWidth) / 2.0f + m_TileSize * m_CursorLocate.x;

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

void CField::Finalize()
{
	m_LaserTrailSprite.Delete();
	m_OutAreaTileSprite.Delete();
	m_GlowMaskSurface.Delete();
	m_GlowMaskSprite.Delete();
	m_TileMenuSprite.Delete();

	for (auto& y : m_pTileLocate)
		for (auto& x : y)
			if (x)
			{
				x->Finalize();
				AQUA_SAFE_DELETE(x);
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

float CField::GetTileSize() const
{
	return m_TileSize;
}

void CField::ResizeField(int field_width, int field_height)
{
	// 範囲外のタイルを削除
	for (int y = field_height; y < m_FieldHeight; ++y)
		for (int x = field_width; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
			{
				m_pTileLocate[y][x]->Finalize();
				m_ChildObjectList.erase(std::find(m_ChildObjectList.begin(), m_ChildObjectList.end(), m_pTileLocate[y][x]));
				AQUA_SAFE_DELETE(m_pTileLocate[y][x]);
			}

	// 変更後のフィールドを生成
	std::vector<std::vector<ITile*>> new_tile_locate(field_height, std::vector<ITile*>(field_width, nullptr));
	for (int y = 0; y < field_height; ++y)
		for (int x = 0; x < field_width; ++x)
			if (x < m_FieldWidth && y < m_FieldHeight)
				new_tile_locate[y][x] = m_pTileLocate[y][x];

	// 変更後のフィールドを適用
	m_FieldWidth = field_width;
	m_FieldHeight = field_height;
	m_pTileLocate = new_tile_locate;
}

void CField::DeleteTile(const aqua::CPoint& locate)
{
	if (locate.x < 0 || locate.y < 0 || locate.x >= m_FieldWidth || locate.y >= m_FieldHeight)
		return;

	if (m_pTileLocate[locate.y][locate.x])
	{
		m_pTileLocate[locate.y][locate.x]->Finalize();
		m_ChildObjectList.erase(std::find(m_ChildObjectList.begin(), m_ChildObjectList.end(), m_pTileLocate[locate.y][locate.x]));
		AQUA_SAFE_DELETE(m_pTileLocate[locate.y][locate.x]);
	}
}

void CField::ReplaceTile(const aqua::CPoint& locate)
{
	if (locate.x < 0 || locate.y < 0 || locate.x >= m_FieldWidth || locate.y >= m_FieldHeight)
		return;

	DeleteTile(locate);

	switch (m_tile_menu[m_SelectedTileMenu].first)
	{
	case TILE_ID::WALL:
		m_pTileLocate[locate.y][locate.x] = aqua::CreateGameObject<CWallTile>(this);
		((CWallTile*)m_pTileLocate[locate.y][locate.x])->Initialize();
		break;
	case TILE_ID::GLASS:
		m_pTileLocate[locate.y][locate.x] = aqua::CreateGameObject<CGlassTile>(this);
		((CGlassTile*)m_pTileLocate[locate.y][locate.x])->Initialize();
		break;
	case TILE_ID::LASER:
		m_pTileLocate[locate.y][locate.x] = aqua::CreateGameObject<CLaserTile>(this);
		((CLaserTile*)m_pTileLocate[locate.y][locate.x])->Initialize(m_SelectedDirection);
		break;
	case TILE_ID::TARGET:
		m_pTileLocate[locate.y][locate.x] = aqua::CreateGameObject<CTargetTile>(this);
		((CTargetTile*)m_pTileLocate[locate.y][locate.x])->Initialize(m_SelectedDirection, m_tile_menu[m_SelectedTileMenu].second);
		break;
	case TILE_ID::MIRROR:
		m_pTileLocate[locate.y][locate.x] = aqua::CreateGameObject<CMirrorTile>(this);
		((CMirrorTile*)m_pTileLocate[locate.y][locate.x])->Initialize((DIRECTION_ID)(((int)m_SelectedDirection % 2) * 2));
		break;
	case TILE_ID::WAY:
		m_pTileLocate[locate.y][locate.x] = aqua::CreateGameObject<CWayTile>(this);
		((CWayTile*)m_pTileLocate[locate.y][locate.x])->Initialize(m_SelectedDirection);
		break;
	case TILE_ID::CRYSTAL:
		m_pTileLocate[locate.y][locate.x] = aqua::CreateGameObject<CCrystalTile>(this);
		((CCrystalTile*)m_pTileLocate[locate.y][locate.x])->Initialize(m_tile_menu[m_SelectedTileMenu].second);
		break;
	}

	m_pTileLocate[locate.y][locate.x]->SetLocate(locate);
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
	for (auto& y : m_pTileLocate)
		for (auto& x : y)
			if (x)
			{
				x->Finalize();
				m_ChildObjectList.erase(std::find(m_ChildObjectList.begin(), m_ChildObjectList.end(), x));
				AQUA_SAFE_DELETE(x);
			}

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
	for (int y = 0; y < m_FieldHeight; ++y)
		for (int x = 0; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
				m_pTileLocate[y][x]->SetLocate(aqua::CPoint(x, y));

	// ファイルを閉じる
	ifs.close();
}

void CField::SaveMapData(const std::string& file_name) const
{
	// ファイルを上書き
	std::ofstream ofs(file_name);

	// フィールドの大きさを書き込む
	ofs << m_FieldWidth << '\n';
	ofs << m_FieldHeight << '\n';

	// タイルの配置を書き込む
	for (int y = 0; y < m_FieldHeight; ++y)
	{
		for (int x = 0; x < m_FieldWidth; ++x)
		{
			if (m_pTileLocate[y][x])
			{
				switch (m_pTileLocate[y][x]->GetTileID())
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
	for (int y = 0; y < m_FieldHeight; ++y)
		for (int x = 0; x < m_FieldWidth; ++x)
			if (m_pTileLocate[y][x])
				switch (m_pTileLocate[y][x]->GetTileID())
				{
				case TILE_ID::LASER:
					ofs <<
						m_direction_table_to_name.at(((CLaserTile*)m_pTileLocate[y][x])->GetDirectionID()) << '\n';
					break;
				case TILE_ID::TARGET:
					ofs <<
						m_direction_table_to_name.at(((CTargetTile*)m_pTileLocate[y][x])->GetDirectionID()) << ',' <<
						m_color_table_to_name.at(((CTargetTile*)m_pTileLocate[y][x])->GetColorID()) << '\n';
					break;
				case TILE_ID::MIRROR:
					ofs <<
						m_direction_table_to_name.at(((CMirrorTile*)m_pTileLocate[y][x])->GetDirectionID()) << '\n';
					break;
				case TILE_ID::WAY:
					ofs <<
						m_direction_table_to_name.at(((CWayTile*)m_pTileLocate[y][x])->GetDirectionID()) << '\n';
					break;
				case TILE_ID::CRYSTAL:
					ofs <<
						m_color_table_to_name.at(((CCrystalTile*)m_pTileLocate[y][x])->GetColorID()) << '\n';
					break;
				}

	// ファイルを閉じる
	ofs.close();
}
