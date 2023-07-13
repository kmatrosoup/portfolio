#pragma once

#include "System.h"

/* 音声データ */
enum class eSOUND
{
	CURSOR,				// カーソル移動
	DECIDE,				// 決定
	CANCEL,				// キャンセル
	FAIL,				// エラー音
	READY,				// 準備完了
	GAMESTART,			// ゲームスタート
	COWNTDOWN,			// カウントダウン

	DAMAGE,				// 被弾
	RUN_SKILL,			// スキル発動
	CHARGE,				// チャージ
	RICOCHET,			// 跳弾
	BLESSING,			// 加護
	REFLECT,			// シールド

	BULLET_NORMAL,		// 発射・通常弾
	BULLET_CHARGE_0,	// 発射・チャージショット(0段階)
	BULLET_CHARGE_1,	// 発射・チャージショット(1段階)
	BULLET_CHARGE_2,	// 発射・チャージショット(2段階)
	BULLET_LASER,		// 発射・レーザー
	BULLET_MISSILE,		// 発射・ミサイル

	BLAST,				// 爆風
	DEATH,				// デスの被弾
	DESTROY,			// 破壊

	SKILL_DASH,				// スキル発動・ダッシュ
	SKILL_SHIELD,			// スキル発動・シールド
	SKILL_MENDING,			// スキル発動・修繕
	SKILL_FULL_BURST,		// スキル発動・フルバースト
	SKILL_QUICK_RELOAD_1,	// スキル発動・クイックリロード
	SKILL_QUICK_RELOAD_2,	// スキル発動・クイックリロード

	MAX
};

/* 音声データ管理クラス */
class cSoundData
{
	/* 公開関数 */
public:
	static void CreateInstance();		// インスタンスの生成
	static void DestroyInstance();		// インスタンスの解放
	static cSoundData& GetInstance();	// インスタンスの取得

	void Play(eSOUND Sound);	// 再生

	/* 非公開関数 */
private:
	::DISALLOW_COPY_CONSTRUCTOR(cSoundData);
	::DISALLOW_MOVE_CONSTRUCTOR(cSoundData);

	cSoundData();	// コンストラクタ
	~cSoundData();	// デストラクタ

	/* 非公開変数 */
private:
	static cSoundData* m_pInstance;	// インスタンスのポインタ

	int m_SoundHandle[(int)eSOUND::MAX];	// 音声ハンドル
};
