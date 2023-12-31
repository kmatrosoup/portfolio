#pragma once

struct VEC2;
struct VEC2F;

/* �񎟌��x�N�g�� */
struct VEC2
{
	int x;
	int y;

	// �萔
	static const VEC2 ZERO;
	static const VEC2 ONE;

	// �R���X�g���N�^
	VEC2();
	VEC2(const VEC2& obj);
	VEC2(const VEC2F& obj);
	VEC2(int x, int y);

	// ���Z�q
	VEC2 operator+() const;						// �Z�p���Z�q�E�P�����Z�q<+>
	VEC2 operator-() const;						// �Z�p���Z�q�E�P�����Z�q<->
	VEC2 operator+(const VEC2& obj) const;		// �Z�p���Z�q�E���Z
	VEC2F operator+(const VEC2F& obj) const;	// �Z�p���Z�q�E���Z(VEC2F�v�Z)
	VEC2 operator-(const VEC2& obj) const;		// �Z�p���Z�q�E���Z
	VEC2F operator-(const VEC2F& obj) const;	// �Z�p���Z�q�E���Z(VEC2F�v�Z)
	VEC2 operator*(const int value) const;		// �Z�p���Z�q�E��Z
	VEC2F operator*(const float value) const;	// �Z�p���Z�q�E��Z(VEC2F�v�Z)
	VEC2 operator/(const int value) const;		// �Z�p���Z�q�E���Z
	VEC2F operator/(const float value) const;	// �Z�p���Z�q�E���Z(VEC2F�v�Z)
	VEC2& operator+=(const VEC2& obj);	// ����Z�p���Z�q�E���Z
	VEC2& operator-=(const VEC2& obj);	// ����Z�p���Z�q�E���Z
	VEC2& operator*=(const int value);	// ����Z�p���Z�q�E��Z
	VEC2& operator/=(const int value);	// ����Z�p���Z�q�E���Z
};

/* �񎟌��x�N�g��<float> */
struct VEC2F
{
	float x;
	float y;

	// �萔
	static const VEC2F ZERO;
	static const VEC2F ONE;

	// �R���X�g���N�^
	VEC2F();
	VEC2F(const VEC2F& obj);
	VEC2F(const VEC2& obj);
	VEC2F(float x, float y);

	// ���Z�q
	VEC2F operator+() const;					// �Z�p���Z�q�E�P�����Z�q<+>
	VEC2F operator-() const;					// �Z�p���Z�q�E�P�����Z�q<->
	VEC2F operator+(const VEC2F& obj) const;	// �Z�p���Z�q�E���Z
	VEC2F operator+(const VEC2& obj) const;		// �Z�p���Z�q�E���Z
	VEC2F operator-(const VEC2F& obj) const;	// �Z�p���Z�q�E���Z
	VEC2F operator-(const VEC2& obj) const;		// �Z�p���Z�q�E���Z
	VEC2F operator*(const float value) const;	// �Z�p���Z�q�E��Z(float)
	VEC2F operator/(const float value) const;	// �Z�p���Z�q�E���Z(float)
	VEC2F operator*(const VEC2F& obj) const;	// �Z�p���Z�q�E��Z(���f��)
	VEC2F operator/(const VEC2F& obj) const;	// �Z�p���Z�q�E���Z(���f��)
	VEC2F& operator+=(const VEC2F& obj);	// ����Z�p���Z�q�E���Z
	VEC2F& operator+=(const VEC2& obj);		// ����Z�p���Z�q�E���Z
	VEC2F& operator-=(const VEC2F& obj);	// ����Z�p���Z�q�E���Z
	VEC2F& operator-=(const VEC2& obj);		// ����Z�p���Z�q�E���Z
	VEC2F& operator*=(const float value);	// ����Z�p���Z�q�E��Z(float)
	VEC2F& operator/=(const float value);	// ����Z�p���Z�q�E���Z(float)
	VEC2F& operator*=(const VEC2F& obj);	// ����Z�p���Z�q�E��Z(���f��)
	VEC2F& operator/=(const VEC2F& obj);	// ����Z�p���Z�q�E���Z(���f��)

	// �֐�
	static float Dot(const VEC2F& v1, const VEC2F& v2);		// ����
	static float Cross(const VEC2F& v1, const VEC2F& v2);	// �O��
	static float Dist(const VEC2F& v1, const VEC2F& v2);	// ����
	static VEC2F Normalize(const VEC2F& v);					// �P�ʉ�
	static bool CheckIntersect_LineLine(const VEC2F& v1, const VEC2F& v2, const VEC2F& v3, const VEC2F& v4);	// �����Ɛ����E��������
	static VEC2F GetIntersectPos_LineLine(const VEC2F& v1, const VEC2F& v2, const VEC2F& v3, const VEC2F& v4);	// �����Ɛ����E�����_���擾
	static bool CheckIntersect_lineCircle(const VEC2F& v1, const VEC2F& v2, const VEC2F& Pos, float Radius);	// �����Ɖ~�E��������
};
