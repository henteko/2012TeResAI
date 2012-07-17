// シューティングプログラム

#include "DxLib.h"

#define TARGETNUM	(3)		// ターゲットの数
#define TARGETSIZE	(16)	// ターゲットの大きさ
#define TARGETWAIT	(60)	// 一度撃たれてから復活するまでのフレーム数

#define SHOTNUM		(9)		// ショットの数
#define SHOTSIZE	(8)		// ショットのサイズ
#define SHOTSPEED	(8)		// ショットのスピード

#define PLAYERSPEED	(5)		// プレイヤーのスピード
#define PLAYERSIZE	(16)	// プレイヤーのサイズ

#define HIT3TIME	(60)	// ３つ同時にヒットした時の「３つ同時にひっとした」を表示するフレーム数


// ターゲットの情報
int TargetX[TARGETNUM], TargetY[TARGETNUM] ; // 座標
int TargetWait[TARGETNUM] ; // 攻撃が当たってからの出現待ち処理用( 0:出現中  0以上:出現待ち )

// ショットの情報
int ShotX[SHOTNUM], ShotY[SHOTNUM] ; // 座標
int ShotSX[SHOTNUM], ShotSY[SHOTNUM] ; // 速度
int ShotValidFlag[SHOTNUM] ; // 使用中かどうかのフラグ変数( 1:使用中  0:使っていない )

// 自機の情報
int PlayerX, PlayerY ; // 座標

// ３つ同時ヒット時の「３つ同時ヒットした」表示用カウンタ
// ( 0:非表示  0以上:表示 )
int Hit3Counter ;

// 入力情報
int Input, EdgeInput ;

// ショットを増やす( x, y : 座標   sx, sy : 速度 )
void AddShot( int x, int y, int sx, int sy )
{
	int i ;
	
	// 使われていないショットを探す
	for( i = 0 ; i < SHOTNUM ; i ++ )
		if( ShotValidFlag[i] == 0 ) break ;
		
	// 全部使われていたら追加しない
	if( i == SHOTNUM ) return ;

	// 情報セット
	ShotX[i] = x ;		ShotY[i] = y ;		// 座標セット
	ShotSX[i] = sx ;	ShotSY[i] = sy ;	// 速度セット
	ShotValidFlag[i] = 1 ;	// 使用中にする
}

