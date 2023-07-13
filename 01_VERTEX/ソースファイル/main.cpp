//////////////////////
//   プリプロセス   //
//////////////////////
#include "DxLib.h"
#include "System.h"
#include "Input.h"
#include "SceneMgr.h"

//////////////////////////
//   メインプログラム   //
//////////////////////////
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	/* 開始の処理 */
	if (SetOutApplicationLogValidFlag(FALSE) != 0 ||		// ログ出力
#if FULL_SCREEN_MODE
		ChangeWindowMode(FALSE) != 0 ||						// ウィンドウモード
#else
		ChangeWindowMode(TRUE) != 0 ||						// ウィンドウモード
#endif
		SetGraphMode(Sw * SScale, Sh * SScale, 32) != 0 ||	// ウィンドウサイズ
		SetWindowTextA("VERTEX") != 0 ||					// ウィンドウテキスト
		SetAlwaysRunFlag(TRUE) != 0 ||						// 常時処理
		DxLib_Init() != 0 ||								// DxLibの初期化
		SetMouseDispFlag(FALSE) != 0 ||						// マウスの表示設定
		SetDrawScreen(DX_SCREEN_BACK) != 0)					// 描画先スクリーン指定
		return -1;

	/* 初期化 */
	Input::Init();
	cSceneMgr::CreateInstance();

	////////////////
	//   ループ   //
	////////////////
	while (!Input::Keyboard::In(KEY_INPUT_ESCAPE) && !cSceneMgr::GetInstance().CheckExitGame() && ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		/* 処理 */
		Input::Update();
		cSceneMgr::GetInstance().Update();

		/* 描画 */
		cSceneMgr::GetInstance().Draw();
	}

	/* 終了 */
	cSceneMgr::DestroyInstance();

	DxLib_End();
	return 0;
}
