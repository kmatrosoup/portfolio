#pragma once

struct VEC2;
struct VEC2F;

/* “ñŸŒ³ƒxƒNƒgƒ‹ */
struct VEC2
{
	int x;
	int y;

	// ’è”
	static const VEC2 ZERO;
	static const VEC2 ONE;

	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	VEC2();
	VEC2(const VEC2& obj);
	VEC2(const VEC2F& obj);
	VEC2(int x, int y);

	// ‰‰Zq
	VEC2 operator+() const;						// Zp‰‰ZqE’P€‰‰Zq<+>
	VEC2 operator-() const;						// Zp‰‰ZqE’P€‰‰Zq<->
	VEC2 operator+(const VEC2& obj) const;		// Zp‰‰ZqE‰ÁZ
	VEC2F operator+(const VEC2F& obj) const;	// Zp‰‰ZqE‰ÁZ(VEC2FŒvZ)
	VEC2 operator-(const VEC2& obj) const;		// Zp‰‰ZqEŒ¸Z
	VEC2F operator-(const VEC2F& obj) const;	// Zp‰‰ZqEŒ¸Z(VEC2FŒvZ)
	VEC2 operator*(const int value) const;		// Zp‰‰ZqEæZ
	VEC2F operator*(const float value) const;	// Zp‰‰ZqEæZ(VEC2FŒvZ)
	VEC2 operator/(const int value) const;		// Zp‰‰ZqEœZ
	VEC2F operator/(const float value) const;	// Zp‰‰ZqEœZ(VEC2FŒvZ)
	VEC2& operator+=(const VEC2& obj);	// ‘ã“üZp‰‰ZqE‰ÁZ
	VEC2& operator-=(const VEC2& obj);	// ‘ã“üZp‰‰ZqEŒ¸Z
	VEC2& operator*=(const int value);	// ‘ã“üZp‰‰ZqEæZ
	VEC2& operator/=(const int value);	// ‘ã“üZp‰‰ZqEœZ
};

/* “ñŸŒ³ƒxƒNƒgƒ‹<float> */
struct VEC2F
{
	float x;
	float y;

	// ’è”
	static const VEC2F ZERO;
	static const VEC2F ONE;

	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	VEC2F();
	VEC2F(const VEC2F& obj);
	VEC2F(const VEC2& obj);
	VEC2F(float x, float y);

	// ‰‰Zq
	VEC2F operator+() const;					// Zp‰‰ZqE’P€‰‰Zq<+>
	VEC2F operator-() const;					// Zp‰‰ZqE’P€‰‰Zq<->
	VEC2F operator+(const VEC2F& obj) const;	// Zp‰‰ZqE‰ÁZ
	VEC2F operator+(const VEC2& obj) const;		// Zp‰‰ZqE‰ÁZ
	VEC2F operator-(const VEC2F& obj) const;	// Zp‰‰ZqEŒ¸Z
	VEC2F operator-(const VEC2& obj) const;		// Zp‰‰ZqEŒ¸Z
	VEC2F operator*(const float value) const;	// Zp‰‰ZqEæZ(float)
	VEC2F operator/(const float value) const;	// Zp‰‰ZqEœZ(float)
	VEC2F operator*(const VEC2F& obj) const;	// Zp‰‰ZqEæZ(•¡‘f”)
	VEC2F operator/(const VEC2F& obj) const;	// Zp‰‰ZqEœZ(•¡‘f”)
	VEC2F& operator+=(const VEC2F& obj);	// ‘ã“üZp‰‰ZqE‰ÁZ
	VEC2F& operator+=(const VEC2& obj);		// ‘ã“üZp‰‰ZqE‰ÁZ
	VEC2F& operator-=(const VEC2F& obj);	// ‘ã“üZp‰‰ZqEŒ¸Z
	VEC2F& operator-=(const VEC2& obj);		// ‘ã“üZp‰‰ZqEŒ¸Z
	VEC2F& operator*=(const float value);	// ‘ã“üZp‰‰ZqEæZ(float)
	VEC2F& operator/=(const float value);	// ‘ã“üZp‰‰ZqEœZ(float)
	VEC2F& operator*=(const VEC2F& obj);	// ‘ã“üZp‰‰ZqEæZ(•¡‘f”)
	VEC2F& operator/=(const VEC2F& obj);	// ‘ã“üZp‰‰ZqEœZ(•¡‘f”)

	// ŠÖ”
	static float Dot(const VEC2F& v1, const VEC2F& v2);		// “àÏ
	static float Cross(const VEC2F& v1, const VEC2F& v2);	// ŠOÏ
	static float Dist(const VEC2F& v1, const VEC2F& v2);	// ‹——£
	static VEC2F Normalize(const VEC2F& v);					// ’PˆÊ‰»
	static bool CheckIntersect_LineLine(const VEC2F& v1, const VEC2F& v2, const VEC2F& v3, const VEC2F& v4);	// ü•ª‚Æü•ªEŒğ·”»’è
	static VEC2F GetIntersectPos_LineLine(const VEC2F& v1, const VEC2F& v2, const VEC2F& v3, const VEC2F& v4);	// ü•ª‚Æü•ªEŒğ·“_‚ğæ“¾
	static bool CheckIntersect_lineCircle(const VEC2F& v1, const VEC2F& v2, const VEC2F& Pos, float Radius);	// ü•ª‚Æ‰~EŒğ·”»’è
};
