// アクションサンプルプログラム
//
// Create 10/12/03
//
// 　２Ｄアクションの基本、マップとの当たり判定です。
// 　画面外に出たら見えないところで永遠に落ちてゆきますのでご注意ください。(汗)
// 　実際はキャラクタが真四角ということはないので、いろいろと改良を加える
// 必要があります。

#include "DxLib.h"

#define SCREEN_WIDTH     (640)                          // 画面の横幅
#define SCREEN_HEIGHT    (480)                          // 画面の縦幅
#define CHIP_SIZE        (32)                           // 一つのチップのサイズ
#define MAP_WIDTH        (SCREEN_WIDTH / CHIP_SIZE)     // マップの横幅
#define MAP_HEIGHT       (SCREEN_HEIGHT / CHIP_SIZE)    // マップの縦幅

#define G                (0.3F)                         // キャラに掛かる重力加速度
#define JUMP_POWER       (9.0F)                         // キャラのジャンプ力
#define SPEED            (5.0F)                         // キャラの移動スピード
#define CHAR_SIZE        (30)                           // プレイヤーのサイズ

// マップデータ
char MapData[MAP_HEIGHT][MAP_WIDTH] =
{
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
    1,0,0,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,0,1,0,1,

    1,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 1,0,0,0,1,

    1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 1,0,0,0,1,
    1,0,0,0,0, 1,1,1,1,1, 0,0,0,1,1, 1,0,0,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,0,0,0,1,
    1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,
    1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
} ;

float PlX, PlY ;            // プレイヤーの座標(中心座標)
float PlDownSp ;            // プレイヤーの落下速度
char PlJumpFlag ;           // プレイヤーがジャンプ中か、のフラグ

int Input, EdgeInput ;      // 入力状態
int FrameStartTime ;        // ６０ＦＰＳ固定用、時間保存用変数



// マップチップの値を取得する関数
int GetChipParam( float X, float Y ) ;

// キャラクタをマップとの当たり判定を考慮しながら移動する関数
int CharMove( float *X, float *Y, float *DownSP,
                    float MoveX, float MoveY, float Size, char *JumpFlag ) ;

// アクションサンプルプログラムメイン関数
int ActMain( void ) ;

// マップとの当たり判定( 戻り値 0:当たらなかった  1:左辺に当たった  2:右辺に当たった
//                                                3:上辺に当たった  4:下辺に当たった
// 注意：MoveX と MoveY 、どっちか片方が０じゃないとまともに動作しません(爆)
int MapHitCheck( float X, float Y, float *MoveX, float *MoveY ) ;




// WinMain関数
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow )
{
    ChangeWindowMode( TRUE ) ;

    // ＤＸライブラリの初期化
    if( DxLib_Init() == -1 ) return -1 ;

    // アクションゲームのメイン関数を呼ぶ
    ActMain() ;

    // ＤＸライブラリの後始末
    DxLib_End() ;

    // 終了
    return 0 ;
}

