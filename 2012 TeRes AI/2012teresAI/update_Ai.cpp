//‚Pƒ}ƒX‚Q‚O•à(ƒhƒbƒg)[„‚O‚ÉXV
#include "Data.h"

void update_Ai(AI_T *ai,int Stage[WIDTH][HEIGHT]){

	ai->step+=2;
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
	//•Ç‚ÉÕ“Ë
	if(Stage[ai->x][ai->y]==1){
		switch(ai->act){//7/27 zero:Õ“ËŒã‚ÌÀ•W‚ğis•ûŒü‚ÅŒˆ’è‚·‚é‚æ‚¤‚ÉC³
		case N:
			ai->y++;
			break;
		case E:
			ai->x--;
			break;
		case S:
			ai->y--;
			break;
		case W:
			ai->x++;
			break;
		case STOP:
			ai->x=GetRand(WIDTH);
			ai->y=GetRand(HEIGHT);
			break;
		}
		ai->act=STOP;
		ai->s_x=(ai->x+0.5)*BOX;
		ai->s_y=(ai->y+0.5)*BOX;
	}

	if(ai->step==BOX){
		ai->act=STOP;
		ai->step=0;
	}

}