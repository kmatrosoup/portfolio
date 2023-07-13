#pragma once

class cScore
{
	// 公開関数
public:
	static void CreateInstance();	// インスタンスを生成
	static void DestroyInstance();	// インスタンスを解放
	static cScore& GetInstance();	// インスタンスを取得

	void Init();	// 初期化

	int GetScore() const;		// スコアを取得
	void AddScore(int score);	// スコアを加算

	// 非公開関数
private:
	cScore(const cScore&) = delete;
	cScore& operator=(const cScore&) = delete;
	cScore(cScore&&) = delete;
	cScore& operator=(cScore&&) = delete;

	cScore();		// コンストラクタ
	~cScore();		// デストラクタ

	// 非公開変数
private:
	static cScore* m_pInstance;

	int m_Score;
};
