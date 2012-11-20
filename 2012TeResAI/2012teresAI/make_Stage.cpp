/*ステージをランダム精製する関数です。*/
#include "Data.h"

int miti[WIDTH][HEIGHT]={1}; /*1で壁、0で通路（＝壁じゃないかつ、隔離されていない）*/

void notWall(int Stage2[WIDTH][HEIGHT]){ /*通路生成　効率悪いのでもっとよく出来きるなら是非やって下さい*/
	
	int i=0;
	while(i<=20){
		for(int x=0;x<WIDTH;x++){
			for(int y=0;y<HEIGHT;y++){
				if(Stage2[x][y]== 0 &&( miti[x+1][y]==0 
									||  miti[x-1][y]==0
									||  miti[x][y+1]==0
									||  miti[x][y-1]==0))
								miti[x][y] = 0;
			}
		}
		for(int x=WIDTH;x>0;x--){
			for(int y=HEIGHT;y<0;y--){
				if(Stage2[x][y]== 0 &&( miti[x+1][y]==0 
									||  miti[x-1][y]==0
									||  miti[x][y+1]==0
									||  miti[x][y-1]==0))
								miti[x][y] = 0;
			}
		}
		i++;
	}
}

void make_Stage(int Stage[WIDTH][HEIGHT]){
	for(int x=0;x<WIDTH;x++){
		for(int y=0;y<HEIGHT;y++){
			miti[x][y]=1;
			if(GetRand(3)==0){
				Stage[x][y]=1;
			}
			else{
				Stage[x][y]=0;
			}
			if(x==WIDTH/2 || y==HEIGHT/2){
				Stage[x][y]=0;
				miti[x][y]=0;
			}
			if(x==2 || y==2 || x==WIDTH-3 || y==HEIGHT-3){
				Stage[x][y]=0;
			}
			if(x==0 || y==0 || x==WIDTH-1 || y==HEIGHT-1){
				Stage[x][y]=1;
			}
		}
	}

	notWall(Stage); /*通路生成*/

	for(int x=0;x<WIDTH;x++){ /*隔離されたところを壁にする*/
		for(int y=0;y<HEIGHT;y++){
			if(Stage[x][y]==0 && miti[x][y]==1)
				Stage[x][y] = 1;
		}
	}

}