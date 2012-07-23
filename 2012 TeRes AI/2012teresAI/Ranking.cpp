//AIの最終結果を出力する
//7/23 tao31 新規追加
#include "Data.h"


int colswitch(int c){
	int Cr;
	switch(c){
	case 1: Cr = GetColor(238,185,15);
		break;
	case 2:Cr = GetColor(165,165,165);
		break;
	case 3: Cr = GetColor(202,176,55);
		break;
	default: Cr = GetColor(100,100,100);
		break;
	}
	return Cr;
}

void ranking(AI_T ai[],int deth[]){
	int hoge,Cr;
	int j ;
	for(int i=0;i<AI_NUM;i++){

		ai[i].life = deth[i];
		j=i;
		//main関数よりdeth[]を引数に取り、小さい順に並び替え
		while(j>0&&deth[j-1]>deth[j]){
			hoge = deth[j-1];
			deth[j-1] = deth[j];
			deth[j] = hoge;
			j--;
		}
	}
	
	for(int i = 0;i<AI_NUM;i++){

		for(int k=0;k<AI_NUM;k++){

		if(ai[k].life==deth[i]){
			Cr = colswitch(i+1);

			DrawFormatString(50,200+i*20,Cr,"%d 位 %d 回",i+1,deth[i]);

			DrawString(130,200+i*20,ai[k].name,GetColor(255,0,0));

			DrawGraph(170,200+i*20,ai[k].Graph,TRUE);
		}
		}
	}

}