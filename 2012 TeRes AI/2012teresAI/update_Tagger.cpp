//‹S‚ÌXV(AI‚Æ“¯‚¶)‚½‚¾‚µ‹S‚ª‚Q”{‚Ìƒy[ƒX‚ÅXV
#include "Data.h"

void update_Tagger(Tagger *tagger,int Stage[WIDTH][HEIGHT]){

	tagger->step+=4;
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
	case STOP:
		tagger->act=STOP; //	toshi : AI‚É~‚Ü‚é‚Æ‚¢‚¤‘I‘ğˆ‚ª‚È‚©‚Á‚½‚Ì‚Å’Ç‰Á
	default:
		break;
	}
	tagger->x=tagger->s_x/BOX;
	tagger->y=tagger->s_y/BOX;
	
	if(Stage[tagger->x][tagger->y]==1){
		switch(tagger->act){//7/27 zero:Õ“ËŒã‚ÌÀ•W‚ğis•ûŒü‚ÅŒˆ’è‚·‚é‚æ‚¤‚ÉC³
		case N:
			tagger->y++;
			break;
		case E:
			tagger->x--;
			break;
		case S:
			tagger->y--;
			break;
		case W:
			tagger->x++;
			break;
		default:
			break;
		}
		tagger->act=STOP;
		tagger->s_x=(tagger->x+0.5)*BOX;
		tagger->s_y=(tagger->y+0.5)*BOX;
	}

	if(tagger->step==BOX){
		tagger->act=STOP;
		tagger->step=0;
	}

}