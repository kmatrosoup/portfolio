#pragma once

namespace Easing
{
	namespace Sin	// サイン波
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Pow2	// 2乗
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Pow3	// 3乗
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Pow4	// 4乗
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Pow5	// 5乗
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Elastic	// バック
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Bounce	// バウンド
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}
};
