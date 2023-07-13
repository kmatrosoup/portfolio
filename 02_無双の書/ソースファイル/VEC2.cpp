//////////////////////
//   ƒvƒŠƒvƒƒZƒX   //
//////////////////////
#include "VEC2.h"
#include <cmath>

//////////////////////////
//   <VEC2>ƒƒ“ƒoŠÖ”   //
//////////////////////////

/* ƒRƒ“ƒXƒgƒ‰ƒNƒ^ */
VEC2::VEC2() : x(0), y(0) {}
VEC2::VEC2(const VEC2& obj) : x(obj.x), y(obj.y) {}
VEC2::VEC2(const VEC2F& obj) : x((int)floorf(obj.x)), y((int)floorf(obj.y)) {}
VEC2::VEC2(int x, int y) : x(x), y(y) {}

/* ‰‰Zq */
VEC2 VEC2::operator+() const { return (*this); }										// Zp‰‰ZqE’P€‰‰Zq<+>
VEC2 VEC2::operator-() const { return (*this * -1); }									// Zp‰‰ZqE’P€‰‰Zq<->
VEC2 VEC2::operator+(const VEC2& obj) const { return VEC2(x + obj.x, y + obj.y); }		// Zp‰‰ZqE‰ÁZ
VEC2F VEC2::operator+(const VEC2F& obj) const { return VEC2F(x + obj.x, y + obj.y); }	// Zp‰‰ZqE‰ÁZ(VEC2FŒvZ)
VEC2 VEC2::operator-(const VEC2& obj) const { return VEC2(x - obj.x, y - obj.y); }		// Zp‰‰ZqEŒ¸Z
VEC2F VEC2::operator-(const VEC2F& obj) const { return VEC2F(x - obj.x, y - obj.y); }	// Zp‰‰ZqEŒ¸Z(VEC2FŒvZ)
VEC2 VEC2::operator*(const int value) const { return VEC2(x * value, y * value); }		// Zp‰‰ZqEæZ
VEC2F VEC2::operator*(const float value) const { return VEC2F(x * value, y * value); }	// Zp‰‰ZqEæZ(VEC2FŒvZ)
VEC2 VEC2::operator/(const int value) const { return VEC2(x / value, y / value); }		// Zp‰‰ZqEœZ
VEC2F VEC2::operator/(const float value) const { return VEC2F(x / value, y / value); }	// Zp‰‰ZqEœZ(VEC2FŒvZ)
VEC2& VEC2::operator+=(const VEC2& obj) { x += obj.x;	y += obj.y;	return *this; }	// ‘ã“üZp‰‰ZqE‰ÁZ
VEC2& VEC2::operator-=(const VEC2& obj) { x -= obj.x;	y -= obj.y;	return *this; }	// ‘ã“üZp‰‰ZqEŒ¸Z
VEC2& VEC2::operator*=(const int value) { x *= value;	y *= value;	return *this; }	// ‘ã“üZp‰‰ZqEæZ
VEC2& VEC2::operator/=(const int value) { x /= value;	y /= value;	return *this; }	// ‘ã“üZp‰‰ZqEœZ

///////////////////////////
//   <VEC2F>ƒƒ“ƒoŠÖ”   //
///////////////////////////

/* ƒRƒ“ƒXƒgƒ‰ƒNƒ^ */
VEC2F::VEC2F() : x(0.0f), y(0.0f) {}
VEC2F::VEC2F(const VEC2F& obj) : x(obj.x), y(obj.y) {}
VEC2F::VEC2F(const VEC2& obj) : x((float)obj.x), y((float)obj.y) {}
VEC2F::VEC2F(float x, float y) : x(x), y(y) {}

