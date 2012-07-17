#include "Data.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	ChangeWindowMode(TRUE);
	SetDrawScreen( DX_SCREEN_BACK);
	if(DxLib_Init()==-1)		// ＤＸライブラリ初期化処理
	{
		return -1 ;			// エラーが起きたら直ちに終了
	}

	Mode gamemode=OPENING;
	AI_T ai[AI_NUM];
	int death[AI_NUM]={0};
	Tagger tagger;
	int STAGE[WIDTH][HEIGHT]={0};
	int round=0;
	int step=0;

	while(ProcessMessage()==0){
		switch(gamemode){
		case OPENING:
			intro();
			gamemode=SETTING;
			break;
		case SETTING:
			
			make_Stage(STAGE);//マップ構成
			init_Tagger(&tagger,STAGE);//鬼の初期化
			for(int i=0;i<AI_NUM;i++){//AIの初期化
				init_Ai(&ai[i],STAGE);
			}
			
			round++;
			gamemode=RUNNING;
			break;
		case RUNNING:
			/*
			if(tagger.step==20){
				tagger.act=next_Tagger(tagger);
				tagger.step=0;
			}

			for(int i=0;i<AI_NUM;i++){
				if(ai[i].step==20){
					setview_Ai(&ai[i]);
					ai[i].act=next_Ai(ai[i]);
					ai[i].step=0;
				}
			}

			update_Tagger(&tagger);
			for(int i=0;i<AI_NUM;i++){
				update_Ai(&ai[i]);
			}
			*/

			
			ClearDrawScreen();
			draw(STAGE,ai[],tagger);
			
			
			/*捕sh・・捕獲判定しーの
			for(int i=0;AI_NUM;i++){
				if(Death_Ai(ai[i],tagger)){
					death[i]++;
					if(round>=ROUND_MAX){
						gamemode=ENDING;
					}
					else{
						gamemode=SETTING;
					}
					break;
				}
			}
			*/
			break;
		case ENDING:
			//result();
			break;
		default:
			break;
		}
		if(CheckHitKey(KEY_INPUT_ESCAPE))break;
		ScreenFlip();
	}

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;				// ソフトの終了 
}