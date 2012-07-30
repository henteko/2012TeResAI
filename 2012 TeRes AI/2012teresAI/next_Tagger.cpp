/*メンバ変数actを書き換えて鬼の次の行動を決める関数です。*/
#include "Data.h"
#include "cmath"



Action next_Tagger(Tagger tagger,int Stage[WIDTH][HEIGHT],AI_T ai[]){
	int r;
	int cx=tagger.x,cy=tagger.y;
	int target_x,target_y,target_d,dist,dx,dy;

	double disx[AI_NUM],disy[AI_NUM];//鬼と各AIの各x,y成分の距離
	double distance[AI_NUM];//鬼と各AIの距離

	//キーが押されているかどうか
	char Buf[ 256 ] ;
	GetHitKeyStateAll( Buf ) ;

	if( Buf[ KEY_INPUT_A ] == 0 ){//Aが押されていない
		//7/27 zero :鬼に追跡プログラム設置
		/*
		target_d=WIDTH+HEIGHT;
		for(int i=0;i<WIDTH;i++){
			for(int j=0;j<HEIGHT;j++){
				if(Stage[i][j]==2){
					
					dx=cx-i;
					if(dx<0)dx*=-1;
					dy=cy-j;
					if(dy<0)dy*=-1;
					dist=dx+dy;
					
					if(dist<target_d){
						target_x=i;
						target_y=j;
						target_d=dist;
					}
				}
			}
		}
		dx=cx-target_x;
		if(dx<0)dx*=-1;
		dy=cy-target_y;
		if(dy<0)dy*=-1;
		if(dx>=dy){
			if(cx<target_x){
				if(Stage[cx+1][cy]==1){
					if(cy<=target_y)return S;
					else return N;
				}else return E;
			}
			else{
				if(Stage[cx-1][cy]==1){
					if(cy<target_y)return S;
					else return N;
				}else return W;
			}
		}
		else{
			if(cy<=target_y){
				if(Stage[cx][cy+1]==1){
					if(cx<=target_x)return E;
					else return W;
				}else return S;
			}
			else{
				if(Stage[cx][cy-1]==1){
					if(cx<target_x)return E;
					else return W;
				}else return N;
			}
		}*/ //matuさんの鬼AI終わり

/*****************************************************************
		toshiも鬼の追跡AIつくってみた。一番近くのAIを捕まえに行くよ。
		壁のことは考えてないから、あほの子だよ
******************************************************************/

		for(int i=0;i<AI_NUM;i++){
			//鬼と各AIの距離を計測
			disx[i]=abs(tagger.x-ai[i].x);
			disy[i]=abs(tagger.y-ai[i].y);
			distance[i] = sqrt((disx[i]*disx[i])+(disy[i]*disy[i]));//そうだねメネラウスだね
		}

		int min_d=0;//鬼とAIの最少の距離の配列の要素
		for(int i=1; i<AI_NUM; i++)//最短距離の要素を探索 
			if( distance[i] < distance[min_d] ) min_d = i;

		//鬼とAIのx,y成分のそれぞれの差の正負によって行動決定
		if((cx-ai[min_d].x)>0) {
			if(Stage[cx-1][cy]!=1)
				return W;
		}
		else if((cx-ai[min_d].x)<0){
			if(Stage[cx+1][cy]!=1)
				return E;
		}
		if((cy-ai[min_d].y)<0){
			if(Stage[cx][cy+1]!=1)
				return S;
		}
		if((cy-ai[min_d].y)>0){
			if(Stage[cx][cy-1]!=1)
				return N;
		}
		else return STOP;

/***********************************************************
　　toshiの鬼AI終わり
**********************************************************/

		/*
		r=GetRand(4);
		switch(r%4){
		case 0:
			if(Stage[cx][cy-1]!=1)
				return N;
			break;
		case 1:
			if(Stage[cx+1][cy]!=1)
				return E;
			break;
		case 2:
			if(Stage[cx][cy+1]!=1)
				return S;
			break;
		case 3:
			if(Stage[cx-1][cy]!=1)
				return W;
			break;
		}
		*/
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