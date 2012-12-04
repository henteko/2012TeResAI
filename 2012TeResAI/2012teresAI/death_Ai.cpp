//AI‚ª•ß‚Ü‚Á‚½‚©‚Ç‚¤‚©‚Ì”»’è‚ð‚µ‚Ü‚·
#include "Data.h"

int death_Ai(AI_T ai,Tagger tagger){
	double dist,dx,dy;
	dx=ai.s_x-tagger.s_x;
	dy=ai.s_y-tagger.s_y;
	dist=sqrt(dx*dx+dy*dy);
	if(ai.x==tagger.x && ai.y==tagger.y)return 1;
	if(dist<15 && (ai.x==tagger.x || ai.y==tagger.y))return 1;
	return 0;

}