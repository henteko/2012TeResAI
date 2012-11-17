/*鬼を初期化する関数です。*/
#include "Data.h"

int init_Tagger(Tagger *tagger,int Stage[WIDTH][HEIGHT]){


	Tagger *init_tagger = tagger;
	/***************************************
	*本番で、誰が鬼をやるかランダムで決定する
	***************************************/
	int tagger_num = 0;
	do{
		tagger_num = GetRand(TAGGER_NUM); //本番の鬼の要素番号

	}while(tagger_num == TAGGER_NUM); //TAGGER_NUMの番号が選ばれないように

	// Taggerを変更するときにいじる場所

	//	テンプレ
	//	extern void !初期化関数名!(Tagger &myTagger);
	//	!初期化関数名!(tagger[!taggerの番号!]);
	//	extern Action !移動関数名!(Tagger tagger,int Stage[WIDTH][HEIGHT],AI_T ai[]);
	//	init_tagger->moveFunc = !移動関数名!;
	//!!![重要]!!!!一番初め(taggerSample)以外、最初にinit_tagger++;を書く!!!!!!!!!!
	//関数名などが被ると駄目です　名前は早い者勝ちで(基本自分のハンドルネーム)
	
	// Tagger0 (taggerSample)
	extern void taggerSampleInit(Tagger &myTagger);
	taggerSampleInit(*init_tagger);
	extern Action taggerSample(int tagger_x,int tagger_y,int Stage[WIDTH][HEIGHT]);
	init_tagger->moveFunc = taggerSample;
	// Tagger0

	init_tagger++;  //次の人用にインクリメント

	// Tagger1 (taggerTest)
	extern void taggerTestInit(Tagger &myTagger);
	taggerTestInit(*init_tagger);
	extern Action taggerTest(int tagger_x,int tagger_y,int Stage[WIDTH][HEIGHT]);
	init_tagger->moveFunc = taggerTest;
	// Tagger1

	init_tagger++;  //次の人用にインクリメント

	


	/**********************************
	*鬼の位置などを初期化する
	**********************************/
	tagger += tagger_num; //tagger_numを足すことで、鬼に指名された人のみ初期化します

	tagger->x=WIDTH/2;
	tagger->y=HEIGHT/2;
	tagger->s_x=tagger->x*BOX+BOX/2;
	tagger->s_y=tagger->y*BOX+BOX/2;
	tagger->act=STOP;
	tagger->step=0;
	Stage[tagger->x][tagger->y]=3;
	
	return tagger_num; //鬼の要素番号を返す
}