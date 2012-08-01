#include "DxLib.h"
#include <math.h>

#define MAX_L	20					// レーザーの最大数定義
#define PI		3.1415926535897932		// π

#define LINE_MAXNUM	1000				// 描画する線の最大数

// データ型宣言

// ライン描画用構造体宣言
typedef struct tugLINE
{
	int x , y ;					// 描くラインの座標
	double Angle ;					// ラインの向き
	int Counter ;					// 描くラインの色決定用値
} LINE , *LPLINE ;

// レーザー構造体型宣言
typedef struct tugLASER
{
	int x , y ;					// 現在の座標
	int sx , sy ;					// 現在の速度

	int LogNum ;					// 記録した軌跡の数
	double Angle ;					// 進んでいる角度
	int Counter ;					// 追尾をはじめてから経過した時間

	LINE Line[LINE_MAXNUM] ;			// レーザーのラインデータ
	int LineNum ;					// 表示されているラインの数

	int ValidFlag ;					// このデータが使用中かフラグ
} LASER , *LPLASER ;

// データ定義

int Hx , Hy ;						// 砲台の座標
int Hm ;						// 砲台の移動方向
int Hsc ;						// 砲台のショット間隔カウンタ
int Px , Py ;						// 自機の座標

LASER Ls[MAX_L] ;					// ホーミングレーザーのデータ
int Lg, Lg2 ;						// レーザーのグラフィックハンドル