// アクションサンプルプログラムメイン
int ActMain( void )
{
    // 描画先を裏画面にセット
    SetDrawScreen( DX_SCREEN_BACK ) ;

    // 垂直同期信号を待たない
    SetWaitVSyncFlag( FALSE ) ;

    // プレイヤーの座標を初期化
    PlX = 320.0F ;        PlY = 240.0F ;

    // プレイヤーの落下速度を初期化
    PlDownSp = 0.0F ;

    // ジャンプ中フラグを倒す
    PlJumpFlag = FALSE ;

    // 入力状態の初期化
    Input = 0 ;
    EdgeInput = 0 ;

    // ６０ＦＰＳ固定用、時間保存用変数を現在のカウント値にセット
    FrameStartTime = GetNowCount() ;

    // メインループ開始、ＥＳＣキーで外に出る
    while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
    {
        // 画面のクリア
        ClearDrawScreen() ;

        // １/６０秒立つまで待つ
        while( GetNowCount() - FrameStartTime < 1000 / 60 ){}

        // 現在のカウント値を保存
        FrameStartTime = GetNowCount() ;

        // 入力状態を更新
        {
            int i ;

            // パッド１とキーボードから入力を得る
            i = GetJoypadInputState( DX_INPUT_KEY_PAD1 ) ;

            // エッジを取った入力をセット
            EdgeInput = i & ~Input ;

            // 入力状態の保存
            Input = i ;
        }

        // プレイヤーの移動処理
        {
            float MoveX, MoveY ;

            // 移動量の初期化
            MoveX = 0.0F ;
            MoveY = 0.0F ;

            // 左右の移動を見る
            if( ( Input & PAD_INPUT_LEFT ) != 0 ) MoveX -= SPEED ;
            if( ( Input & PAD_INPUT_RIGHT ) != 0 ) MoveX += SPEED ;

            // 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
            if( PlJumpFlag == FALSE && ( EdgeInput & PAD_INPUT_A ) != 0 )
            {
                PlDownSp = -JUMP_POWER ;
                PlJumpFlag = TRUE ;
            }

            // 落下処理
            PlDownSp += G ;

            // 落下速度を移動量に加える
            MoveY = PlDownSp ;

            // 移動量に基づいてキャラクタの座標を移動
            CharMove( &PlX, &PlY, &PlDownSp, MoveX, MoveY, CHAR_SIZE, &PlJumpFlag ) ;
        }

        // マップの描画
        {
            int i, j ;

            for( i = 0 ; i < MAP_HEIGHT ; i ++ )
            {
                for( j = 0 ; j < MAP_WIDTH ; j ++ )
                {
                    // １は当たり判定チップを表しているので１のところだけ描画
                    if( MapData[i][j] == 1 )
                    {
                        DrawBox( j * CHIP_SIZE,                i * CHIP_SIZE,
                                 j * CHIP_SIZE + CHIP_SIZE,    i * CHIP_SIZE + CHIP_SIZE,
                                 GetColor( 255, 255, 255 ), TRUE ) ;
                    }
                }
            }
        }

        // キャラクタの描画
        DrawBox( ( int )( PlX - CHAR_SIZE * 0.5F ),     ( int )( PlY - CHAR_SIZE * 0.5F ),
                 ( int )( PlX + CHAR_SIZE * 0.5F ) + 1, ( int )( PlY + CHAR_SIZE * 0.5F ) + 1,
                    GetColor( 255, 0, 0 ), TRUE ) ;

        // 画面の更新
        ScreenFlip() ;
    }

    // 終了
    return 0 ;
}

// キャラクタをマップとの当たり判定を考慮しながら移動する
int CharMove( float *X, float *Y, float *DownSP,
                    float MoveX, float MoveY, float Size, char *JumpFlag )
{
    float Dummy = 0.0F ;
    float hsize ;

    // キャラクタの左上、右上、左下、右下部分が当たり判定のある
    // マップに衝突しているか調べ、衝突していたら補正する

    // 半分のサイズを算出
    hsize = Size * 0.5F ;

    // 先ず上下移動成分だけでチェック
    {
        // 左下のチェック、もしブロックの上辺に着いていたら落下を止める
        if( MapHitCheck( *X - hsize, *Y + hsize, &Dummy, &MoveY ) == 3 ) *DownSP = 0.0F ;

        // 右下のチェック、もしブロックの上辺に着いていたら落下を止める
        if( MapHitCheck( *X + hsize, *Y + hsize, &Dummy, &MoveY ) == 3 ) *DownSP = 0.0F ;

        // 左上のチェック、もしブロックの下辺に当たっていたら落下させる
        if( MapHitCheck( *X - hsize, *Y - hsize, &Dummy, &MoveY ) == 4 ) *DownSP *= -1.0F ;

        // 右上のチェック、もしブロックの下辺に当たっていたら落下させる
        if( MapHitCheck( *X + hsize, *Y - hsize, &Dummy, &MoveY ) == 4 ) *DownSP *= -1.0F ;

        // 上下移動成分を加算
        *Y += MoveY ;
    }

    // 後に左右移動成分だけでチェック
    {
        // 左下のチェック
        MapHitCheck( *X - hsize, *Y + hsize, &MoveX, &Dummy ) ;

        // 右下のチェック
        MapHitCheck( *X + hsize, *Y + hsize, &MoveX, &Dummy ) ;

        // 左上のチェック
        MapHitCheck( *X - hsize, *Y - hsize, &MoveX, &Dummy ) ;

        // 右上のチェック
        MapHitCheck( *X + hsize, *Y - hsize, &MoveX, &Dummy ) ;

        // 左右移動成分を加算
        *X += MoveX ;
    }

    // 接地判定
    {
        // キャラクタの左下と右下の下に地面があるか調べる
        if( GetChipParam( *X - Size * 0.5F, *Y + Size * 0.5F + 1.0F ) == 0 && 
            GetChipParam( *X + Size * 0.5F, *Y + Size * 0.5F + 1.0F ) == 0 )
        {
            // 足場が無かったらジャンプ中にする
            *JumpFlag = TRUE ;
        }
        else
        {
            // 足場が在ったら接地中にする
            *JumpFlag = FALSE ;
        }
    }

    // 終了
    return 0 ;
}


