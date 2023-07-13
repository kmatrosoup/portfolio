//////////////////////
//   プリプロセス   //
//////////////////////
#include "DxLib.h"
#include "System.h"
#include "Input.h"
#include "InputMgr.h"
#include "Game.h"

//////////////////////////
//   メインプログラム   //
//////////////////////////
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	/* 開始の処理 */
	if (SetOutApplicationLogValidFlag(FALSE) != 0 ||	// ログ出力
		ChangeWindowMode(TRUE) != 0 ||					// ウィンドウモード
		SetGraphMode(Sw, Sh, 32) != 0 ||				// ウィンドウサイズ
		SetWindowTextA("無双の書") != 0 ||				// ウィンドウテキスト
		SetAlwaysRunFlag(TRUE) != 0 ||					// 常時処理設定
		DxLib_Init() != 0 ||							// DxLibの初期化
		SetDrawScreen(DX_SCREEN_BACK) != 0)				// 描画先スクリーン指定
		return -1;

	/* 初期化 */
	cGame::CreateInstance();

	//////////////////////
	//   ゲームループ   //
	//////////////////////
	while (!Input::Keyboard::In(KEY_INPUT_ESCAPE) && ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		/* 処理 */
		InputMgr::Update();
		cGame::GetInstance().Update();

		/* 描画 */
		cGame::GetInstance().Draw();
	}

	/* 解放 */
	cGame::DestroyInstance();

	/* 終了の処理 */
	DxLib_End();
	return 0;
}
