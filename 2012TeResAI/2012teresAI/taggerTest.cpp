/**********************************************************
	AIのサンプル
**********************************************************/

#include "Data.h"
/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void taggerTestInit(Tagger &myTagger)
{
	myTagger.Graph = LoadGraph("Tagger_image\\aooni.jpg");  //画像の設定
	strcpy_s(myTagger.name, "taggerTest");  //自分のAIの名前設定
}

void taggerTestSearch(int x,int y,int step,int *length,int ai_x,int ai_y,int map[WIDTH][HEIGHT],int miss);
double taggerTestdirect(int x,int y,int ax,int ay);
int count;
/**********************************************************
	AIの行動を返す関数
**********************************************************/
Action taggerTest(int tx,int ty,int Stage[WIDTH][HEIGHT],AI_T ai[])
{
	//キーが押されているかどうか
	char Buf[ 256 ] ;
	GetHitKeyStateAll( Buf ) ;

	if( Buf[ KEY_INPUT_A ] == 0 ){//Aが押されていない

		double dist,direct;
		double  PI=3.141592;
		int ai_x,ai_y;
		double target_d=80;

		int map[WIDTH][HEIGHT];


		for(int i=0;i<WIDTH;i++){
			for(int j=0;j<HEIGHT;j++){
				map[i][j]=Stage[i][j];
				if(Stage[i][j]==2){
					int dx=tx-i,dy=ty-j;
					dist=sqrt(1.0*dx*dx+dy*dy);
					if(dist<target_d){
						ai_x=i;
						ai_y=j;
						target_d=dist;
						direct=taggerTestdirect(tx,ty,i,j);
					}
				}
			}
		}
		/*
		if(direct<45){
			search[0]=E;
			search[1]=S;
			search[2]=W;
			search[3]=N;
		}else if(direct<135){
			search[0]=S;
			search[1]=W;
			search[2]=N;
			search[3]=E;
		}else if(direct<225){
			search[0]=W;
			search[1]=N;
			search[2]=E;
			search[3]=S;
		}*/
		int dx[]={0,1,0,-1},dy[]={-1,0,1,0};
		int step[4]={50,50,50,50};
		Action search[4]={N,E,S,W};
		int miss=0;
		for(int i=0;i<4;i++){
			count=0;
			taggerTestSearch(tx+dx[i],ty+dy[i],0,&step[i],ai_x,ai_y,map,miss);
		}

		int min=80;
		int choice;
		static Action go;
		static int loop=0;
		for(int i=0;i<4;i++){
			if(step[i]<min){
				min=step[i];
				if((i==0 && go==S) || (i==1 && go==W) || (i==2 && go==N) || (i==3 && go==E))loop++;
				if(loop>5){
					choice=(i+2)%4;
					loop=0;
				}else{
					choice=i;
				}
			}
		}
		go=search[choice];
		return go;
	}
	else{//Aが押されている
		if( Buf[ KEY_INPUT_UP ] == 1 || Buf[ KEY_INPUT_N ] == 1 )//Nか↑が押されている
			return N;
		else if( Buf[ KEY_INPUT_RIGHT ] == 1 || Buf[ KEY_INPUT_E ] == 1 )//Ｅか→が押されれている
			return E;
		else if( Buf[ KEY_INPUT_DOWN ] == 1 || Buf[ KEY_INPUT_S ] == 1 )//Sか↓が押されている
			return S;
		else if( Buf[ KEY_INPUT_LEFT ] == 1 || Buf[ KEY_INPUT_W ] == 1 )//Wか←が押されている
			return W;
		else 
			return STOP;//何も押されていない
	}
	return STOP;

}

double taggerTestdirect(int x,int y,int ax,int ay){
	double direct,dx,dy;
	double  PI=3.141592;
	dx=ax-x;
	dy=ay-y;
	if(dx==0){
		if(dy<0){
			direct=-90;
		}else{
			direct=90;
		}
	}
	else {
		if(dx>0){
			direct=atan(dy/dx)*180/PI;
		}else{
			direct=atan(dy/dx)*180/PI+180;
		}
	}
	return direct;
}

void taggerTestSearch(int x,int y,int step,int *length,int ai_x,int ai_y,int map[WIDTH][HEIGHT],int miss){
	if(map[x][y]==1 || CheckHitKey(KEY_INPUT_Z)==1 || step>*length || *length<50 || count>500){
		return;
	}
	count++;
	int check[WIDTH][HEIGHT];
	for(int i=0;i<WIDTH;i++){
		for(int j=0;j<HEIGHT;j++){
			check[i][j]=map[i][j];
		}
	}
	check[x][y]=1;
	step++;
	static int cr=GetColor(255,255,255);

	if(x==ai_x && y==ai_y){
		cr=GetColor(GetRand(255),GetRand(255),GetRand(255));
		*length=step;
		return;
	}
	
	if(map[x][y-1]==1 && map[x+1][y]==1 && map[x][y+1]==1 && map[x-1][y]==1){
		return;
	}

	int dx[4]={0,1,0,-1},dy[4]={-1,0,1,0};
	double direct=taggerTestdirect(x,y,ai_x,ai_y);
	if(direct<-45){
		int xx[4]={0,1,0,-1},yy[4]={-1,0,1,0};
		for(int i=0;i<4;i++){
			dx[i]=xx[i];
			dy[i]=yy[i];
		}
	}
	else if(direct<0){
		int xx[4]={1,0,0,-1},yy[4]={0,-1,1,0};
		for(int i=0;i<4;i++){
			dx[i]=xx[i];
			dy[i]=yy[i];
		}
	}
	else if(direct<45){
		int xx[4]={1,0,0,-1},yy[4]={0,1,-1,0};
		for(int i=0;i<4;i++){
			dx[i]=xx[i];
			dy[i]=yy[i];
		}
	}
	else if(direct<90){
		int xx[4]={0,1,-1,0},yy[4]={1,0,0,-1};
		for(int i=0;i<4;i++){
			dx[i]=xx[i];
			dy[i]=yy[i];
		}
	}
	else if(direct<135){
		int xx[4]={0,-1,1,0},yy[4]={1,0,0,-1};
		for(int i=0;i<4;i++){
			dx[i]=xx[i];
			dy[i]=yy[i];
		}
	}
	else if(direct<180){
		int xx[4]={-1,0,0,1},yy[4]={0,1,-1,0};
		for(int i=0;i<4;i++){
			dx[i]=xx[i];
			dy[i]=yy[i];
		}
	}
	else if(direct<225){
		int xx[4]={-1,0,0,1},yy[4]={0,-1,1,0};
		for(int i=0;i<4;i++){
			dx[i]=xx[i];
			dy[i]=yy[i];
		}
	}else{
		int xx[4]={0,-1,0,1},yy[4]={-1,0,1,0};
		for(int i=0;i<4;i++){
			dx[i]=xx[i];
			dy[i]=yy[i];
		}
	}
	
	DrawCircle(20*x+10,20*y+10,10,cr,1);
	DrawCircle(20*ai_x+10,20*ai_y+10,10,GetColor(255,0,0),1);
	DrawFormatString(20*x+5,20*y+5,GetColor(255,255,0),"%d",count);
	//WaitTimer(5);

	for(int i=0;i<4;i++){
		taggerTestSearch(x+dx[i],y+dy[i],step,length,ai_x,ai_y,check,miss);
	}
}
