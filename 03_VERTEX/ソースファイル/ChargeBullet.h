#pragma once

#include "BaseAtkobj.h"
class cPlayer;

class cChargeBullet : public cBaseAtkobj
{
	/* öJÖ */
public:
	cChargeBullet(cPlayer* Owner, int AtkDamage, VEC2F Pos, float Angle, int Ricochet, int ChargePower);	// RXgN^
	~cChargeBullet() override;																				// fXgN^
	void Update() override;																					// XV
	void Draw_Default() const override;																		// `æEDEFAULT
	void Draw_FadeAdd() const override;																		// `æEADD

	bool Active() const override;	// ANeBuóÔ

	/* ñöJÏ */
private:
	static const float m_Radius_0;		// ¼aE­ß0
	static const float m_Radius_1;		// ¼aE­ß1
	static const float m_Radius_2;		// ¼aE­ß2
	static const float m_Speed_Start_0;	// ¬E­ß0
	static const float m_Speed_Start_1;	// ¬E­ß1
	static const float m_Speed_Start_2;	// ¬E­ß2
	static const float m_Speed_End;		// Åá¬x

	bool m_HitFlag;				// ÕËÏ»è
	const int m_AtkDamage;		// UÍ
	VEC2F m_Pos;				// Êu
	VEC2F m_Pos_prev;			// Êu(1t[O)
	const float m_Radius;		// ¼a
	float m_Speed;				// Ú®¬x
	float m_Angle;				// px
	int m_Ricochet;				// µe
	const int m_ChargePower;	// `[Wl
};
