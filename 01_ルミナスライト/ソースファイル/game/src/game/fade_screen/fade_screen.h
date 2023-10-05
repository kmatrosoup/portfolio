#pragma once

#include "aqua.h"

class CFadeScreen : public aqua::IGameObject
{
public:
	CFadeScreen(aqua::IGameObject* parent);
	~CFadeScreen() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Draw_Lit();
	void Finalize() override;

	void SetFade(bool flag) { m_IsFade = flag; }					// フェード状態の設定
	bool IsFade() const { return m_IsFade; }						// フェード中判定
	bool IsFinish() const { return m_IsFade && m_Param == 1.0f; }	// フェード演出の終了判定

private:
	static const float m_fade_time;	// フェード時間

	bool m_IsFade;	// フェード状態
	float m_Param;	// フェードの適用率（0.0f～1.0f）

	aqua::CSprite m_BaseSprite;		// 元画像
	aqua::CSurface m_DestSurface;	// 結果スクリーン
	aqua::CSprite m_DestSprite;		// 結果画像
};
