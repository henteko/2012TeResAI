/**********************************************************
	AIのサンプル
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
Action kuzumi(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	// 逃げるようの変数
	static stack *st = (stack*)malloc(sizeof(stack));
	static int initFlag = 1;
	static int bfview[5][5] = {-1};

	/* 視界の2値化作業 */
	int myStep[2*VISIBLE+1][2*VISIBLE+1];
	int taggerStep[2*VISIBLE+1][2*VISIBLE+1];
	for(int i=0; i<2*VISIBLE+1; i++)		// 2値化とtaggerの把握
		for(int j=0; j<2*VISIBLE+1; j++)	// (way or wall)
		{
			switch(view[i][j])
			{
			case 3:
				myStep[i][j] = 0;
				taggerStep[i][j] = 1;
				break;
			case 0:
			case 2:
				myStep[i][j] = 0;
				taggerStep[i][j] = 0;
				break;
			case 1:
				myStep[i][j] = -1;
				taggerStep[i][j] = -1;
			}
		}
	myStep[VISIBLE][VISIBLE] = 1;			// 自分の足元を１とする

	for(int i=0; i<5; i++)
		for(int j=0; j<5; j++)
			if(bfview[i][j] != myStep[VISIBLE-2+i][VISIBLE-2+j])
			{
				initFlag = 1;
				goto init;
			}

init:
	if(initFlag)
	{
		initFlag = 0;
		st->count = 0;
	}

	if(st->count > 0)
	{
		Action ret = pop(st);
		int nextVx=0, nextVy=0;
		if(ret == N)
			nextVy--;
		else if(ret == S)
			nextVy++;
		else if(ret == E)
			nextVx++;
		else if(ret == W)
			nextVx--;
		for(int i=0; i<5; i++)
			for(int j=0; j<5; j++)
			{
				bfview[i][j] = myStep[VISIBLE-2+i+nextVx][VISIBLE-2+j+nextVy];
				if(bfview[i][j] != -1)
					bfview[i][j] = 0;
			}
		bfview[2][2] = 1;
		return ret;
	}


	/* マスごとの自分とオニの距離を求める */
	for(int step=1; step<KZ_STEPMAX; step++)
	{
		for(int i=0; i<2*VISIBLE+1; i++)
			for(int j=0; j<2*VISIBLE+1; j++)
			{
				if(myStep[i][j] == step)
				{
					if(myStep[i+1][j] == 0)
						myStep[i+1][j] = step+1;
					if(myStep[i-1][j] == 0)
						myStep[i-1][j] = step+1;
					if(myStep[i][j-1] == 0)
						myStep[i][j-1] = step+1;
					if(myStep[i][j+1] == 0)
						myStep[i][j+1] = step+1;
				}
				if(taggerStep[i][j] == step)
				{
					if(taggerStep[i+1][j] == 0)
						taggerStep[i+1][j] = step+1;
					if(taggerStep[i-1][j] == 0)
						taggerStep[i-1][j] = step+1;
					if(taggerStep[i][j-1] == 0)
						taggerStep[i][j-1] = step+1;
					if(taggerStep[i][j+1] == 0)
						taggerStep[i][j+1] = step+1;
				}
			}
	}
	for(int i=0; i<2*VISIBLE+1; i++)
		for(int j=0; j<2*VISIBLE+1; j++)
		{
			if(myStep[i][j] == 0)
				myStep[i][j] = -1;
			if(taggerStep[i][j] == 0)
				taggerStep[i][j] = -1;
		}

	if(taggerStep[VISIBLE][VISIBLE] > 0) // 見える範囲で自分に手が届く
	{
		int runX=VISIBLE, runY=VISIBLE; //現在地を基準にする
		int distance=taggerStep[VISIBLE][VISIBLE] - myStep[VISIBLE][VISIBLE];
		for(int i=0; i<2*VISIBLE+1; i++)
			for(int j=0; j<2*VISIBLE+1; j++)
				if(myStep[i][j]>0 && taggerStep[i][j]>0) // どちらも到達できる場所について
				{
					int tmp = taggerStep[i][j] - myStep[i][j];
					if(distance < tmp)
					{
						distance = tmp;
						runX = i; runY = j;
					}
					else if(distance == tmp && (myStep[i][j] > myStep[runX][runY]))
					{
						runX = i; runY = j;
					}
				}

		/* 行き先までの道のりをpushする */
		while(runX != VISIBLE || runY != VISIBLE) // runはゴール地点なので、逆走してたどる
		{
			if(myStep[runX-1][runY] == myStep[runX][runY]-1) // 西側が一歩手前
			{
				push(st, E);	// 東方向をpushして
				runX -= 1;		// ゴールをその場所に
			}
			else if(myStep[runX+1][runY] == myStep[runX][runY]-1)
			{
				push(st, W);
				runX += 1;
			}
			else if(myStep[runX][runY-1] == myStep[runX][runY]-1)
			{
				push(st, S);
				runY -= 1;
			}
			else if(myStep[runX][runY+1] == myStep[runX][runY]-1)
			{
				push(st, N);
				runY += 1;
			}
			else
			{
				st->count = 0;
				return STOP;
			}
		}
		Action ret = pop(st);
		int nextVx=0, nextVy=0;
		if(ret == N)
			nextVy--;
		else if(ret == S)
			nextVy++;
		else if(ret == E)
			nextVx++;
		else if(ret == W)
			nextVx--;
		for(int i=0; i<5; i++)
			for(int j=0; j<5; j++)
			{
				bfview[i][j] = myStep[VISIBLE-2+i+nextVx][VISIBLE-2+j+nextVy];
				if(bfview[i][j] != -1)
					bfview[i][j] = 0;
			}
		bfview[2][2] = 1;
		return ret;
	}
	else // 近くに鬼がいない
	{
		return (Action)GetRand(3);
	}
	return STOP;
}