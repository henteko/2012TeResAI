/*全オブジェクトを描画する関数です。*/

#include "Data.h"

	

void draw(int stage[WIDTH][HEIGHT],AI_T ai[AI_NUM],Tagger tagger){
	int view=0;
	if(CheckHitKey(KEY_INPUT_V))view=1;// 8/19 zero:Vキーを押していると描画モードが変わります。
	//数列stageにしたがってマップの描画
	int stageGraph = LoadGraph("Stage_image\\kabe0.png"); //壁の画像読み込み
	for(int i=0;i<WIDTH;i++){
		for(int j=0;j<HEIGHT;j++){
			if(view==0)SetDrawBright(150,150,150);// 8/19 zero: 薄暗く描写するよう設定
			for(int k=0;k<AI_NUM;k++){// 8/19 zero: AIの視界のみ明るくなるように
				if(i>=ai[k].x-VISIBLE && i<=ai[k].x+VISIBLE && j>=ai[k].y-VISIBLE && j<=ai[k].y+VISIBLE){
					SetDrawBright(255,255,255);
				}
			}
			if(i==WIDTH/2 || j==HEIGHT/2){//7/27 zero: 十字回廊を描いてちょっとお洒落に
				DrawBox(20*i,20*j,20*(i+1),20*(j+1),GetColor(100,100,250),0);
			}
			if(stage[i][j]==1){//壁を仮に白い正方形としています。//土の壁にしました。
//				DrawBox(20*i,20*j,20*(i+1),20*(j+1),GetColor(255,255,255),1);//四角の描写
				DrawRotaGraph((20*i)+10,(20*j)+10,1.0,0.0,stageGraph,FALSE,FALSE);//読み込んだ壁画像表示
			}
		}
	}
	//AIの描画
	for(int i=0;i<AI_NUM;i++){
		SetDrawBright(255,255,255);
		DrawRotaGraph(ai[i].s_x,ai[i].s_y,1,0,ai[i].Graph,TRUE,FALSE);//読み込んだ画像表示
		//DrawCircle(ai[i].s_x,ai[i].s_y,5,GetColor(255,0,0),1);//点表示
		if(view==1){
			DrawBox(BOX*ai[i].x,BOX*ai[i].y,BOX*(ai[i].x+1),BOX*(ai[i].y+1),GetColor(255,0,0),0);
			DrawBox(BOX*(ai[i].x-VISIBLE),BOX*(ai[i].y-VISIBLE),BOX*(ai[i].x+VISIBLE+1),BOX*(ai[i].y+VISIBLE+1),GetColor(255,255,255),0);//AIの視界（確認用）
		}
	}
	//鬼の描画
	
	//DrawRotaGraph(tagger.s_x,tagger.s_y,1,0,tagger.Graph,TRUE,FALSE);//読み込んだ画像表示 //今は青鬼が出てきて怖いからコメントアウト
	DrawCircle(tagger.s_x,tagger.s_y,10,GetColor(0,0,255),1);
	if(view==1)DrawBox(BOX*tagger.x,BOX*tagger.y,BOX*(tagger.x+1),BOX*(tagger.y+1),GetColor(255,0,0),0);
	
	//AI名の描画

	for(int i=0;i<AI_NUM;i++){
		static int flash=0;
		int cr;
		flash++;
		cr=GetColor(255,255*(flash%3),255*(flash%4));
		DrawFormatString(ai[i].s_x,ai[i].s_y,cr,ai[i].name);
	}
	
	//マップデータ表示
	while(CheckHitKey(KEY_INPUT_P)){
		for(int i=0;i<WIDTH;i++){
			for(int j=0;j<HEIGHT;j++){
				int cr=GetColor(100,255,100);
				DrawFormatString(i*20+5,j*20+5,cr,"%d",stage[i][j]);
				DrawBox(20*i,20*j,20*(i+1),20*(j+1),cr,0);
			}
		}
		WaitTimer(100);
	}
	
	//鬼が手動か自動か表示
    char Buf[ 256 ] ;
	GetHitKeyStateAll( Buf ) ;
	if( Buf[ KEY_INPUT_A ] == 1 ) {
		const char* str = strcat(tagger.name , "鬼手動モード"); //文字列連結
		DrawString( 15, 15, str ,GetColor(255,0,255) );
	}else {
		const char* str = strcat(tagger.name , "鬼AIモード"); //文字列連結
		DrawString( 15, 15, str ,GetColor(255,0,255) );
	}
}