/**********************************************************
	AI‚ÌƒTƒ“ƒvƒ‹
**********************************************************/

#include "Data.h"


/**********************************************************
	‰Šú‰»‚ÉŒÄ‚Î‚ê‚éŠÖ”
**********************************************************/
void aiTestInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\red.png");
	strcpy_s(myAi.name, "aiTest");
}


/**********************************************************
	AI‚Ìs“®‚ğ•Ô‚·ŠÖ”
**********************************************************/
Action aiTest(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	int r;
	int cx=VISIBLE,cy=VISIBLE;

	r=GetRand(4);
	switch(r%4){
	case 0:
		if(view[cx][cy-1]!=1)
			return N;
		break;
	case 1:
		if(view[cx+1][cy]!=1)
			return E;
		break;
	case 2:
		if(view[cx][cy+1]!=1)
			return S;
		break;
	case 3:
		if(view[cx-1][cy]!=1)
			return W;
		break;
	}

}