/* ‰‰Zq */
VEC2F VEC2F::operator+() const { return (*this); }										// Zp‰‰ZqE’P€‰‰Zq<+>
VEC2F VEC2F::operator-() const { return (*this * -1.0f); }								// Zp‰‰ZqE’P€‰‰Zq<->
VEC2F VEC2F::operator+(const VEC2F& obj) const { return VEC2F(x + obj.x, y + obj.y); }	// Zp‰‰ZqE‰ÁZ
VEC2F VEC2F::operator+(const VEC2& obj) const { return VEC2F(x + obj.x, y + obj.y); }	// Zp‰‰ZqE‰ÁZ
VEC2F VEC2F::operator-(const VEC2F& obj) const { return VEC2F(x - obj.x, y - obj.y); }	// Zp‰‰ZqEŒ¸Z
VEC2F VEC2F::operator-(const VEC2& obj) const { return VEC2F(x - obj.x, y - obj.y); }	// Zp‰‰ZqEŒ¸Z
VEC2F VEC2F::operator*(const float value) const { return VEC2F(x * value, y * value); }	// Zp‰‰ZqEæZ(float)
VEC2F VEC2F::operator/(const float value) const { return VEC2F(x / value, y / value); }	// Zp‰‰ZqEœZ(float)
VEC2F VEC2F::operator*(const VEC2F& obj) const { return VEC2F(x * obj.x - y * obj.y, y * obj.x + x * obj.y); }																			// Zp‰‰ZqEæZ(•¡‘f”)
VEC2F VEC2F::operator/(const VEC2F& obj) const { return VEC2F((x * obj.x + y * obj.y) / (obj.x * obj.x + obj.y * obj.y), (y * obj.x - x * obj.y) / (obj.x * obj.x + obj.y * obj.y)); }	// Zp‰‰ZqEœZ(•¡‘f”)
VEC2F& VEC2F::operator+=(const VEC2F& obj) { return *this = *this + obj; }		// ‘ã“üZp‰‰ZqE‰ÁZ
VEC2F& VEC2F::operator+=(const VEC2& obj) { return *this = *this + obj; }		// ‘ã“üZp‰‰ZqE‰ÁZ
VEC2F& VEC2F::operator-=(const VEC2F& obj) { return *this = *this - obj; }		// ‘ã“üZp‰‰ZqEŒ¸Z
VEC2F& VEC2F::operator-=(const VEC2& obj) { return *this = *this - obj; }		// ‘ã“üZp‰‰ZqEŒ¸Z
VEC2F& VEC2F::operator*=(const float value) { return *this = *this * value; }	// ‘ã“üZp‰‰ZqEæZ(float)
VEC2F& VEC2F::operator/=(const float value) { return *this = *this / value; }	// ‘ã“üZp‰‰ZqEœZ(float)
VEC2F& VEC2F::operator*=(const VEC2F& obj) { return *this = *this * obj; }		// ‘ã“üZp‰‰ZqEæZ(•¡‘f”)
VEC2F& VEC2F::operator/=(const VEC2F& obj) { return *this = *this / obj; }		// ‘ã“üZp‰‰ZqEœZ(•¡‘f”)

/* “®“IŠÖ” */
float VEC2F::Length() const
{
	return sqrtf(x * x + y * y);
}

/* Ã“IŠÖ” */
float VEC2F::Dot(const VEC2F& v1, const VEC2F& v2) { return (v1.x * v2.x + v1.y * v2.y); }												// “àÏ
float VEC2F::Cross(const VEC2F& v1, const VEC2F& v2) { return (v1.x * v2.y - v1.y * v2.x); }											// ŠOÏ
float VEC2F::Dist(const VEC2F& v1, const VEC2F& v2) { return sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y)); }	// ‹——£

// ’PˆÊ‰»
VEC2F VEC2F::Normalize(const VEC2F& v)
{
	float angle = atan2f(v.y, v.x);
	return VEC2F(cosf(angle), sinf(angle));
}

// ü•ª‚Æü•ªEŒğ·”»’è
bool VEC2F::CheckIntersect_LineLine(const VEC2F& v1, const VEC2F& v2, const VEC2F& v3, const VEC2F& v4)
{
	return (
		((v3.x - v4.x) * (v1.y - v3.y) + (v3.y - v4.y) * (v3.x - v1.x)) *
		((v3.x - v4.x) * (v2.y - v3.y) + (v3.y - v4.y) * (v3.x - v2.x)) <= 0.0f &&
		((v1.x - v2.x) * (v3.y - v1.y) + (v1.y - v2.y) * (v1.x - v3.x)) *
		((v1.x - v2.x) * (v4.y - v1.y) + (v1.y - v2.y) * (v1.x - v4.x)) <= 0.0f);
}

// ü•ª‚Æü•ªEŒğ·“_‚ğæ“¾
VEC2F VEC2F::GetIntersectPos_LineLine(const VEC2F& v1, const VEC2F& v2, const VEC2F& v3, const VEC2F& v4)
{
	float det = ((v1.x - v2.x) * (v4.y - v3.y) - (v4.x - v3.x) * (v1.y - v2.y));
	float t = ((v4.y - v3.y) * (v4.x - v2.x) + (v3.x - v4.x) * (v4.y - v2.y)) / det;
	return v1 * t + v2 * (1.0f - t);
}

// ü•ª‚Æ‰~EŒğ·”»’è
bool VEC2F::CheckIntersect_lineCircle(const VEC2F& v1, const VEC2F& v2, const VEC2F& Pos, float Radius)
{
	if ((Pos.x - v1.x) * (Pos.x - v1.x) + (Pos.y - v1.y) * (Pos.y - v1.y) <= Radius * Radius ||
		(Pos.x - v2.x) * (Pos.x - v2.x) + (Pos.y - v2.y) * (Pos.y - v2.y) <= Radius * Radius)
		return true;
	{
		float angle = atan2f(v2.y - v1.y, v2.x - v1.x);
		if (cosf(atan2f(Pos.y - v1.y, Pos.x - v1.x) - angle) * cosf(atan2f(Pos.y - v2.y, Pos.x - v2.x) - angle) > 0.0f)
			return false;
	}
	if (fabsf(Cross(Normalize(v2 - v1), Pos - v1)) <= Radius)
		return true;
	return false;
}
