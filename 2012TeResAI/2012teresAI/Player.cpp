/**********************************************************
	AIのサンプル
**********************************************************/

#include "Data.h"

/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void PlayerInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\player.png");  //画像の設定
	strcpy_s(myAi.name, "Player");  //自分のAIの名前設定
}


/**********************************************************
	AIの行動を返す関数
**********************************************************/
Action Player(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	if(CheckHitKey(KEY_INPUT_UP)==1)return N;
	if(CheckHitKey(KEY_INPUT_RIGHT)==1)return E;
	if(CheckHitKey(KEY_INPUT_DOWN)==1)return S;
	if(CheckHitKey(KEY_INPUT_LEFT)==1)return W;
	return STOP;
}
