
/*!
 *  @file       surface.cpp
 *  @brief      サーフェイスクラス
 *  @author     Kazuya Maruyama
 *  @date       2018/12/19
 *  @version    7.17
 *
 *  Copyright (c) 2013-2021, Kazuya Maruyama. All rights reserved.
 */

#include <DxLib.h>
#include "surface.h"
#include "..\..\debug\debug.h"
#include "..\..\utility\memory\memory.h"

std::list<int> aqua::CSurface::m_ScreenStack;

/*
 *  コンストラクタ
 */
aqua::CSurface::
CSurface( void )
{
}

/*
 *  サーフェイス作成
 */
void
aqua::CSurface::
Create( int width, int height, bool alpha_channel )
{
    m_Texture.Create( width, height, alpha_channel );
}

/*
 *  サーフェイス解放
 */
void
aqua::CSurface::
Delete( void )
{
    m_Texture.Delete( );
}

/*
 *  サーフェイスへの描画開始
 */
void
aqua::CSurface::
Begin( void )
{
    //if( !m_Texture.IsEnable( ) )
    //    return;

    //// 生成したサーフェイスに書き込む
    //SetDrawScreen( m_Texture.GetResourceHandle( ) );

    m_ScreenStack.push_back(m_Texture.GetResourceHandle());
    SetDrawScreen(m_ScreenStack.back());
}

/*
 *  サーフェイスへの描画終了
 */
void
aqua::CSurface::
End( void )
{
    //// 書き込み先をバックバッファに戻す
    //SetDrawScreen( DX_SCREEN_BACK );

    m_ScreenStack.pop_back();
    m_ScreenStack.empty() ?
        SetDrawScreen(DX_SCREEN_BACK) :
        SetDrawScreen(m_ScreenStack.back());
}

/*
 *  描画先のスクリーンをサーフェイスにコピー
 */
void
aqua::CSurface::
CopyDrawScreen( void )
{
    if( !m_Texture.IsEnable( ) )
        return;

    // 描画先のスクリーンをサーフェイスにコピー
    GetDrawScreenGraph( 0, 0, m_Texture.GetWidth( ), m_Texture.GetHeight( ), m_Texture.GetResourceHandle( ) );
}

/*
 *  テクスチャの取得
 */
aqua::CTexture
aqua::CSurface::
GetTexture( void )
{
    return m_Texture;
}

/*
 *  リソースの読み込み(未使用)
 */
void
aqua::CSurface::
Load( const std::string& file_name )
{
    (void)file_name;
}

/*
 *  リソースの解放(未使用)
 */
void
aqua::CSurface::
Unload( void )
{
}
