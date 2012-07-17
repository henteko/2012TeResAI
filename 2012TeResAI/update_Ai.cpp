#include "Data.h"

void update_Ai(AI_T *ai,int Stage[WIDTH][HEIGHT]){

	ai->step+=2;
	int last_x=ai->x,last_y=ai->y;
	switch(ai->act){
	case N:
		ai->s_y-=2;
		break;
	case E:
		ai->s_x+=2;
		break;
	case S:
		ai->s_y+=2;
		break;
	case W:
		ai->s_x-=2;
		break;
	default:
		break;
	}
	ai->x=ai->s_x/BOX;
	ai->y=ai->s_y/BOX;
	
	if(Stage[ai->x][ai->y]==1){
		ai->act=STOP;
		ai->x=last_x;
		ai->y=last_y;
		ai->s_x=(ai->x+0.5)*BOX;
		ai->s_y=(ai->y+0.5)*BOX;
	}

	if(ai->step==BOX){
		ai->act=STOP;
		ai->step=0;
	}

}