// WinMain関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, int nCmdShow )
{
	int Key ;
	LONGLONG Time ;
	int i ;

	// 画面モードの設定
	SetGraphMode( 640 , 480 , 16 ) ;

	// ＤＸライブラリ初期化処理
	if( DxLib_Init() == -1 )
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	// 描画先を裏画面にセット
	SetDrawScreen( DX_SCREEN_BACK ) ;

	// 初期化処理
	{
		// レーザーグラフィックのロード
		Lg = LoadGraph( "Laser.bmp" ) ;
		Lg2 = LoadGraph( "Laser2.bmp" ) ;

		// 自機の座標セット
		Px = 320 ; Py = 200 ;

		// 砲台の座標セット
		Hx = 320 ; Hy = 30 ;

		// 砲台の移動方向セット
		Hm = 3 ;

		// 砲台の移動間隔カウンタセット
		Hsc = 30 ;

		// レーザーデータの初期化
		for( i = 0 ; i < MAX_L ; i ++ )
			Ls[ i ].ValidFlag = 0 ;
	}

	// ゲームループ
	Time = GetNowHiPerformanceCount() + 1000000 / 60 ;
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		// プレイヤーの移動処理
		{
			// 入力取得
			Key = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ;

			if( Key & PAD_INPUT_RIGHT ) Px += 5 ;	// 右を押していたら右に進む
			if( Key & PAD_INPUT_LEFT ) Px -= 5 ;	// 左を押していたら左に進む

			if( Key & PAD_INPUT_UP ) Py -= 5 ;	// 上を押していたら上に進む
			if( Key & PAD_INPUT_DOWN ) Py += 5 ;	// 下を押していたら下に進む

			// 画面外に出ていたら補正
			if( Px > 640 - 16 ) Px = 640 - 16 ;
			if( Px < 0 ) Px = 0 ;

			if( Py > 480 - 16 ) Py = 480 - 16 ;
			if( Py < 0 ) Py = 0 ;
		}

		// レーザーの移動処理
		for( i = 0 ; i < MAX_L ; i ++ )
		{
			int xb , yb ;

			// レーザーデータが無効だったらスキップ
			if( Ls[ i ].ValidFlag == 0 ) continue ;

			// 照準に当たっていたらレーザーデータを無効にする
			if( ( Ls[ i ].x > Px && Ls[ i ].x < Px + 32 ) &&
				( Ls[ i ].y > Py && Ls[ i ].y < Py + 32 ) )
			{
				Ls[ i ].ValidFlag = 0 ;
				continue  ;
			}

			// 追尾カウンタが規定値に来ていなければ追尾処理
			if( Ls[ i ].Counter < 100 )
			{
				double ax , ay , bx , by ;
				double ar , br ;

				// bx,by 自分の進んでいる方向 ax,ay 本来進むべき方向  
				bx = cos( Ls[ i ].Angle ) ;
				by = sin( Ls[ i ].Angle ) ;
				ax = Px - Ls[ i ].x + 16 ;
				ay = Py - Ls[ i ].y + 16 ;

				// ベクトルb と aの絶対値を求める
				br = sqrt( bx * bx + by * by ) ;
				ar = sqrt( ax * ax + ay * ay ) ;

				// 外積を利用し向きを照準側に向ける
				Ls[ i ].Angle += PI / 180 * ( ( bx * ay - by * ax ) / ( br * ar ) ) * 5 ;
			}

			// 追尾カウンタ加算
			Ls[ i ].Counter ++ ;

			// 速度を変更する
			Ls[ i ].sx = ( int )( cos( Ls[ i ].Angle ) * 1000 );
			Ls[ i ].sy = ( int )( sin( Ls[ i ].Angle ) * 1000 );

			// 移動前のアドレスを保存
			xb = Ls[ i ].x ;
			yb = Ls[ i ].y ;

			// 移動する
			Ls[ i ].x = ( Ls[ i ].x * 100 + Ls[ i ].sx ) / 100 ;
			Ls[ i ].y = ( Ls[ i ].y * 100 + Ls[ i ].sy ) / 100 ;

			// 現在の状態をラインデータに変換
			{
				int j ;

				if( Ls[ i ].LineNum != LINE_MAXNUM )
				{
					// ライン情報をセットする
					j = Ls[ i ].LineNum ;

					// 座標のセット
					Ls[ i ].Line[ j ].x = xb ; Ls[ i ].Line[ j ].y = yb ;

					// 角度をセット
					{
						double r ;

						r = sqrt( Ls[ i ].sx * Ls[ i ].sx + Ls[ i ].sy * Ls[ i ].sy ) ;
						Ls[ i ].Line[ j ].Angle = atan2( Ls[ i ].sy , Ls[ i ].sx ) ;
					}

					// 色決定カウンタを初期化
					Ls[ i ].Line[ j ].Counter = 0 ;

					// ラインの数を増やす
					Ls[ i ].LineNum ++ ; 
				}
			}

			// 画面外に出ていたらレーザーデータを無効にする
			if( Ls[ i ].x < -100 || Ls[ i ].x > 740 ||
				Ls[ i ].y < -100 || Ls[ i ].y > 580 ) Ls[ i ].ValidFlag = 0 ;
		}

		// 砲台の移動処理
		{
			Hx += Hm ;

			// 画面端まで来ていたら反転
			if( Hx > 640 - 16 || Hx < 0 ) Hm *= -1 ;
			
			// ショットカウンタを減らす
			Hsc -- ;

			// カウンタが０になっていたらレーザー発射
			if( Hsc == 0 )
			{
				// 使われていないレーザーデータを探す
				for( i = 0 ; i < MAX_L ; i ++ )
				{
					if( Ls[ i ].ValidFlag == 0 && Ls[ i ].LineNum == 0 ) break ;
				}
		
				// もし使われていないレーザーデータがあったらショットを出す
				if( i != MAX_L )
				{
					// レーザーの位置を設定
					Ls[ i ].x = Hx + 16 ;
					Ls[ i ].y = Hy + 16 ;

					// レーザーの速度を設定
					Ls[ i ].sx = 0 ;
					Ls[ i ].sy = 0 ;

					// 角度をセット
					Ls[ i ].Angle = PI / 2 ;

					// 追尾カウンタをセット
					Ls[ i ].Counter = 0 ;

					// レーザーデータを使用中にセット
					Ls[ i ].ValidFlag = 1 ;
				}

				// 発射間隔カウンタ値をセット
				Hsc = 30 ;
			}
		}

		// 描画処理
		{
			int j , DeleteNum, Bx[2], By[2], Nx[2], Ny[2] ;

			// 画面の初期化
			ClearDrawScreen() ;

			// 加算ブレンドにセット
			SetDrawBlendMode( DX_BLENDMODE_ADD, 255 ) ;

			// ラインの描画
			for( j = 0 ; j < MAX_L ; j ++ )
			{
				DeleteNum = 0 ;

				// 最初の座標をセット
				Bx[0] = (int)( cos( PI / 2 + Ls[ j ].Line[ 0 ].Angle ) * 8 + Ls[ j ].Line[ 0 ].x ) ;
				By[0] = (int)( sin( PI / 2 + Ls[ j ].Line[ 0 ].Angle ) * 8 + Ls[ j ].Line[ 0 ].y ) ;

				Bx[1] = (int)( cos( -PI / 2 + Ls[ j ].Line[ 0 ].Angle ) * 8 + Ls[ j ].Line[ 0 ].x ) ;
				By[1] = (int)( sin( -PI / 2 + Ls[ j ].Line[ 0 ].Angle ) * 8 + Ls[ j ].Line[ 0 ].y ) ;

				for( i = 0 ; i < Ls[ j ].LineNum - DeleteNum ; i ++ )
				{
					// 座標の算出
					Nx[0] = (int)( cos( PI / 2 + Ls[ j ].Line[ i ].Angle ) * 8 + Ls[ j ].Line[ i ].x ) ;
					Ny[0] = (int)( sin( PI / 2 + Ls[ j ].Line[ i ].Angle ) * 8 + Ls[ j ].Line[ i ].y ) ;

					Nx[1] = (int)( cos( -PI / 2 + Ls[ j ].Line[ i ].Angle ) * 8 + Ls[ j ].Line[ i ].x ) ;
					Ny[1] = (int)( sin( -PI / 2 + Ls[ j ].Line[ i ].Angle ) * 8 + Ls[ j ].Line[ i ].y ) ;

					// ラインの描画
					DrawModiGraph( Nx[0], Ny[0], Bx[0], By[0], 
									Bx[1], By[1], Nx[1], Ny[1],  Lg2, TRUE ) ;

					// もしカウンタが規定値に達していたら普段と１８０度回転したグラフィックを
					// 描画し、その後ラインデータを無効にする
					if( Ls[ j ].Line[ i ].Counter == 64 )
					{
						// 削除するデータの数を一つ増やす
						DeleteNum ++ ;

						// データを詰める
						MoveMemory( &Ls[ j ].Line[ 0 ] , &Ls[ j ].Line[ 1 ] ,
										sizeof( LINE ) * ( Ls[ j ].LineNum - DeleteNum ) ) ;

						// 詰めたので次のデータが今の配列番号 i と同じなる
						// ので、の処理
						i -- ;

					}
					else
					{
						// カウンタを加算する
						Ls[ j ].Line[ i ].Counter ++  ;
					}

					// 座標の保存
					Bx[0] = Nx[0] ;
					By[0] = Ny[0] ;
					Bx[1] = Nx[1] ;
					By[1] = Ny[1] ;
				}
				Ls[ j ].LineNum -= DeleteNum ;
			}

			// ブレンド無しにセット
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 ) ;

			// 描画輝度をセット
			SetDrawBright( 255 , 255 , 255 ) ; 

			// プレーヤーの描画
			DrawBox( Px , Py , Px + 32 , Py + 32 , GetColor( 255 , 255 , 255 ) , TRUE ) ;

			// 砲台の描画
			DrawBox( Hx - 8 , Hy - 8 , Hx + 8 , Hy + 8 , GetColor( 255 , 255 , 0 ) , TRUE ) ;

			// 裏画面の内容を表画面に反映
			ScreenFlip() ;

			// 時間待ち
			while( GetNowHiPerformanceCount() < Time ){}
			Time += 1000000 / 60 ;
		}
	}

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;				// ソフトの終了
}
