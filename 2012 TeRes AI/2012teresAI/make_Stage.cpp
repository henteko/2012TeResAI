/*ステージをランダム精製する関数です。*/
#include "Data.h"

void make_Stage(int Stage[WIDTH][HEIGHT]){
	for(int x=0;x<WIDTH;x++){
		for(int y=0;y<HEIGHT;y++){
			if(GetRand(3)==0){
				Stage[x][y]=1;
			}
			else{
				Stage[x][y]=0;
			}
			if(x==WIDTH/2 || y==HEIGHT/2){
				Stage[x][y]=0;
			}
			if(x==0 || y==0 || x==WIDTH-1 || y==HEIGHT-1){
				Stage[x][y]=1;
			}
		}
	}
}