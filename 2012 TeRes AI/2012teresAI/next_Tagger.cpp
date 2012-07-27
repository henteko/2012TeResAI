/*メンバ変数actを書き換えて鬼の次の行動を決める関数です。*/
#include "Data.h"
#include "cmath"



Action next_Tagger(Tagger tagger,int Stage[WIDTH][HEIGHT]){
	int r;
	int cx=tagger.x,cy=tagger.y;
	int target_x,target_y,target_d,dist,dx,dy;
	//キーが押されているかどうか
	char Buf[ 256 ] ;
	GetHitKeyStateAll( Buf ) ;

	if( Buf[ KEY_INPUT_A ] == 0 ){//Aが押されていない
		//7/27 zero :鬼に追跡プログラム設置
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
		}

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