#pragma once

#include "System.h"
#include "VEC2.h"

/* ステータス */
enum class eSTATUS
{
	HP,			// 体力
	ATK,		// 攻撃力
	INTERVAL,	// 発射間隔
	BULLET,		// 弾数

	MAX
};

/* 弾 */
enum class eBULLET
{
	NORMAL,		// 通常弾
	CHARGE,		// チャージ
	LASER,		// レーザー
	MISSILE,	// ミサイル

	MAX
};

/* スキル */
enum class eSKILL
{
	DASH,			// ダッシュ
	SHIELD,			// シールド展開
	ELECTRIC_TRAP,	// 静電トラップ
	MINE_TRAP,		// 地雷トラップ
	MENDING,		// 修繕
	FULL_BURST,		// フルバースト
	QUICK_RELOAD,	// クイックリロード
	RANDOM,			// ランダム発動

	MAX
};

/* アビリティ */
enum class eABILITY
{
	RICOCHET,		// 跳弾
	BLESSING,		// 加護
	ADVERSITY,		// 逆境
	BERSERKER,		// バーサーカー
	REVENGER_ORB,	// リベンジャーオーブ
	DEATH,			// デス

	MAX
};

/* プレイヤー初期化情報 */
struct sPlayerInitData
{
	int hp;				// 体力
	int atk;			// 攻撃力
	int interval;		// 発射間隔
	int bulletnum;		// 弾数
	eBULLET bullet;		// 弾
	eSKILL skill;		// スキル
	eABILITY ability;	// アビリティ

	sPlayerInitData()
		: hp(0)
		, atk(0)
		, interval(0)
		, bulletnum(0)
		, bullet(eBULLET::NORMAL)
		, skill(eSKILL::MAX)
		, ability(eABILITY::MAX)
	{}

	sPlayerInitData(int hp, int atk, int interval, int bulletnum, eBULLET bullet, eSKILL skill, eABILITY ability)
		: hp(hp)
		, atk(atk)
		, interval(interval)
		, bulletnum(bulletnum)
		, bullet(bullet)
		, skill(skill)
		, ability(ability)
	{}
};

/* プレイヤークラス */
class cPlayer
{
	/* 列挙型 */
private:
	enum class eSTATE	// 状態
	{
		MOVE,	// 移動
		READY,	// 準備
		STOP,	// 硬直
		REPAIR,	// 復活
		DEAD,	// 死亡
		CHARGE,	// チャージ

		SKILL_DASH,				// スキル・ダッシュ
		SKILL_SHIELD,			// スキル・シールド展開
		SKILL_ELECTRIC_TRAP,	// スキル・静電トラップ
		SKILL_MINE_TRAP,		// スキル・地雷トラップ
		SKILL_MENDING,			// スキル・修繕
		SKILL_FULL_BURST,		// スキル・フルバースト
		SKILL_QUICK_RELOAD,		// スキル・クイックリロード
	};

	/* 公開関数 */
public:
	cPlayer(int PlayerID, sPlayerInitData InitData);	// コンストラクタ
	~cPlayer();											// デストラクタ
	void Update();										// 更新
	void Draw_Default() const;							// 描画・DEFAULT
	void Draw_FadeAdd() const;							// 描画・FADE_ADD

	bool CheckShot() const;			// ショット判定
	bool CheckRunSkill() const;		// スキルの発動判定
	bool IsRepair() const;			// 復活判定
	bool IsDead() const;			// 死亡判定
	bool IsShield() const;			// シールド展開判定
	void SetPos(VEC2F Pos);							// 座標を設定
	void AddMotion(VEC2F Motion);					// 移動量を加算
	void SetPalsyTime(int Time);					// 麻痺状態にする
	void TakenDamageProcess(int Owner, int Damage);	// ダメージを受けたときの処理
	void DealtDamageProcess(int Target);			// ダメージを与えたときの処理

	static void LoadResource();												// リソースの読み込み
	static void DeleteResource();											// リソースの解放
	static bool Comp(const cPlayer* const lhs, const cPlayer* const rhs);	// 描画順の比較関数

	/* 非公開関数 */
private:
	::DISALLOW_COPY_CONSTRUCTOR(cPlayer);
	::DISALLOW_MOVE_CONSTRUCTOR(cPlayer);

	void m_MoveProcess();				// 移動処理
	void m_RunSkill();					// スキル発動
	void m_SkillProcess(eSKILL skill);	// スキルの内容を実行

	/* 公開変数 */
public:
	static const int MaxCoreNum;	// 残基数
	static const float Radius;		// 衝突半径
	static const int MaxOrbNum;		// リベンジャーオーブの最大数

