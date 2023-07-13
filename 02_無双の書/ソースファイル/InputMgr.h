#pragma once

enum class eINPUT
{
	DECIDE,	// Œˆ’è
	UP,		// ã“ü—Í
	DOWN,	// ‰º“ü—Í
	LEFT,	// ¶“ü—Í
	RIGHT,	// ‰E“ü—Í
};

namespace InputMgr
{
	extern void Update();	// XV

	extern bool In(eINPUT input);	// ‰Ÿ‚³‚ê‚½uŠÔ
	extern bool Keep(eINPUT input);	// ‰Ÿ‚³‚ê‚Ä‚¢‚éŠÔ
	extern bool Out(eINPUT input);	// —£‚³‚ê‚½uŠÔ
}
