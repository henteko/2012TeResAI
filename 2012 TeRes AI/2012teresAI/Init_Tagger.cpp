/*‹S‚ð‰Šú‰»‚·‚éŠÖ”‚Å‚·B*/
#include "Data.h"

void init_Tagger(Tagger *tagger,int Stage[WIDTH][HEIGHT]){
	tagger->x=(WIDTH-1)/2;
	tagger->y=(HEIGHT-1)/2;
	tagger->s_x=tagger->x*BOX+BOX/2;
	tagger->s_y=tagger->y*BOX+BOX/2;
	tagger->act=STOP;
	tagger->step=0;
	Stage[tagger->x][tagger->y]=3;
}