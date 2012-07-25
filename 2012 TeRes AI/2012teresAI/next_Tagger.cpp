/*メンバ変数actを書き換えて鬼の次の行動を決める関数です。*/
#include "Data.h"


Action next_Tagger(Tagger tagger,int Stage[WIDTH][HEIGHT]){
	int r;
	int cx=tagger.x,cy=tagger.y;

	//キーが押されているかどうか
	char Buf[ 256 ] ;
	GetHitKeyStateAll( Buf ) ;

	if( Buf[ KEY_INPUT_A ] == 0 ){//Aが押されていない
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
	}
	else{//Aが押されている
		if( Buf[ KEY_INPUT_N ] == 1 )//Nが押されている
			return N;
		else if( Buf[ KEY_INPUT_E ] == 1 )//Ｅが押されれている
			return E;
		else if( Buf[ KEY_INPUT_S ] == 1 )//Sが押されている
			return S;
		else if( Buf[ KEY_INPUT_W ] == 1 )//Wが押されている
			return W;
		else 
			return STOP;//何も押されていない
	}
	return STOP;

}