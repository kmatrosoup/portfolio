#pragma once

#include "../effect.h"

class CLineEffect : public IEffect
{
public:
	CLineEffect(aqua::IGameObject* parent);
	~CLineEffect() override = default;

	void Initialize(float pos_y);
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	static const float m_max_wait_time;			// 最大待機時間
	static const float m_min_active_time;		// 最低生存時間
	static const float m_max_active_time;		// 最大生存時間
	static const float m_min_base_thickness;	// 最低元高さ
	static const float m_max_base_thickness;	// 最大元高さ

	aqua::CTimer m_WaitTimer;	// 待機時間
	aqua::CTimer m_ActiveTimer;	// 生存時間
	float m_PositionY;			// Y座標
	float m_BaseThickness;		// 元高さ

	aqua::CLinePrimitive m_LinePrimitive;	// 横線の図形
};
