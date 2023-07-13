#pragma once

#include "BaseScene.h"

class cPreview : public cBaseScene
{
	/* 公開関数 */
public:
	cPreview();						// コンストラクタ
	~cPreview() override;			// デストラクタ
	cBaseScene* Update() override;	// 更新
	void Draw() const override;		// 描画

	/* 非公開関数 */
private:
	cPreview(const cPreview&) = delete;
	cPreview& operator=(const cPreview&) = delete;
	cPreview(cPreview&&) = delete;
	cPreview& operator=(cPreview&&) = delete;

	/* 非公開変数 */
private:
	static const int m_MaxTime;	// 表示時間

	int m_Timer;				// 経過時間

	int m_Image_Circle;	// 画像ハンドル・円
};