	const int PlayerID;			// プレイヤーID
	const int MaxHP;			// 最大体力
	const int AtkDamage;		// 攻撃力
	const int MaxShotCoolTime;	// 発射のクールタイム
	const int MaxBulletNum;		// 最大弾数
	const eBULLET Bullet;		// 弾
	const eSKILL Skill;			// スキル
	const eABILITY Ability;		// アビリティ
	const int MaxSkillCoolTime;	// スキルのクールタイム
	const int MaxMineTrapNum;	// 地雷トラップの最大設置数

	// 外部からの参照用
	const int& CoreNum;				// 残基数
	const VEC2F& Pos;				// 位置
	const float& Angle;				// 角度
	const int& HP;					// 体力
	const int& BulletNum;			// 残弾数
	const int& SkillCoolTime;		// スキルのクールタイム
	const int& OrbNum;				// リベンジャーオーブの数
	const float& OrbAngle;			// リベンジャーオーブの始点角度

	/* 非公開変数 */
private:
	static const float m_MoveSpeed;				// 元の移動速度
	static const float m_Friction;				// 摩擦係数
	static const float m_MuzzleLength;			// 銃口までの距離
	static const int m_MaxReadyTime;			// 準備時間
	static const int m_MaxStopTime;				// 硬直時間
	static const int m_AddStopTime_Laser;		// レーザー選択時の追加硬直時間
	static const int m_MaxReloadTime;			// 装填開始までの時間
	static const int m_MaxRepairTime;			// 復活時間
	static const int m_MaxChargeEffectTime;		// チャージ完了エフェクト最大時間
	static const int m_MaxSkillCoolTime[];		// スキルのクールタイム
	static const int m_MaxSkillNeedTime[];		// スキル発動の所要時間
	static const int m_MaxShieldTime;			// シールド展開時間
	static const int m_MaxElectricTrapTime;		// 電気トラップの設置時間
	static const int m_MaxDeathBulletTimes;		// デス弾発射までの最大回数
	static const int m_MaxMendingNum;			// 修繕の最大回数
	static const int m_MaxRandomTime;			// ランダムの発動最大時間
	static const float m_OrbRotation;			// リベンジャーオーブの回転速度
	static const int m_AdversityLine;			// 逆境発動の残りHP
	static const int m_AdversityValue;			// 逆境発動の増加攻撃力
	static const int m_MaxDamageTime;			// ダメージ演出の最大時間
	static const int m_MaxFrameTime;			// 1コマのフレーム数
	static const int m_DispOffset;				// 表示位置のズレ
	static const double m_DispScale;			// 表示する拡大率

	int m_CoreNum;					// 残基数
	eSTATE m_State;					// 状態
	VEC2F m_Pos;					// 位置
	VEC2F m_Motion;					// 移動量
	float m_Angle;					// 角度
	int m_StateTimer;				// 経過時間
	int m_HP;						// 体力
	int m_BulletNum;				// 残弾数
	int m_ShotCoolTime;				// 発射のクールタイム
	const int m_ReloadInterval;		// 装填間隔
	int m_ReloadTime;				// 装填時間
	int m_SkillCoolTime;			// スキルのクールタイム
	int m_SkillNeedTime;			// スキル発動の所要時間
	bool m_ShotFlag;				// 発射判定フラグ
	bool m_RunSkillFlag;			// スキル発動判定フラグ
	bool m_AimFlag;					// 狙い撃ちフラグ
	const int m_ChargeTime_First;	// チャージ１回目までの時間
	const int m_ChargeTime_Second;	// チャージ２回目までの時間
	int m_ChargeTime;				// チャージ時間
	int m_ChargeEffectTime;			// チャージ完了エフェクト
	int m_PalsyTime;				// 麻痺時間
	int m_ShieldTime;				// シールド展開効果時間
	int m_FullburstValue;			// フルバーストで用いる値
	int m_BerserkerValue;			// バーサーカーで用いる値
	int m_ElectricTrapTime;			// 電気トラップの設置時間
	int m_MendingNum;				// 修繕回数
	int m_RandomTime;				// ランダムの発動時間
	int m_RandomInterval;			// ランダムの発動間隔
	eSKILL m_NowRandomSkill;		// ランダムの発動スキル
	int m_RunRandomSkillTimer;		// ランダムスキル発動時間
	int m_RicochetTimes;			// 跳弾回数
	int m_DeathBulletTimes;			// デス弾発射までの回数
	int m_OrbNum;					// リベンジャーオーブの数
	float m_OrbAngle;				// リベンジャーオーブの始点角度
	int m_DamageTimer;				// ダメージタイマー
	int m_AnimationTimer;			// アニメーションタイマー

	static bool m_LoadedFlag;				// リソースの読み込みフラグ
	static int m_Image_ChargeRing;			// 画像ハンドル・チャージリング
	static int m_Image_SkillTimeRing;		// 画像ハンドル・スキルリング
	static int m_Image_Player[3 * 8];		// 画像ハンドル・プレイヤー
	static int m_Image_PlayerColor[3 * 8];	// 画像ハンドル・プレイヤーの色部分
};
