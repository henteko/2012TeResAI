/**********************************************************
	AIのサンプル
**********************************************************/

#include "Data.h"


/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void taoaiInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\blue.png");
	strcpy_s(myAi.name, "taoai");
}


/**********************************************************
	AIの行動を返す関数
**********************************************************/
#define Base 2*VISIBLE+1
Action taoai(int view[2*VISIBLE+1][2*VISIBLE+1])//8/25:zero:追記
{

	//Action act[3];

	bool danger = false;
	int area;
	int tx,ty;

	for(int i = 0;i < Base;i++){
		for(int j =0;j < Base;j++){
			if(view[i][j] == 3){
				danger = true;
				tx = i;
				ty = j;
			}
		}
	}

	if(danger){
		if(tx<VISIBLE){
			if(ty<VISIBLE){
				return S;
			}else{
				return E;
			}
		}else{
			if(ty < VISIBLE){
				return N;
			}else{
				return W;
			}
		}
	}
	return STOP;
}