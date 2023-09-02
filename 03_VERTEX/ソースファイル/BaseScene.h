#pragma once

class cBaseScene
{
	/* 公開関数 */
public:
	virtual ~cBaseScene() = default;	// デストラクタ
	virtual cBaseScene* Update() = 0;	// 更新
	virtual void Draw() const = 0;		// 描画
};
