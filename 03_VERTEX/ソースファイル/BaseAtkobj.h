#pragma once

#include "VEC2.h"
class cPlayer;

class cBaseAtkobj
{
	/* 公開関数 */
public:
	cBaseAtkobj(cPlayer* Owner);			// コンストラクタ
	virtual ~cBaseAtkobj() = default;		// デストラクタ
	virtual void Update() = 0;				// 更新
	virtual void Draw_Default() const {};	// 描画・DEFAULT
	virtual void Draw_FadeAdd() const {};	// 描画・FADE_ADD

	virtual bool Active() const = 0;	// アクティブ状態

	/* 非公開変数 */
protected:
	cPlayer* m_Owner;	// 所有プレイヤー
};
