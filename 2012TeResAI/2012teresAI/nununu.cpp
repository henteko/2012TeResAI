/**********************************************************
	AIのサンプル
**********************************************************/

#include "Data.h"

/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void nununuInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\AI_nu.png");  //画像の設定
	strcpy_s(myAi.name, "nununu");  //自分のAIの名前設定
}


/**********************************************************
	AIの行動を返す関数
**********************************************************/

Action nununu(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	int r;
	int danger=0;
	double direct;
	double  PI=3.141592;
	int tx,ty;
	int Go=1;


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
	
	for(int i=0;i<2*VISIBLE+1;i++){
		for(int j=0;j<2*VISIBLE+1;j++){
			if(i!=CENTER&&j!=CENTER){
				if(view[i][j]==0){
					if(view[i+1][j]+view[i][j+1]+view[i-1][j]+view[i][j-1]==3){
						view[i][j]=1;
				}
			}
		}
	}
	}
	


	if(danger==0)
	{
		if(Go==2){
			if(view[CENTER][CENTER+1]!=1){
				Go=2;
				return S;
				
			}
			else
				 if(view[CENTER+1][CENTER]!=1){
					 Go=1;
					return E;
				}else if(view[CENTER-1][CENTER]!=1){
					Go=3;
					return W;
				}else{
					Go=4;
					return N;
				
			}
		}
		else if(Go==3){
			
			if(view[CENTER-1][CENTER]!=1){
				Go=3;
				return W;
			}
			else{
				if(view[CENTER][CENTER-1]!=1){
					Go=4;
					return N;
				}else if(view[CENTER][CENTER+1]!=1){
					Go=2;
					return S;
				}else{
					Go=1;
					return E;
				}
			}
		}else if(Go==4){
			if(view[CENTER][CENTER-1]!=1){
				Go=4;
				return N;
			}
			else{
				if(view[CENTER-1][CENTER]!=1){
					Go=3;
					return W;
				}else if(view[CENTER+1][CENTER]!=1){
					Go=1;
					return E;
				}else{
					Go=2;
					return S;
				}
			}
		}
		else if(Go==1){
			if(view[CENTER+1][CENTER]!=1){
				Go=1;
				return E;
			}
			else{
				if(view[CENTER][CENTER-1]!=1){
					Go=4;
					return N;
				}else if(view[CENTER][CENTER+1]!=1){
					Go=2;
					return S;
				}else{
					Go=3;
					return W;
				}
			}
		}
		return STOP;
		}
	
	
	else if(danger==1){

	
			r = GetRand(6);
			if(r%6==0 || r%6==1 || r%6==2 ||r%6==3 || r%6==4 || r%6==5 ){
			

		if(direct<-45 || direct>=225){
			if(view[CENTER][CENTER+1]!=1 && view[CENTER][CENTER+1]!=3){
				Go=2;
				return S;
			}
		else if(view[CENTER+1][CENTER]!=1 && view[CENTER+1][CENTER]!=3){
					 Go=1;
					return E;
				}else if(view[CENTER-1][CENTER]!=1 && view[CENTER-1][CENTER]!=3){
					Go=3;
					return W;
				}else{
					Go=4;
					return N;
				
			}
		}
		else if(direct<45){
			if(view[CENTER-1][CENTER]!=1 && view[CENTER-1][CENTER]!=3){
				Go=3;
				return W;
			}
			else{
				if(view[CENTER][CENTER-1]!=1 && view[CENTER][CENTER-1]!=3){
					Go=4;
					return N;
				}else if(view[CENTER][CENTER+1]!=1 && view[CENTER][CENTER+1]!=3){
					Go=2;
					return S;
				}else{
					Go=1;
					return E;
				}
			}
		}else if(direct<135){
			if(view[CENTER][CENTER-1]!=1 && view[CENTER][CENTER-1]!=3){
				Go=4;
				return N;
			}
			else{
				if(view[CENTER-1][CENTER]!=1 && view[CENTER-1][CENTER]!=3){
					Go=3;
					return W;
				}else if(view[CENTER+1][CENTER]!=1 && view[CENTER+1][CENTER]!=3){
					Go=1;
					return E;
				}else{
					Go=2;
					return S;
				}
			}
		}
		else if(direct<225){
			if(view[CENTER+1][CENTER]!=1 && view[CENTER+1][CENTER]!=3){
				Go=1;
				return E;
			}
			else{
				if(view[CENTER][CENTER-1]!=1 && view[CENTER][CENTER-1]!=3){
					Go=4;
					return N;
				}else if(view[CENTER][CENTER+1]!=1 && view[CENTER][CENTER+1]!=3){
					Go=2;
					return S;
				}else{
					Go=3;
					return W;
				}
			}
		}
		return STOP;
	
			}


	else  {
		int r=GetRand(4);
		switch(r%4){
		case 0:
			if(view[CENTER][CENTER-1]!=1)
			return N;
			break;
		case 1:
			if(view[CENTER+1][CENTER]!=1)
			return E;
			break;
		case 2:
			if(view[CENTER][CENTER+1]!=1)
			return S;
			break;
		case 3:
			if(view[CENTER-1][CENTER]!=1)
			return W;
			break;
	}


	}

	}
	}