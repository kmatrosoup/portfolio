// プリプロセス
#include "Easing.h"
#include <cmath>

#define PI (3.1415926535897932384626433832795f)

// Sin-In
float Easing::Sin::In(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return 1.0f - cosf(param * PI / 2.0f);
}
// Sin-Out
float Easing::Sin::Out(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return sinf(param * PI / 2.0f);
}
// Sin-InOut
float Easing::Sin::InOut(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return (1.0f - cosf(param * PI)) / 2.0f;
}

// Quad-In
float Easing::Quad::In(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return param * param;
}
// Quad-Out
float Easing::Quad::Out(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return 1.0f - (1.0f - param) * (1.0f - param);
}
// Quad-InOut
float Easing::Quad::InOut(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return (param < 0.5f) ?
		Easing::Quad::In(param * 2.0f) / 2.0f :
		Easing::Quad::Out((param - 0.5f) * 2.0f) / 2.0f + 0.5f;
}

// Cubic-In
float Easing::Cubic::In(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return param * param * param;
}
// Cubic-Out
float Easing::Cubic::Out(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return 1.0f - (1.0f - param) * (1.0f - param) * (1.0f - param);
}
// Cubic-InOut
float Easing::Cubic::InOut(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return (param < 0.5f) ?
		Easing::Cubic::In(param * 2.0f) / 2.0f :
		Easing::Cubic::Out((param - 0.5f) * 2.0f) / 2.0f + 0.5f;
}

// Quart-In
float Easing::Quart::In(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return param * param * param * param;
}
// Quart-Out
float Easing::Quart::Out(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return 1.0f - (1.0f - param) * (1.0f - param) * (1.0f - param) * (1.0f - param);
}
// Quart-InOut
float Easing::Quart::InOut(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return (param < 0.5f) ?
		Easing::Quart::In(param * 2.0f) / 2.0f :
		Easing::Quart::Out((param - 0.5f) * 2.0f) / 2.0f + 0.5f;
}

// Quint-In
float Easing::Quint::In(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return param * param * param * param * param;
}
// Quint-Out
float Easing::Quint::Out(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return 1.0f - (1.0f - param) * (1.0f - param) * (1.0f - param) * (1.0f - param) * (1.0f - param);
}
// Quint-InOut
float Easing::Quint::InOut(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return (param < 0.5f) ?
		Easing::Quint::In(param * 2.0f) / 2.0f :
		Easing::Quint::Out((param - 0.5f) * 2.0f) / 2.0f + 0.5f;
}

// Elastic-In
float Easing::Elastic::In(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return -powf(2.0f, 10.0f * param - 10.0f) * sinf((param * 10.0f - 10.75f) * (2.0f * PI / 3.0f));
}
// Elastic-Out
float Easing::Elastic::Out(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return powf(2.0f, -10.0f * param) * sinf((param * 10.0f - 0.75f) * (2.0f * PI / 3.0f)) + 1;
}
// Elastic-InOut
float Easing::Elastic::InOut(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return (param < 0.5f) ?
		Easing::Elastic::In(param * 2.0f) / 2.0f :
		Easing::Elastic::Out((param - 0.5f) * 2.0f) / 2.0f + 0.5f;
}

// Bounce-In
float Easing::Bounce::In(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return 1.0f - Easing::Bounce::Out(1.0f - param);
}
// Bounce-Out
float Easing::Bounce::Out(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (param < 1.0f / d1)		return n1 * param * param;
	else if (param < 2.0f / d1)	return n1 * (param -= 1.5f / d1) * param + 0.75f;
	else if (param < 2.5f / d1)	return n1 * (param -= 2.25f / d1) * param + 0.9375f;
	else						return n1 * (param -= 2.625f / d1) * param + 0.984375f;
}
// Bounce-InOut
float Easing::Bounce::InOut(float param)
{
	if (param < 0.0f)	return 0.0f;
	if (param > 1.0f)	return 1.0f;

	return (param < 0.5f) ?
		Easing::Bounce::In(param * 2.0f) / 2.0f :
		Easing::Bounce::Out((param - 0.5f) * 2.0f) / 2.0f + 0.5f;
}
