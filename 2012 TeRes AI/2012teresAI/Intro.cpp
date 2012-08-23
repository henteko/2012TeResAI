/*何か洒落たオープニングを*/
//21th june　tao31編集

#include "Data.h"

void intro(AI_T *ai){
	AI_T *init_ai = ai;

	int gametitle;
	gametitle = LoadGraph("media\\chofu_fes2.bmp");

	DrawGraph(0,0,gametitle,FALSE);

	ScreenFlip();

	//DrawString(200,240,"2012 Chohu Festival",GetColor(255,255,255),0);
	DrawString(240,270,"ENTRY AI",GetColor(125,255,205),0);


	// AI0 (aiSample)
	extern void aiSampleInit(AI_T &myAi);
	aiSampleInit(*init_ai);
	extern Action aiSample(int view[2*VISIBLE+1][2*VISIBLE+1]);
	init_ai->moveFunc = aiSample;
	// AI0

	// AI1 (aiTest)
	init_ai++;
	extern void aiTestInit(AI_T &myAi);
	aiTestInit(*init_ai);
	extern Action aiTest(int view[2*VISIBLE+1][2*VISIBLE+1]);
	init_ai->moveFunc = aiTest;
	// AI1


	/*//test デモ
	DrawString(200,290,"AI_name",GetColor(0,255,205),0);
	DrawCircle(300,300,5,GetColor(255,0,0),1);
	//本当は下のほうでエントリーを表示する予定AI_T ai[]*/
	for(int i= 0; i < AI_NUM ;i++, ai++){
	DrawString(240,290+(i*50),ai->name,GetColor(0,255,255),0);
	DrawGraph( 300 , 290+i*50 , ai->Graph , FALSE ) ;
	}
	
	
	
	WaitKey();

}