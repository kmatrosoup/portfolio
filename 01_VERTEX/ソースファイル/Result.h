#pragma once

#include "BaseScene.h"
#include "System.h"
#include <string>

enum class eRank
{
	ONE,
	TWO,
	THREE,
	FOUR,

	MAX
};
enum class eSTATUS;

class cResult : public cBaseScene
{
public:
	/* 公開関数 */
	cResult();						// コンストラクタ
	~cResult() override;			// デストラクタ
	cBaseScene* Update() override;	// 更新
	void Draw() const override;		// 描画

private:
	/* 非公開関数 */

	cResult(const cResult&) = delete;
	cResult& operator=(const cResult&) = delete;
	cResult(cResult&&) = delete;
	cResult& operator=(cResult&&) = delete;

private:
	/* 非公開変数 */
	static constexpr int BoxPos_H = Sh / 2;
	static constexpr int BoxPos_W = Sw / 2;

	int PlayerGraph[24];
	int PlayerColorGraph[24];

	int MedalGraph[4];

	static constexpr unsigned int PlayerColor[(int)eRank::MAX] = { 0xff0000, 0x0000ff, 0x00ff00, 0xffff00 };

	static const int AlphaBlend;
};
