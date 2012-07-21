//‹S‚ÌXV(AI‚Æ“¯‚¶)‚½‚¾‚µ‹S‚ª‚Q”{‚Ìƒy[ƒX‚ÅXV
#include "Data.h"

void update_Tagger(Tagger *tagger,int Stage[WIDTH][HEIGHT]){

	tagger->step+=4;
	int last_x=tagger->x,last_y=tagger->y;
	switch(tagger->act){
	case N:
		tagger->s_y-=4;
		break;
	case E:
		tagger->s_x+=4;
		break;
	case S:
		tagger->s_y+=4;
		break;
	case W:
		tagger->s_x-=4;
		break;
	default:
		break;
	}
	tagger->x=tagger->s_x/BOX;
	tagger->y=tagger->s_y/BOX;
	
	if(Stage[tagger->x][tagger->y]==1){
		tagger->act=STOP;
		tagger->x=last_x;
		tagger->y=last_y;
		tagger->s_x=(tagger->x+0.5)*BOX;
		tagger->s_y=(tagger->y+0.5)*BOX;
	}

	if(tagger->step==BOX){
		tagger->act=STOP;
		tagger->step=0;
	}

}