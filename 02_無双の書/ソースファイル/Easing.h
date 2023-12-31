#pragma once

namespace Easing
{
	namespace Sin	// �T�C���g
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Pow2	// 2��
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Pow3	// 3��
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Pow4	// 4��
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Pow5	// 5��
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
