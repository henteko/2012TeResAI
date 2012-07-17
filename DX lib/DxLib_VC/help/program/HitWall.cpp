// 回転壁当たり判定プログラム
//
// Create 04/06/04
//
// 　回転する壁との当たり判定をしているプログラムです。
// 　主にしている事は壁となる線と、当たり判定をしたいオブジェクトの移動前と移動後を結ぶ線との交差判定です。
// 　微妙にベクトルの知識が必要となります。

#include "DxLib.h"
#include <stdio.h>
#include <math.h>

#define PI			(3.14159265358)		// π
#define PI_2		(3.14159265358)		// ２π
#define BOXSIZE		(128)				// 箱の大きさ
#define BOXX		(320)				// 箱のＸ座標
#define BOXY		(240)				// 箱のＹ座標
#define ROTSPEED	(PI_2 / 180)		// 箱の回転速度
#define SPEED		(8.0F)				// 点の速度
#define HITLENGTH	(4.0F)				// 箱の壁との当たり判定の幅

float rot ;				// 箱の回転角度
float x, y, sx, sy ;	// 箱の中を動く点の座標と速度

// 二次元ベクトル構造体
typedef struct tagVECT
{
	float x, y ;
} VECT ;

//  HitCheck 関数内の『直線に垂直に交わる点を求める演算』で行っていること、xを求めたい
//　なお　『*』：スカラー倍　　『・』：内積　　『大文字』：ベクトル　　『小文字』：スカラー値
//
// P1:直線の始点　V1:直線の向き　P2:交差判定をしたい直線の始点(又は終点)
// ( P1 + x * V1 ) ：P2 から 直線に垂直に交わる点の座標 
//
// ( ( P1 + x * V1 ) - P2 )・V1 = 0
// ( x * V1 + P1 - P2 )・V1 = 0 
// x * V1・V1 + P1・V1 - P2・V1 = 0  (← V1・V1 = 1 なので x * V1・V1 = x )
// x = P1・V1 - P2・V1
// x = V1・( P1 - P2 )
//
// というわけで、『ここから～ここまで』では x = V1・( P1 - P2 ) をやっています

// 線と線の交差判定
// l1x1,l1y1:直線１の始点　　l1x2,l1y2:直線１の終点
// l2x1,l2y1:直線２の始点　　l2x2,l2y2:直線２の終点
bool HitCheck( float l1x1, float l1y1, float l1x2, float l1y2,
				float l2x1, float l2y1, float l2x2, float l2y2 )
{
	VECT p1, v1, p2, v3, t ;
	float x, z1, z2, len ;

	// 直線の始点と向きを算出
	p1.x = l1x1 ;				p1.y = l1y1 ;			// 直線の始点
	v1.x = l1x2 - l1x1 ;		v1.y = l1y2 - l1y1 ;	// 向き

	len = sqrt( v1.x * v1.x + v1.y * v1.y ) ;			// 正規化
	v1.x /= len ;				v1.y /= len ;


	// 交差判定をしたい線の始点と、始点から直線に垂直に交わる直線上の点の座標を求める
	p2.x = l2x1 ;				p2.y = l2y1 ;				// 当たり判定をしたい線の始点

	// 直線に垂直に交わる点を求める演算
	t.x = p2.x - p1.x ;			t.y = p2.y - p1.y ;			// ここから
	x = v1.x * t.x + v1.y * t.y ;							// (←この行で内積してます) 
															// 
	v3.x = p1.x + v1.x * x ;	v3.y = p1.y + v1.y * x ;	// ここまで

	t.x = v3.x - p2.x ;			t.y = v3.y - p2.y ;					// 始点から求めた直線上の点へのベクトルを算出
	if( sqrt( t.x * t.x + t.y * t.y ) < HITLENGTH ) return true ;	// 求めた点までの距離が HITLENGTH 以下だったら線に当たった事にする

	z1 = v1.x * t.y - v1.y * t.x ;			// 始点から求めた点へのベクトルと、直線の向きとの外積を求めておく・・・①


	// 交差判定をしたい線の終点と、終点から直線に垂直に交わる直線上の点の座標を求める
	p2.x = l2x2 ;				p2.y = l2y2 ;				// 当たり判定をしたい線の終点

	// 直線に垂直に交わる点を求める演算
	t.x = p2.x - p1.x ;			t.y = p2.y - p1.y ;			// ここから
	x = v1.x * t.x + v1.y * t.y ;							// (←この行で内積してます) 
															// 
	v3.x = p1.x + v1.x * x ;	v3.y = p1.y + v1.y * x ;	// ここまで

	t.x = v3.x - p2.x ;			t.y = v3.y - p2.y ;					// 終点から求めた直線上の点へのベクトルを算出
	if( sqrt( t.x * t.x + t.y * t.y ) < HITLENGTH ) return true ;	// 求めた点までの距離が HITLENGTH 以下だったら線に当たった事にする

	z2 = v1.x * t.y - v1.y * t.x ;			// 終点から求めた点へのベクトルと、直線の向きとの外積を求めておく・・・②


	// ①と②で求めたベクトルの向きが違う場合は交差判定をしたい線と交差した証拠
	return ( z1 >= 0.0F && z2 < 0.0F ) || ( z2 >= 0.0F && z1 < 0.0F ) ;
}


