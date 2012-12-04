#include "Data.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	ChangeWindowMode(TRUE);
	//SetMouseDispFlag(TRUE);
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	SetScreenMemToVramFlag(FALSE);
	SetAlwaysRunFlag(TRUE);
	SetDrawScreen( DX_SCREEN_BACK);
	if(DxLib_Init()==-1)		// ＤＸライブラリ初期化処理
	{
		return -1 ;			// エラーが起きたら直ちに終了
	}

	//デバック用にコンソールを出現させる
	/*
	AllocConsole();
	freopen("CONOUT$","w",stdout);
	freopen("CONIN$","r",stdin);z
	*/

	Mode gamemode=OPENING;
	AI_T ai[AI_NUM];
	int death[AI_NUM]={0};
	Tagger tagger[TAGGER_NUM];
	int tagger_num = 0;
	int STAGE[WIDTH][HEIGHT]={0};
	int round=0;
	int start=0;
	int end=0;
	int StartTime,TimeLimit;

	for(int i=0;i<AI_NUM;i++){
		ai[i].entry=1;
	}
	ai[0].entry=0;

	while(ProcessMessage()!=-1){
		int speed=0;
		switch(gamemode){
		case OPENING:
			start=intro(ai);
			if(start==1)gamemode=SETTING;
			break;
		case SETTING:
			make_Stage(STAGE);//マップ構成
			tagger_num = init_Tagger(tagger,STAGE);//鬼の初期化 //tagger_numは鬼の要素番号

			init_Ai(ai,STAGE);

			round++;
			TimeLimit=TIME_LIMIT*60*79;//ゲーム開始時の時刻に合わせる
			gamemode=RUNNING;

			break;
		case RUNNING:
			TimeLimit-=79;
			if(TimeLimit<0)TimeLimit=0;
			if(tagger[tagger_num].step==0){
				//tagger[tagger_num].act=next_Tagger(tagger[tagger_num],STAGE,ai);
				tagger[tagger_num].act=tagger[tagger_num].moveFunc(tagger[tagger_num].x,tagger[tagger_num].y,STAGE); //AIと一緒で、moveFunc使う
			}
			for(int i=0;i<AI_NUM;i++){
				if(ai[i].step==0 && ai[i].entry==1){
					setview_Ai(&ai[i],STAGE);
					//ai[i].act=next_Ai(ai[i].view); //henteko : 下のmoveFunc()を使うためコメントアウト
					ai[i].act = ai[i].moveFunc(ai[i].view);
				}
			}
			if(TimeLimit>TIME_LIMIT*45*79)speed=0;
			else if(TimeLimit>TIME_LIMIT*30*79)speed=1;
			else if(TimeLimit>TIME_LIMIT*15*79)speed=1;
			else speed=3;
			update_Tagger(&tagger[tagger_num],STAGE,speed);
			for(int i=0;i<AI_NUM;i++){
				if(ai[i].entry==1)
					update_Ai(&ai[i],STAGE);
			}
			update_stage(STAGE,ai,tagger[tagger_num]);
			
			ClearDrawScreen();
			draw(STAGE,ai,tagger[tagger_num]);
			DrawFormatString(30,30,GetColor(0,255,255),"ROUND%d",round);
			
			DrawFormatString(500,15,GetColor(0,255,0),"TIME %d",TimeLimit);
			
			if(1){
				for(int i=0;i<AI_NUM;i++){
					if(death_Ai(ai[i],tagger[tagger_num])==1 && ai[i].entry==1){
						death[i]++;
						DrawBox(0,230,640,260,GetColor(0,0,0),1);
						DrawBox(-1,230,642,260,GetColor(255,0,0),0);
						DrawFormatString(100,240,GetColor(255,0,0),"%sがつかまりました",ai[i].name);// 8/3 zero追記:AI捕獲の宣言をまとめた。
						WaitTimer(3000);
						if(round>=ROUND_MAX){
							gamemode=ENDING;
						}
						else{
							gamemode=SETTING;
						}
						break;
					}
				}
			}
			if(TimeLimit<=0){// 8/3 zero追記:タイムアップを設定
				round--;
				DrawString(100,240,"時間切れです",GetColor(255,0,0));
				WaitTimer(3000);
				if(round>=ROUND_MAX){
					gamemode=ENDING;
				}
				else{
					gamemode=SETTING;
				}
				break;
			}
			if(CheckHitKey(KEY_INPUT_R)==1){
				gamemode=SETTING;
			}
			break;
		case ENDING:
			end=ranking(ai,death);
			break;
		default:
			break;
		}
		if(end==1 || CheckHitKey(KEY_INPUT_ESCAPE))break;
		ScreenFlip();
	}

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;				// ソフトの終了 
}