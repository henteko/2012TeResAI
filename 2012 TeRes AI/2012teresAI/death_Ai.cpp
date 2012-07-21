//AI‚ª•ß‚Ü‚Á‚½‚©‚Ç‚¤‚©‚Ì”»’è‚ð‚µ‚Ü‚·
#include "Data.h"

int death_Ai(AI_T ai,Tagger tagger){
	if(ai.x==tagger.x && ai.y==tagger.y){
		return 1;
	}else{
		return 0;
	}
}