// WinMain 関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
	float bx, by, px[4], py[4], f ;
	int i ;

	ChangeWindowMode( TRUE ) ;						// ウインドウモードで起動
	if( DxLib_Init() < 0 ) return -1 ;				// ＤＸライブラリ初期化

	rot = 0.0F ;									// 箱の回転角度を初期化
	x = 320 ;		y = 240 ;						// 点の位置を初期化
	sx = rand() * SPEED / RAND_MAX - SPEED / 2 ;	// 点の速度を初期化
	sy = rand() * SPEED / RAND_MAX - SPEED / 2 ;

	// メインループ( メッセージ処理が失敗するか、ＥＳＣキーが押されるまでループ )
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		ClearDrawScreen() ;		// 画面初期化

		bx = x ;	by = y ;	// 移動前の座標を保存しておく
		x += sx ;	y += sy ;	// 点の座標の移動

		rot += ROTSPEED ;								// 箱の回転角度を変更
		if( rot > PI_2 ) rot -= PI_2 ;					// 一周していたら補正をする
		px[0] = - BOXSIZE ;		py[0] = - BOXSIZE ;		// 箱の四隅の座標の初期値をセット
		px[1] = + BOXSIZE ;		py[1] = - BOXSIZE ;
		px[2] = + BOXSIZE ;		py[2] = + BOXSIZE ;
		px[3] = - BOXSIZE ;		py[3] = + BOXSIZE ;
		for( i = 0 ; i < 4 ; i ++ )						// 箱の四隅の座標を現在の回転角度に合わせて回転
		{
			f =		px[i] * cos( rot ) - py[i] * sin( rot ) ;
			py[i] =	px[i] * sin( rot ) + py[i] * cos( rot ) ;
			px[i] = f ;

			px[i] += BOXX ;		// 回転した後、表示したい座標に移動
			py[i] += BOXY ;
		}

		// 箱の四辺と点との当たり判定(点の移動前と移動後を結ぶ線が箱の四辺と交差したかを判定)
		if( HitCheck( px[0], py[0], px[1], py[1], bx, by, x, y ) == true ||
			HitCheck( px[1], py[1], px[2], py[2], bx, by, x, y ) == true ||
			HitCheck( px[2], py[2], px[3], py[3], bx, by, x, y ) == true ||
			HitCheck( px[3], py[3], px[0], py[0], bx, by, x, y ) == true )
		{
			// 当たって(交差して)いたら点の位置を元に戻す
			x = 320 ;		y = 240 ;						// 位置初期化
			sx = rand() * SPEED / RAND_MAX - SPEED / 2 ;	// 速度設定
			sy = rand() * SPEED / RAND_MAX - SPEED / 2 ;
		}

		// 箱の四辺を描画
		DrawLine( px[0], py[0], px[1], py[1], GetColor( 255, 255, 255 ) ) ;
		DrawLine( px[1], py[1], px[2], py[2], GetColor( 255, 255, 255 ) ) ;
		DrawLine( px[2], py[2], px[3], py[3], GetColor( 255, 255, 255 ) ) ;
		DrawLine( px[3], py[3], px[0], py[0], GetColor( 255, 255, 255 ) ) ;

		// 点を描画
		DrawPixel( x, y, GetColor( 255, 255, 255 ) ) ;

		// 画面を更新
		ScreenFlip() ;
	}

	DxLib_End() ;	// ＤＸライブラリの後始末

	return 0 ;		// ソフト終了
}


