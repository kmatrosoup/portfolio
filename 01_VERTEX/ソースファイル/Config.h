#pragma once

#include "BaseScene.h"

class cConfig : public cBaseScene
{
	/* 公開関数 */
public:
	cConfig();						// コンストラクタ
	~cConfig() override;			// デストラクタ
	cBaseScene* Update() override;	// 更新
	void Draw() const override;		// 描画

	/* 非公開関数 */
private:
	cConfig(const cConfig&) = delete;
	cConfig& operator=(const cConfig&) = delete;
	cConfig(cConfig&&) = delete;
	cConfig& operator=(cConfig&&) = delete;

	/* 非公開変数 */
private:
	static int m_Cursor;	// カーソル位置
};
