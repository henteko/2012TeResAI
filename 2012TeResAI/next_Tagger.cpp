/*ƒƒ“ƒo•Ï”act‚ğ‘‚«Š·‚¦‚Ä‹S‚ÌŸ‚Ìs“®‚ğŒˆ‚ß‚éŠÖ”‚Å‚·B*/
#include "Data.h"


Action next_Tagger(Tagger tagger,int Stage[WIDTH][HEIGHT]){
	int r;
	int cx=tagger.x,cy=tagger.y;
	
		r=GetRand(4);
		switch(r%4){
		case 0:
			if(Stage[cx][cy-1]!=1)
				return N;
			break;
		case 1:
			if(Stage[cx+1][cy]!=1)
				return E;
			break;
		case 2:
			if(Stage[cx][cy+1]!=1)
				return S;
			break;
		case 3:
			if(Stage[cx-1][cy]!=1)
				return W;
			break;
		
	}
}