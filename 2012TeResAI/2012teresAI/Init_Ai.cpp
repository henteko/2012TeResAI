/*AIを初期化する関数です。*/

#include "Data.h"


//8/19追加tao31鬼から一定範囲内なら繰り返し
int distans_tag_ai(int x, int y){

	if(WIDTH/4<x&&x<(WIDTH - WIDTH/4)){
		if(HEIGHT/4<y&&y<(HEIGHT - HEIGHT/4)){
			return 0;
		}
	}
	else return 1;

}

void init_Ai(AI_T *ai,int Stage[WIDTH][HEIGHT]){

	AI_T *init_ai = ai;

	// AIを変更するときにいじる場所

	//	テンプレ
	//	extern void !初期化関数名!(AI_T &myAi);
	//	!初期化関数名!(ai[!aiの番号!]);
	//	extern Action !移動関数名!(int view[2*VISIBLE+1][2*VISIBLE+1]);
	//	init_ai->moveFunc = !移動関数名!;
	//!!![重要]!!!!一番初め(aiSample)以外、最初にinit_ai++;を書く!!!!!!!!!!
	//関数名などが被ると駄目です　名前は早い者勝ちで(基本自分のハンドルネーム)

	// AI0 (aiSample)
	extern void aiSampleInit(AI_T &myAi);
	aiSampleInit(*init_ai);
	extern Action aiSample(int view[2*VISIBLE+1][2*VISIBLE+1]);
	init_ai->moveFunc = aiSample;
	// AI0

	// AI1 (aiTest)
	init_ai++;
	extern void aiTestInit(AI_T &myAi);
	aiTestInit(*init_ai);
	extern Action aiTest(int view[2*VISIBLE+1][2*VISIBLE+1]);
	init_ai->moveFunc = aiTest;
	// AI1



	//位置などの決定
	for(int i=0;i<AI_NUM;i++,ai++){//AIの初期化

		do{//AIが壁に重ならないように配置
			ai->x=GetRand(WIDTH);
			ai->y=GetRand(HEIGHT);
		}while(Stage[ai->x][ai->y]==1 || ai->x<0 || ai->x>=WIDTH || ai->y<0 || ai->y>=HEIGHT || !distans_tag_ai(ai->x,ai->y));//7/27 zero:画面外に出現した場合も考慮

		ai->s_x=(ai->x+0.5)*BOX;
		ai->s_y=(ai->y+0.5)*BOX;
		ai->act=STOP;
		ai->step=0;
		ai->life=1;


		Stage[ai->x][ai->y]=2;
	}
}