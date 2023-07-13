#pragma once

#include "aqua.h"

class CCommonData;
class CSoundManager;
class CGlowScreen;

class CGame
    : public aqua::IGameObject
{
public:
    CGame( aqua::IGameObject* parent );
    ~CGame( void ) = default;

    void        Initialize( void ) override;
    void        Update( void ) override;
    void        Draw( void ) override;

private:
    CCommonData* m_pCommonData;
    CSoundManager* m_pSoundManager;
    CGlowScreen* m_pGlowScreen;
};
