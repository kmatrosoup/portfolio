// プリプロセス
#include "Score.h"
#include "DxLib.h"

// 静的メンバ変数の宣言
cScore* cScore::m_pInstance(nullptr);

// インスタンスを生成
void cScore::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = new cScore;
}

// インスタンスを解放
void cScore::DestroyInstance()
{
	if (!m_pInstance)
		return;

	delete m_pInstance;
	m_pInstance = nullptr;
}

// インスタンスを取得
cScore& cScore::GetInstance()
{
	return *m_pInstance;
}

// コンストラクタ
cScore::cScore()
	: m_Score(0)
{
}

// デストラクタ
cScore::~cScore()
{
}

// 初期化
void cScore::Init()
{
	m_Score = 0;
}

// スコアを取得
int cScore::GetScore() const
{
	return m_Score;
}

// スコアを加算
void cScore::AddScore(int score)
{
	m_Score += score;
}
