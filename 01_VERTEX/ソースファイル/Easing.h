#pragma once

namespace Easing
{
	namespace Sin	// サイン波
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Quad	// 2乗
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Cubic	// 3乗
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Quart	// 4乗
	{
		float In(float param);
		float Out(float param);
		float InOut(float param);
	}

	namespace Quint	// 5乗
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