// マップとの当たり判定( 戻り値 0:当たらなかった  1:左辺に当たった  2:右辺に当たった
//                                                3:上辺に当たった  4:下辺に当たった
// 注意：MoveX と MoveY 、どっちか片方が０じゃないとまともに動作しません(爆)
int MapHitCheck( float X, float Y,
                    float *MoveX, float *MoveY )
{
    float afX, afY ;

    // 移動量を足す
    afX = X + *MoveX ;
    afY = Y + *MoveY ;

    // 当たり判定のあるブロックに当たっているかチェック
    if( GetChipParam( afX, afY ) == 1 )
    {
        float blx, bty, brx, bby ;

        // 当たっていたら壁から離す処理を行う

        // ブロックの上下左右の座標を算出
        blx = (float)( (int)afX / CHIP_SIZE ) * CHIP_SIZE ;        // 左辺の X 座標
        brx = (float)( (int)afX / CHIP_SIZE + 1 ) * CHIP_SIZE ;    // 右辺の X 座標

        bty = (float)( (int)afY / CHIP_SIZE ) * CHIP_SIZE ;        // 上辺の Y 座標
        bby = (float)( (int)afY / CHIP_SIZE + 1 ) * CHIP_SIZE ;    // 下辺の Y 座標

        // 上辺に当たっていた場合
        if( *MoveY > 0.0F )
        {
            // 移動量を補正する
            *MoveY = bty - Y - 1.0F  ;

            // 上辺に当たったと返す
            return 3 ;
        }

        // 下辺に当たっていた場合
        if( *MoveY < 0.0F )
        {
            // 移動量を補正する
            *MoveY = bby - Y + 1.0F ;

            // 下辺に当たったと返す
            return 4 ;
        }

        // 左辺に当たっていた場合
        if( *MoveX > 0.0F )
        {
            // 移動量を補正する
            *MoveX = blx - X - 1.0F ;

            // 左辺に当たったと返す
            return 1 ;
        }

        // 右辺に当たっていた場合
        if( *MoveX < 0.0F )
        {
            // 移動量を補正する
            *MoveX = brx - X + 1.0F ;

            // 右辺に当たったと返す
            return 2 ;
        }

        // ここに来たら適当な値を返す
        return 4 ;
    }

    // どこにも当たらなかったと返す
    return 0 ;
}

// マップチップの値を取得する関数
int GetChipParam( float X, float Y )
{
    int x, y ;

    // 整数値へ変換
    x = (int)X / CHIP_SIZE ;
    y = (int)Y / CHIP_SIZE ;

    // マップからはみ出ていたら 0 を返す
    if( x >= MAP_WIDTH || y >= MAP_HEIGHT || x < 0 || y < 0 ) return 0 ;

    // 指定の座標に該当するマップの情報を返す
    return MapData[ y ][ x ] ;
}



