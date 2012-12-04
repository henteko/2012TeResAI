/**********************************************************
	hotaのAI
**********************************************************/

#include "Data.h"

/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void hotaInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\アルパカ.png");  //画像の設定
	strcpy_s(myAi.name, "hota");  //自分のAIの名前設定
}

/**********************************************************
	AIの行動を返す関数
**********************************************************/

Action hota(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	int danger=0;
	int tx,ty;
	int mystep[2*VISIBLE+1][2*VISIBLE+1],tgstep[2*VISIBLE+1][2*VISIBLE+1],steplug[2*VISIBLE+1][2*VISIBLE+1];
	int ai_x[AI_NUM],ai_y[AI_NUM],ai_n=0;
	for(int i=0;i<AI_NUM;i++){
		ai_x[i]=0;
		ai_y[i]=0;
	}
	int dx[]={0,1,0,-1};
	int dy[]={-1,0,1,0};
	int step;

	for(int i=0;i<2*VISIBLE+1;i++){
		for(int j=0;j<2*VISIBLE+1;j++){
			if(view[i][j]==1){
				mystep[i][j]=-100;
				tgstep[i][j]=-100;
			}else{
				mystep[i][j]=0;
				tgstep[i][j]=0;
			}
			if(view[i][j]==2 && !(i==CENTER && j==CENTER)){
				ai_x[ai_n]=i;
				ai_y[ai_n]=j;
				ai_n++;
			}
			if(view[i][j]==3){
				tx=i;
				ty=j;
				danger=1;
				tgstep[tx][ty]=1;
			}
		}
		mystep[CENTER][CENTER]=1;
	}
	int sx,sy;
	if(danger==1){
		step=1;
		while(1){
			int myex=1,tgex=1;
			for(int i=0;i<2*VISIBLE+1;i++){
				for(int j=0;j<2*VISIBLE+1;j++){
					if(mystep[i][j]==step){
						for(int k=0;k<4;k++){
							int x=i+dx[k],y=j+dy[k];
							if(x<0 || x>2*VISIBLE || y<0 || y>2*VISIBLE)continue;
							if(mystep[x][y]==0){
								mystep[x][y]=step+1;
								myex=0;
							}
						}
					}
					if(tgstep[i][j]==step){
						for(int k=0;k<4;k++){
							int x=i+dx[k],y=j+dy[k];
							if(x<0 || x>2*VISIBLE || y<0 || y>2*VISIBLE)continue;
							if(tgstep[x][y]==0){
								tgstep[x][y]=step+1;
								tgex=0;
							}
						}
					}
				}
			}
			step++;
			if(myex==1 && tgex==1)break;
		}

		for(int i=0;i<2*VISIBLE+1;i++){
			for(int j=0;j<2*VISIBLE+1;j++){
				if(mystep[i][j]==-100)steplug[i][j]=-1000;
				else steplug[i][j]=tgstep[i][j]-mystep[i][j];
			}
		}
		
		int max=-100;
		while(1){
			int check=0;
			for(int i=0;i<2*VISIBLE+1;i++){
				for(int j=0;j<2*VISIBLE+1;j++){
					if(steplug[i][j]>max && mystep[i][j]>1){
						int wall=0;
						for(int k=0;k<4;k++){
							int x=i+dx[k],y=j+dy[k];
							if(steplug[x][y]==-1000){
								wall++;
							}
						}
						if(wall>2){
							//check=0; *12/03 zeroが修正
							steplug[i][j]=-1000; 
							continue;
						}
						check=1;
						max=steplug[i][j];
						sx=i;
						sy=j;
						step=mystep[i][j];
					}
				}
			}
			if(check==1)break;
		}/*
		for(int i=0;i<2*VISIBLE+1;i++){
			for(int j=0;j<2*VISIBLE+1;j++){
				if(steplug[i][j]>=0)DrawFormatString(20*i+5,20*j+5,GetColor(255,255,255),"%d",steplug[i][j]);
			}
		}
		WaitTimer(1000);*/
	}
	Action go=STOP;
	while(step>1){
		for(int i=0;i<4;i++){
			int x=sx+dx[i],y=sy+dy[i];
			if(mystep[x][y]==step-1){
				sx=x;
				sy=y;
				if(i==0)go=S;
				if(i==1)go=W;
				if(i==2)go=N;
				if(i==3)go=E;
			}
		}
		step--;
	}
	return go;
}