/**********************************************************
	久住のAI
**********************************************************/

#include "Data.h"

/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void kuzumiInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\ika.png");  //画像の設定
	strcpy_s(myAi.name, "kuzumi");  //自分のAIの名前設定
}


/**********************************************************
	AIの行動を返す関数
**********************************************************/
#define KZ_STEPMAX 100
#define KZ_TAGGER_STEP_GETA 5 // 評価の補正
#define KZ_TAGGER_POINT_GETA 10 // 評価の補正
int KZ_dequeue(stack *st);
Action kuzumi(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	// 逃げるようの変数
	static stack *st = (stack*)malloc(sizeof(stack));	// 逃走経路stack
	static int initFlag = TRUE;		// stack情報を破棄するか
	int aiX[AI_NUM], aiY[AI_NUM];	// 他のaiの座標	
	int aiCount = 0;				// 他のaiの数

	static int map[5][5] = {-1};			// 前の行動時のマップ
	int myStep[2*VISIBLE+1][2*VISIBLE+1]; // 自分中心の歩数データ
	int taggerStep[2*VISIBLE+1][2*VISIBLE+1]; // オニ中心の歩数データ

	static int crawlCount = 0; 
	/* viewの整理 */
	for(int i=0; i<2*VISIBLE+1; i++)
		for(int j=0; j<2*VISIBLE+1; j++)
			switch(view[i][j]) {
			case 1: // wall
				myStep[i][j] = taggerStep[i][j] = -1;
				break;
			case 2: // ai
				if(i != VISIBLE || j != VISIBLE) { // 自分以外なら居場所を記録
					aiX[aiCount] = i;
					aiY[aiCount] = j;
					aiCount++;
				}
				// そのまま0の処理を行う
			case 0: // way
				myStep[i][j] = taggerStep[i][j] = 0;
				break;
			case 3: // tagger
				myStep[i][j] = -1;		// 自分は壁扱いする
				taggerStep[i][j] = 1;	// オニは自分の居場所
			}
	myStep[VISIBLE][VISIBLE] = 1;		// 自分の居場所を入れる

	/* ここで、フィールド更新の判定を行う */
	for(int i=1; i<5; i++)
		for(int j=0; j<5; j++)
			if(map[i][j] != myStep[VISIBLE-2+i][VISIBLE-2+j])
				initFlag = TRUE;

	/* Stepのマッピング */
	for(int i=1; i<KZ_STEPMAX; i++)
		for(int j=0; j<2*VISIBLE+1; j++)
			for(int k=0; k<2*VISIBLE+1; k++) {
				if(myStep[j][k] == i) {
					if(j != 0 && myStep[j-1][k] == 0)
						myStep[j-1][k] = i+1;
					if(j != VISIBLE*2 && myStep[j+1][k] == 0)
						myStep[j+1][k] = i+1;
					if(k != 0 && myStep[j][k-1] == 0)
						myStep[j][k-1] = i+1;
					if(k != VISIBLE*2 && myStep[j][k+1] == 0)
						myStep[j][k+1] = i+1;
				}
				if(taggerStep[j][k] == i) {
					if(j != 0 && taggerStep[j-1][k] == 0)
						taggerStep[j-1][k] = i+1;
					if(j != VISIBLE*2 && taggerStep[j+1][k] == 0)
						taggerStep[j+1][k] = i+1;
					if(k != 0 && taggerStep[j][k-1] == 0)
						taggerStep[j][k-1] = i+1;
					if(k != VISIBLE*2 && taggerStep[j][k+1] == 0)
						taggerStep[j][k+1] = i+1;
				}
			}
	// 到達不能点を壁扱いする
	for(int i=1; i<2*VISIBLE+1; i++)
		for(int j=0; j<2*VISIBLE+1; j++) {
			if(myStep[i][j] == 0)
				myStep[i][j] = -1;
			if(taggerStep[i][j] == 0)
				taggerStep[i][j] = -1;
		}


	if(initFlag) {
		initFlag = FALSE;
		st->count = 0;
	}

	if(st->count > 0) // スタックに積まれていれば
		return pop(st);	// 中身を返す

	if(taggerStep[VISIBLE][VISIBLE] > 0) { // オニが見えて、自分が捕まる場所にいるとき
		// 逃走ポイントの決定
		int escapeX, escapeY;
		int fastPoint = -30;
		for(int i=1; i<2*VISIBLE+1; i++)
			for(int j=0; j<2*VISIBLE+1; j++) {
				if(myStep[i][j]>0) { // 自分が行ける場所について
					/* 評価ポイント１
						その場所にどちらが何歩早く着けるか
						補正１、自由度ポイント（何方向に動けるか）を加算する
						補正２、オニの歩数が一定以上かかる場所は点数を加算する
					*/
					int stepPoint = taggerStep[i][j] - myStep[i][j];
					int dirPoint = 0;
					if(i != 0 && myStep[i-1][j] == 0)			dirPoint++;
					if(i != VISIBLE*2 && myStep[i+1][j] == 0)	dirPoint++;
					if(j != 0 && myStep[i][j-1] == 0)			dirPoint++;
					if(j != VISIBLE*2 && myStep[i][j+1] == 0)	dirPoint++;
					if(dirPoint > 2)
						dirPoint *= 2;
					stepPoint += dirPoint;
					if(taggerStep[i][j] > KZ_TAGGER_STEP_GETA)
						stepPoint += KZ_TAGGER_POINT_GETA;

					if(stepPoint > fastPoint) {
						fastPoint = stepPoint;
						escapeX = i;
						escapeY = j;
					}
					/* 評価ポイント２
					評価ポイント１が同点の場合
					オニの歩数がよりかかる場所を選ぶ
				*/
					if(stepPoint == fastPoint && taggerStep[i][j] > taggerStep[escapeX][escapeY]) {
						escapeX = i;
						escapeY = j;
					}
				}
			}

		// 逃走ポイントの記憶
		for(int i=myStep[escapeX][escapeY]; 1<i; i--) { // 自分の居場所がゴールになるまで
			if(myStep[escapeX-1][escapeY] == i-1) { // その場所に西側から入れるなら
				push(st, E);	// 東方向をpushして
				escapeX--;		// 逃走ポイントを１つ西にずらす
			} else if(myStep[escapeX+1][escapeY] == i-1) { // その場所に東側から入れるなら
				push(st, W);	// 西方向をpushして
				escapeX++;		// 逃走ポイントを１つ西にずらす
			} else if(myStep[escapeX][escapeY-1] == i-1) { // その場所に北側から入れるなら
				push(st, S);	// 南方向をpushして
				escapeY--;		// 逃走ポイントを１つ北にずらす
			}  else if(myStep[escapeX][escapeY+1] == i-1) { // その場所に南側から入れるなら
				push(st, N);	// 北方向をpushして
				escapeY++;		// 逃走ポイントを１つ南にずらす
			} else {	// 変な状態に陥ったとき
				st->count = 0;				// スタックをリセットして
				return STOP;	// とりあえずあきらめるとする
			}
		}
		Action ret = pop(st);
		int mapX = VISIBLE;
		int mapY = VISIBLE;
		switch(ret) {
		case N:
			mapY--;
			break;
		case S:
			mapY++;
			break;
		case W:
			mapX--;
			break;
		case E:
			mapX++;
		}
		for(int i=0; i<5; i++)
			for(int j=0; j<5; j++) {
				map[i][j] = myStep[mapX-2+i][mapY-2+j] == -1? -1: 0;
			}
		return ret;	// 結果をpopして返す
	}
	else { // ひとまず安全なとき
		if(++crawlCount == 2) // 2ターンに１マス這いよる
		{
			crawlCount = 0;
			int targetX, targetY;
			int targetStep = KZ_STEPMAX;

			if(aiCount == 0) return STOP;
			for(int i=0; i<aiCount; i++)
				if(targetStep > myStep[aiX[i]][aiY[i]]) {
					targetX = aiX[i];
					targetY = aiY[i];
					targetStep = myStep[aiX[i]][aiY[i]];					
				}
			// 這いよる方向の決定
			for(int i=targetStep; 1<i; i--) { // 自分の居場所がゴールになるまで
				if(myStep[targetX-1][targetY] == i-1) { // その場所に西側から入れるなら
					push(st, E);	// 東方向をpushして
					targetX--;		// 逃走ポイントを１つ西にずらす
				} else if(myStep[targetX+1][targetY] == i-1) { // その場所に東側から入れるなら
					push(st, W);	// 西方向をpushして
					targetX++;		// 逃走ポイントを１つ東にずらす
				} else if(myStep[targetX][targetY-1] == i-1) { // その場所に北側から入れるなら
					push(st, S);	// 南方向をpushして
					targetY--;		// 逃走ポイントを１つ北にずらす
				}  else if(myStep[targetX][targetY+1] == i-1) { // その場所に南側から入れるなら
					push(st, N);	// 北方向をpushして
					targetY++;		// 逃走ポイントを１つ南にずらす
				} else {	// 変な状態に陥ったとき
					st->count = 0;				// スタックをリセットして
					return STOP;	// とりあえずあきらめるとする
				}
			}
			Action ret = pop(st);
			st->count = 0;	// 中身は捨てて
			int mapX = VISIBLE;
			int mapY = VISIBLE;
			switch(ret) {
			case N:
				mapY--;
				break;
			case S:
				mapY++;
				break;
			case W:
				mapX--;
				break;
			case E:
				mapX++;
			}
			for(int i=0; i<5; i++)	// mapは保存して
				for(int j=0; j<5; j++)
					map[i][j] = myStep[mapX-2+i][mapY-2+j] == -1? -1: 0;
			return ret;
		}
	}
	return STOP;
}