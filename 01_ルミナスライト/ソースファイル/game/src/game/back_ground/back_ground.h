#pragma once

#include "aqua.h"

class CBackGround : public aqua::IGameObject
{
public:
	CBackGround(aqua::IGameObject* parent);
	~CBackGround() override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

public:
	float paramR;	// ”wŒiF‚ÌR’l( 0.0f`1.0f )
	float paramG;	// ”wŒiF‚ÌG’l( 0.0f`1.0f )
	float paramB;	// ”wŒiF‚ÌB’l( 0.0f`1.0f )

private:
	aqua::CBoxPrimitive m_FillBox;	// •`‰æ—p‚Ì‹éŒ`
};
