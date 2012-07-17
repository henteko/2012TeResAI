#include "Data.h"

void setview_Ai(AI_T *ai,int Stage[WIDTH][HEIGHT]){
	for(int i=0;i<2*VISIBLE+1;i++){
		for(int j=0;j<2*VISIBLE+1;j++){
			if(ai->x+i-VISIBLE<0 || (ai->x)+i-VISIBLE>=WIDTH || (ai->y)+j-VISIBLE<0 || (ai->y)+j-VISIBLE>=HEIGHT){
				ai->view[i][j]=1;
			}else{
				ai->view[i][j]=Stage[(ai->x)+i-VISIBLE][(ai->y)+j-VISIBLE];
			}
		}
	}
}