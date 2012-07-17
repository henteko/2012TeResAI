/*AIを初期化する関数です。*/

#include "Data.h"

void init_Ai(AI_T *ai,int Stage[WIDTH][HEIGHT]){
	do{//AIが壁に重ならないように配置
		ai->x=GetRand(WIDTH);
		ai->y=GetRand(HEIGHT);
	}while(Stage[ai->x][ai->y]==1);

	ai->s_x=(ai->x+0.5)*BOX;
	ai->s_y=(ai->y+0.5)*BOX;
	ai->act=STOP;
	ai->step=0;
	ai->life=1;
	
	Stage[ai->x][ai->y]=2;
}