#include "DxLib.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int grhandle, pshandle ;
	VERTEX2DSHADER Vert[ 6 ] ;

	// ウインドウモードで起動
	ChangeWindowMode( TRUE );

	// ＤＸライブラリの初期化
	if( DxLib_Init() < 0 ) return -1;

	// 画像の読み込み
	grhandle = LoadGraph( "Test1.bmp" );

	// ピクセルシェーダーバイナリコードの読み込み
	pshandle = LoadPixelShader( "SamplePS.pso" ) ;

	// 頂点データの準備
	Vert[ 0 ].pos = VGet(   0.0f,   0.0f, 0.0f ) ;
	Vert[ 1 ].pos = VGet( 128.0f,   0.0f, 0.0f ) ;
	Vert[ 2 ].pos = VGet(   0.0f, 512.0f, 0.0f ) ;
	Vert[ 3 ].pos = VGet( 128.0f, 512.0f, 0.0f ) ;
	Vert[ 0 ].dif = GetColorU8( 255,255,255,255 ) ;
	Vert[ 0 ].spc = GetColorU8( 0,0,0,0 ) ;
	Vert[ 0 ].u = 0.0f ; Vert[ 0 ].v = 0.0f ;
	Vert[ 1 ].u = 1.0f ; Vert[ 1 ].v = 0.0f ;
	Vert[ 2 ].u = 0.0f ; Vert[ 2 ].v = 1.0f ;
	Vert[ 3 ].u = 1.0f ; Vert[ 3 ].v = 1.0f ;
	Vert[ 0 ].su = 0.0f ; Vert[ 0 ].sv = 0.0f ;
	Vert[ 1 ].su = 1.0f ; Vert[ 1 ].sv = 0.0f ;
	Vert[ 2 ].su = 0.0f ; Vert[ 2 ].sv = 1.0f ;
	Vert[ 3 ].su = 1.0f ; Vert[ 3 ].sv = 1.0f ;
	Vert[ 0 ].rhw = 1.0f ;
	Vert[ 1 ].rhw = 1.0f ;
	Vert[ 2 ].rhw = 1.0f ;
	Vert[ 3 ].rhw = 1.0f ;
	Vert[ 4 ] = Vert[ 2 ] ;
	Vert[ 5 ] = Vert[ 1 ] ;

	// 使用するテクスチャをセット
	SetUseTextureToShader( 0, grhandle ) ;

	// 使用するピクセルシェーダーをセット
	SetUsePixelShader( pshandle ) ;

	// 描画
	DrawPrimitive2DToShader( Vert, 6, DX_PRIMTYPE_TRIANGLELIST ) ;

	// キー入力待ち
	WaitKey() ;

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}
