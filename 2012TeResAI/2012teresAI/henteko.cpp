/**********************************************************
	AIのサンプル
**********************************************************/

#include "Data.h"

/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void hentekoInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\henteko.png");  //画像の設定
	strcpy_s(myAi.name, "henteko");  //自分のAIの名前設定
}


/**********************************************************
	AIの行動を返す関数
**********************************************************/
Action henteko(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	int r;
	int danger=0;
	double direct;
	double  PI=3.141592;
	int tx,ty;

	for(int i=0;i<2*VISIBLE+1;i++){
		for(int j=0;j<2*VISIBLE+1;j++){
			if(view[i][j]==3){
				tx=i;
				ty=j;
				danger=1;
				int dx=tx-CENTER;
				int dy=ty-CENTER;
				if(dx==0){
					if(dy>=0){
						direct=90;
					}else{
						direct=-90;
					}
				}else{
					double t=dy/dx;
					if(dx>0){
						direct=atan(t)/PI*180;
					}
					else{
						direct=atan(t)/PI*180+180;
					}
				}
			}
		}
	}
	if(danger==0){	
		for(int i=0;i<2*VISIBLE+1;i++){
			for(int j=0;j<2*VISIBLE+1;j++){
				if(view[i][j]==2){
					tx=i;
					ty=j;
					int dx=tx-CENTER;
					int dy=ty-CENTER;
					if(dx==0){
						if(dy==0){
							return STOP;
						}else if(dy>=0){
							direct=-90;
						}else{
							direct=90;
						}
					}else{
						double t=dy/dx;
						if(dx>0){
							direct=atan(t)/PI*180+180;
						}
						else{
							direct=atan(t)/PI*180;
						}
					}
				}
			}
		}
		if(direct<-45 || direct>=225){
			if(view[CENTER][CENTER+1]!=1)
				return S;
			else{
				if(view[CENTER+1][CENTER]!=1){
					return E;
				}else if(view[CENTER-1][CENTER]!=1){
					return W;
				}else{
					return N;
				}
			}
		}
		else if(direct<45){
			if(view[CENTER-1][CENTER]!=1)
				return W;
			else{
				if(view[CENTER][CENTER-1]!=1){
					return N;
				}else if(view[CENTER][CENTER+1]!=1){
					return S;
				}else{
					return E;
				}
			}
		}else if(direct<135){
			if(view[CENTER][CENTER-1]!=1)
				return N;
			else{
				if(view[CENTER-1][CENTER]!=1){
					return W;
				}else if(view[CENTER+1][CENTER]!=1){
					return E;
				}else{
					return S;
				}
			}
		}
		else if(direct<225){
			if(view[CENTER+1][CENTER]!=1)
				return E;
			else{
				if(view[CENTER][CENTER-1]!=1){
					return N;
				}else if(view[CENTER][CENTER+1]!=1){
					return S;
				}else{
					return W;
				}
			}
		}
	}else{
		if(direct<-45 || direct>=225){
			if(view[CENTER][CENTER+1]!=1)
				return S;
			else{
				if(view[CENTER+1][CENTER]!=1){
					return E;
				}else if(view[CENTER-1][CENTER]!=1){
					return W;
				}else{
					return N;
				}
			}
		}
		else if(direct<45){
			if(view[CENTER-1][CENTER]!=1)
				return W;
			else{
				if(view[CENTER][CENTER-1]!=1){
					return N;
				}else if(view[CENTER][CENTER+1]!=1){
					return S;
				}else{
					return E;
				}
			}
		}else if(direct<135){
			if(view[CENTER][CENTER-1]!=1)
				return N;
			else{
				if(view[CENTER-1][CENTER]!=1){
					return W;
				}else if(view[CENTER+1][CENTER]!=1){
					return E;
				}else{
					return S;
				}
			}
		}
		else if(direct<225){
			if(view[CENTER+1][CENTER]!=1)
				return E;
			else{
				if(view[CENTER][CENTER-1]!=1){
					return N;
				}else if(view[CENTER][CENTER+1]!=1){
					return S;
				}else{
					return W;
				}
			}
		}
	}
	return STOP;
}