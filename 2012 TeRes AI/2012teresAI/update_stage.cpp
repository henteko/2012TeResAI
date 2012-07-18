#include "Data.h"

void update_stage(int Stage[WIDTH][HEIGHT],AI_T ai[],Tagger tagger){
	for(int i=0;i<WIDTH;i++){
		for(int j=0;j<HEIGHT;j++){
			if(i==0 || j==0 || i==WIDTH-1 || j==HEIGHT-1){
				Stage[i][j]=1;
			}
			if(Stage[i][j]!=1)Stage[i][j]=0;
		}
	}
	for(int i=0;i<AI_NUM;i++){
		Stage[ai[i].x][ai[i].y]=2;
	}
	Stage[tagger.x][tagger.y]=3;
}