/**********************************************************
	AIのサンプル
**********************************************************/

#include "Data.h"
/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void taggerTestInit(Tagger &myTagger)
{
	myTagger.Graph = LoadGraph("Tagger_image\\tagger.png");  //画像の設定
	strcpy_s(myTagger.name, "Tagger");  //自分のAIの名前設定
}

void taggerTestMark(int ax,int ay,int step,int map[WIDTH][HEIGHT]);
Action taggerTestRoute(Action lets,int x,int y,int tx,int ty,int step,int map[WIDTH][HEIGHT]);
double taggerTestdirect(int x,int y,int ax,int ay);

/**********************************************************
	AIの行動を返す関数
**********************************************************/
Action taggerTest(int tx,int ty,int Stage[WIDTH][HEIGHT])
{
	//キーが押されているかどうか
	char Buf[ 256 ] ;
	GetHitKeyStateAll( Buf ) ;

	if( Buf[ KEY_INPUT_A ] == 0 ){//Aが押されていない
		int map[WIDTH][HEIGHT];
		double dist=10000;
		int ax,ay;
		for(int i=0;i<WIDTH;i++){
			for(int j=0;j<HEIGHT;j++){
				if(Stage[i][j]==1){
					map[i][j]=-1;
				}else{
					map[i][j]=0;
				}

				if(Stage[i][j]==2){
					int dx=fabs(1.0*i-tx),dy=fabs(1.0*j-ty);
					if(dx+dy<dist){
						dist=dx+dy;
						ax=i;
						ay=j;
					}
				}
			}
		}
		map[tx][ty]=1;
		taggerTestMark(ax,ay,1,map);
		Action go=taggerTestRoute(N,ax,ay,tx,ty,map[ax][ay],map);

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

void taggerTestMark(int ax,int ay,int step,int map[WIDTH][HEIGHT]){
	for(int i=0;i<WIDTH;i++){
		for(int j=0;j<HEIGHT;j++){
			//DrawFormatString(i*20+5,j*20+5,GetColor(255,255,255),"%d",map[i][j]);
			if(map[i][j]==step){
				if(map[i][j-1]==0)map[i][j-1]=step+1;
				if(map[i+1][j]==0)map[i+1][j]=step+1;
				if(map[i][j+1]==0)map[i][j+1]=step+1;
				if(map[i-1][j]==0)map[i-1][j]=step+1;
			}
		}
	}
	if(map[ax][ay]>0 || step>50)return;
	DrawFormatString(200,240,GetColor(255,255,255),"%d,%d",ax,ay);
	//WaitTimer(1000);
	taggerTestMark(ax,ay,++step,map);
}

Action taggerTestRoute(Action lets,int x,int y,int tx,int ty,int step,int map[WIDTH][HEIGHT]){
	Action go=lets;
	if(step>1){
		if(map[x][y-1]==step-1)go=taggerTestRoute(S,x,y-1,tx,ty,step-1,map);
		else if(map[x+1][y]==step-1)go=taggerTestRoute(W,x+1,y,tx,ty,step-1,map);
		else if(map[x][y+1]==step-1)go=taggerTestRoute(N,x,y+1,tx,ty,step-1,map);
		else if(map[x-1][y]==step-1)go=taggerTestRoute(E,x-1,y,tx,ty,step-1,map);
	}
	return go;
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

