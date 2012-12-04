/**********************************************************
	zeroÇÃAI
**********************************************************/

#include "Data.h"

/**********************************************************
	èâä˙âªéûÇ…åƒÇŒÇÍÇÈä÷êî
**********************************************************/
void AI_MsZeroInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\zero.png");  //âÊëúÇÃê›íË
	strcpy_s(myAi.name, "AI_MsZero");  //é©ï™ÇÃAIÇÃñºëOê›íË
}

/**********************************************************
	AIÇÃçsìÆÇï‘Ç∑ä÷êî
**********************************************************/

void MsCheckHall(int x,int y,int steplug[2*VISIBLE+1][2*VISIBLE+1]){
	int dx[]={0,1,0,-1},dy[]={-1,0,1,0};
	int wall=0;
	for(int i=0;i<4;i++){
		int hx=x+dx[i],hy=y+dy[i];
		if(steplug[hx][hy]==-1000)wall++;
	}
	if(wall==3){
		steplug[x][y]=-1000;
		for(int i=0;i<4;i++){
			int hx=x+dx[i],hy=y+dy[i];
			if(steplug[hx][hy]!=-1000)MsCheckHall(hx,hy,steplug);
		}	
	}
}

int MsCheckWall(int x,int y,int steplug[2*VISIBLE+1][2*VISIBLE+1]){
	int ux=x,dx=x;
	while(1){
		if(steplug[ux][y]==-1000 || ux<=0 || ux>=2*VISIBLE)break;
		ux++;
	}
	while(1){
		if(steplug[dx][y]==-1000 || dx<=0 || dx>=2*VISIBLE)break;
		dx--;
	}
	int wall_x[2]={0,0};
	for(int i=dx+1;i<ux;i++){
		if(steplug[i][y-1]==-1000)wall_x[0]++;
		if(steplug[i][y+1]==-1000)wall_x[1]++;
	}
	if(wall_x[0]==ux-dx-1 || wall_x[1]==ux-dx-1)return 0;
	
	int uy=y,dy=y;
	while(1){
		if(steplug[x][uy]==-1000 || uy<=0 || uy>=2*VISIBLE)break;
		uy++;
	}
	while(1){
		if(steplug[x][dy]==-1000 || dy<=0 || dy>=2*VISIBLE)break;
		dy--;
	}
	int wall_y[2]={0,0};
	for(int i=dy+1;i<uy;i++){
		if(steplug[x-1][i]==-1000)wall_y[0]++;
		if(steplug[x+1][i]==-1000)wall_y[1]++;
	}
	if(wall_y[0]==uy-dy-1 || wall_y[1]==uy-dy-1)return 0;

	return 1;
}
Action AI_MsZero(int view[2*VISIBLE+1][2*VISIBLE+1])
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
			if(view[i][j]==3){
				tx=i;
				ty=j;
				danger=1;
				tgstep[tx][ty]=1;
			}
		}
		mystep[CENTER][CENTER]=1;
	}
	int sx=CENTER,sy=CENTER;
	
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
		//DrawString(400,200,"CHECK1",GetColor(255,255,255),0);
		//WaitTimer(1000);
		for(int i=0;i<2*VISIBLE+1;i++){
			for(int j=0;j<2*VISIBLE+1;j++){
				if(mystep[i][j]==-100)steplug[i][j]=-1000;
				else steplug[i][j]=tgstep[i][j]-mystep[i][j];
			}
		}
		
		for(int i=0;i<2*VISIBLE+1;i++){
			for(int j=0;j<2*VISIBLE+1;j++){
				if(steplug[i][j]!=-1000){
					MsCheckHall(i,j,steplug);
				}
			}
		}
		
		int max=-100;

		for(int i=0;i<2*VISIBLE+1;i++){
			for(int j=0;j<2*VISIBLE+1;j++){
				if(steplug[i][j]>max && mystep[i][j]>1){
					if(MsCheckWall(i,j,steplug)==0)continue;
					max=steplug[i][j];
					sx=i;
					sy=j;
					step=mystep[i][j];
				}
			}
		}
		/*
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