/**********************************************************
	AIのサンプル
**********************************************************/

#include "Data.h"

/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void aiSampleInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\blue.png");  //画像の設定
	strcpy_s(myAi.name, "aiSample");  //自分のAIの名前設定
}


/**********************************************************
	AIの行動を返す関数
**********************************************************/
Action aiSample(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	int r;
	int cx=VISIBLE,cy=VISIBLE;

	r=GetRand(4);
	switch(r%4){
	case 0:
		if(view[cx][cy-1]!=1)
			return N;
		break;
	case 1:
		if(view[cx+1][cy]!=1)
			return E;
		break;
	case 2:
		if(view[cx][cy+1]!=1)
			return S;
		break;
	case 3:
		if(view[cx-1][cy]!=1)
			return W;
		break;
	}

}
