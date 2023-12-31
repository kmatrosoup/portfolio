#pragma once

namespace Easing
{
	namespace Sin	// �T�C���g
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Quad	// 2��
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Cubic	// 3��
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Quart	// 4��
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Quint	// 5��
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Elastic	// �o�b�N
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Bounce	// �o�E���h
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}
};
