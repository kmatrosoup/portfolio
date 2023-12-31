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
	float paramR;	// �w�i�F��R�l( 0.0f�`1.0f )
	float paramG;	// �w�i�F��G�l( 0.0f�`1.0f )
	float paramB;	// �w�i�F��B�l( 0.0f�`1.0f )

private:
	aqua::CBoxPrimitive m_FillBox;	// �`��p�̋�`
};
