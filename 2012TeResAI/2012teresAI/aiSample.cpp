/**********************************************************
	AIのサンプル
**********************************************************/

#include "Data.h"

/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void aiSampleInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\blue.png");  //画像の設定
	strcpy_s(myAi.name, "aiSample");  //自分のAIの名前設定
}


/**********************************************************
	AIの行動を返す関数
**********************************************************/
Action aiSample(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	int r;
	int cx=VISIBLE,cy=VISIBLE;
	int danger=0;
	double direct;
	double  PI=3.141592;
	int tx,ty;
	static Action go=N;
	static Action back;

	for(int i=0;i<2*VISIBLE+1;i++){
		for(int j=0;j<2*VISIBLE+1;j++){
			if(view[i][j]==3){
				tx=i;
				ty=j;
				danger=1;
				int dx=cx-i;
				int dy=cy-j;
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
		if(go==N){
			if(view[cx][cy-1]!=1){
				go=N;
			}else{
				if(view[cx+1][cy]!=1){
					go=E;
				}else if(view[cx-1][cy]!=1){
					go=W;
				}else{
					go=S;
				}
			}
		}else if(go==E){
			if(view[cx+1][cy]!=1){
				go=E;
			}else{
				if(view[cx][cy+1]!=1){
					go=S;
				}else if(view[cx][cy-1]!=1){
					go=N;
				}else{
					go=W;
				}
			}
		}else if(go==S){
			if(view[cx][cy+1]!=1){
				go=S;
			}else{
				if(view[cx-1][cy]!=1){
					go=W;
				}else if(view[cx+1][cy]!=1){
					go=E;
				}else{
					go=N;
				}
			}
		}else if(go==W){
			if(view[cx-1][cy]!=1){
				go=W;
			}else{
				if(view[cx][cy-1]!=1){
					go=N;
				}else if(view[cx][cy+1]!=1){
					go=S;
				}else{
					go=E;
				}
			}
		}
		return go;
	}else{
		if(direct<-45){
			if(view[cx][cy-1]!=1)
				return N;
			else{
				if(view[cx-1][cy]!=1){
					return W;
				}
				else if(view[cx+1][cy]!=1){
					return E;
				}
			}
		}
		else if(direct<45){
			if(view[cx-1][cy]!=1)
				return E;
			else{
				if(view[cx][cy-1]!=1){
					return N;
				}
				else if(view[cx][cy+1]!=1){
					return S;
				}
			}
		}
		else if(direct<135){
			if(view[cx][cy+1]!=1)
				return S;
			else{
				if(view[cx+1][cy]!=1){
					return E;
				}
				else if(view[cx-1][cy]!=1){
					return W;
				}
			}
		}
		else if(direct<225){
			if(view[cx-1][cy]!=1)
				return W;
			else{
				if(view[cx][cy+1]!=1){
					return S;
				}
				else if(view[cx][cy-1]!=1){
					return N;
				}
			}
		}
		else{
			if(view[cx][cy-1]!=1)
				return N;
			else{
				if(view[cx-1][cy]!=1){
					return W;
				}
				else if(view[cx+1][cy]!=1){
					return E;
				}
			}
		}
		return STOP;
	}
}