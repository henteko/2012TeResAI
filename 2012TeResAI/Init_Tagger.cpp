/*‹S‚ð‰Šú‰»‚·‚éŠÖ”‚Å‚·B*/
#include "Data.h"

void init_Tagger(Tagger *tagger,int Stage[WIDTH][HEIGHT]){
	tagger->x=WIDTH/2;
	tagger->y=HEIGHT/2;
	tagger->act=STOP;
	Stage[tagger->x][tagger->y]=3;
}