// WinMain 関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
	int i, j, HitNum ;

	ChangeWindowMode( TRUE ) ; // 一応ウインドウモードで起動
	if( DxLib_Init() < 0 ) return -1 ; // ＤＸライブラリの初期化
	SetDrawScreen( DX_SCREEN_BACK ) ; // 描画先を裏画面にする
	
	// 「３つ同時にヒットした」表示用カウンタを初期化
	Hit3Counter = 0 ;

	// ターゲットの情報を初期化
	TargetX[0] = 320 ;	TargetY[0] = 100 ;	TargetWait[0] = 0 ;
	TargetX[1] = 100 ;	TargetY[1] = 100 ;	TargetWait[1] = 0 ;
	TargetX[2] = 540 ;	TargetY[2] = 100 ;	TargetWait[2] = 0 ;
	
	// ショットの情報の初期化
	for( i = 0 ; i < SHOTNUM ; i ++ ) ShotValidFlag[i] = 0 ; // 全部使っていないことにする
	
	// プレイヤーの情報初期化
	PlayerX = 320 ;	PlayerY = 400 ;

	// メインループ
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		ClearDrawScreen() ; // 画面初期化
		
		// 入力情報更新
		{
			int inp ;
			
			inp = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ; 
			EdgeInput = inp & ~Input ;
			Input = inp ;
		}
		
		// ターゲットの処理
		for( i = 0 ; i < TARGETNUM ; i ++ )
		{
			// 復活待ちかどうかで処理を分岐
			if( TargetWait[i] != 0 )
			{
				TargetWait[i] -- ; // 復活待ちカウンタが０では無い時は１減らす
			}
			else
			{
				// ターゲットの表示
				DrawBox( TargetX[i] - TARGETSIZE, TargetY[i] - TARGETSIZE,
						 TargetX[i] + TARGETSIZE, TargetY[i] + TARGETSIZE, GetColor( 255, 0, 0 ), TRUE ) ;
			}			
		}

		// ショットの処理
		HitNum = 0 ; // 同時にヒットした数を初期化
		for( i = 0 ; i < SHOTNUM ; i ++ )
		{
			if( ShotValidFlag[i] == 0 ) continue ; // 使用中では無ければ何もしない
			
			ShotX[i] += ShotSX[i] ; ShotY[i] += ShotSY[i] ; // 移動処理
			
			// 画面外に出ていたら未使用にする
			if( ShotY[i] < -60 )
			{
				 ShotValidFlag[i] = 0 ;
				 continue ;
			}
			
			// ショットの表示
			DrawBox( ShotX[i] - SHOTSIZE, ShotY[i] - SHOTSIZE,
					 ShotX[i] + SHOTSIZE, ShotY[i] + SHOTSIZE, GetColor( 255, 255, 255 ), TRUE ) ;
			
			// ターゲットとの当たり判定
			for( j = 0 ; j < TARGETNUM ; j ++ )
			{
				if( TargetWait[j] != 0 ) continue ; // 撃たれてからまだ復活していない時は当たり判定を行わない
				
				// 判定
				if( ( ( ShotX[i] > TargetX[j] - TARGETSIZE && ShotX[i] < TargetX[j] + TARGETSIZE ) &&
					  ( ShotY[i] > TargetY[j] - TARGETSIZE && ShotY[i] < TargetY[j] + TARGETSIZE ) ) ||
				  	( ( TargetX[j] > ShotX[i] - SHOTSIZE && TargetX[j] < ShotX[i] + SHOTSIZE ) &&
					  ( TargetY[j] > ShotY[i] - SHOTSIZE && TargetY[j] < ShotY[i] + SHOTSIZE ) ) )
				{
					ShotValidFlag[i] = 0 ; // ヒットしたらショットが未使用に
					TargetWait[j] = TARGETWAIT ;  // ターゲットは復活待ちに
					HitNum ++ ; // 同時にヒットした数を１増やす
				}
			}
		}

		// 同時にヒットした数が３つだったら「３つ同時にヒットした」カウンタをセット
		if( HitNum == 3 )
		{
			Hit3Counter = HIT3TIME ;
		}
		
		// 「３つ同時にヒットした」表示処理
		if( Hit3Counter != 0 )
		{
			DrawString( 0, 0, "３つ同時にヒットした", GetColor( 255, 255, 255 ) ) ; // ３つ同時にヒットした知らせを表示
			Hit3Counter -- ; // 表示中はカウンタを常に減らす
		}

		// プレイヤーの処理
		if( Input & PAD_INPUT_LEFT ) PlayerX -= PLAYERSPEED ;
		if( Input & PAD_INPUT_RIGHT ) PlayerX += PLAYERSPEED ;
		if( EdgeInput & PAD_INPUT_A )
		{
			// ３ＷＡＹ砲発射
			AddShot( PlayerX, PlayerY, 0, -8 ) ;
			AddShot( PlayerX, PlayerY, 6, -8 ) ;
			AddShot( PlayerX, PlayerY, -6, -8 ) ;
		}
		DrawBox( PlayerX - PLAYERSIZE, PlayerY - PLAYERSIZE,
				 PlayerX + PLAYERSIZE, PlayerY + PLAYERSIZE, GetColor( 0, 255, 0 ), TRUE ) ;
	 
		// 画面の更新
		ScreenFlip() ;
	}

	DxLib_End() ; // ＤＸライブラリの後始末
	
	return 0 ; // ソフト終了
}




