/*AIを初期化する関数です。*/

#include "Data.h"

void init_Ai(AI_T *ai,int Stage[WIDTH][HEIGHT]){
	while(1){//AIが壁に重ならないように配置
		ai->x=GetRand(WIDTH);
		ai->y=GetRand(HEIGHT);
		if(Stage[ai->x][ai->y]==0)break;
	}
	ai->act=STOP;
	ai->step=0;
	ai->life=1;
	//AIの視界を確保
	for(int i=0;i<2*VISIBLE+1;i++){
		for(int j=0;j<2*VISIBLE+1;j++){
			if(ai->x+i-VISIBLE<0 || (ai->x)+i-VISIBLE>=WIDTH || (ai->y)+i-VISIBLE<0 || (ai->y)+i-VISIBLE>=HEIGHT){
				ai->view[i][j]=1;
			}else{
				ai->view[i][j]=Stage[(ai->x)+i-VISIBLE][(ai->y)+i-VISIBLE];
			}
		}
	}
	Stage[ai->x][ai->y]=2;
}