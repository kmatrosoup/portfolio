#pragma once

enum class SOUND_ID
{
	BGM,				// BGM
	BUTTON_CURSOR_SE,	// カーソル移動
	BUTTON_CLICK_SE,	// ボタン決定
	BUTTON_ERROR_SE,	// ボタンエラー
	LIGHTUP_SE,			// タイトルシーンのライトアップ
	TILE_PICK_SE,		// タイルの持ち上げ
	TILE_MOVE_SE,		// タイルの移動
	TILE_PUT_SE,		// タイルの確定
	SUCCESS_SE,			// 成功
	TYPING_SE_1,		// ゲームクリア時のタイプ音１
	TYPING_SE_2,		// ゲームクリア時のタイプ音２
	TYPING_SE_3,		// ゲームクリア時のタイプ音３

	MAX
};
