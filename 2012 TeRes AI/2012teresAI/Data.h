#pragma once

#include "DxLib.h"

#define AI_NUM 10
#define ROUND_MAX 10
//ステージの規模
#define WIDTH 32
#define HEIGHT 24

#define BOX (640/WIDTH)

//AIの視界
#define VISIBLE 3

typedef enum{
	OPENING,
	SETTING,
	RUNNING,
	ENDING,
} Mode;

typedef enum{
	N,
	E,
	S,
	W,
	STOP,
} Action;

typedef struct{
	char name[10];//名前
	int Graph;//AIの画像
	int x;//座標x
	int y;//座標y
	int s_x;//
	int s_y;//
	int step;//
	Action act;//
	int life;//
	int view[2*VISIBLE+1][2*VISIBLE+1];//
} AI_T;

typedef struct{
	int Graph;
	int x;
	int y;
	int s_x;
	int s_y;
	int step;
	Action act;
} Tagger;


void intro();
void make_Stage(int Stage[WIDTH][HEIGHT]);
void init_Tagger(Tagger *tagger,int Stage[WIDTH][HEIGHT]);
void init_Ai(AI_T *ai,int Stage[WIDTH][HEIGHT]);
void draw(int stage[WIDTH][HEIGHT],AI_T ai[],Tagger tagger);
Action next_Ai(int view[2*VISIBLE+1][2*VISIBLE+1]);
Action next_Tagger(Tagger tagger,int Stage[WIDTH][HEIGHT]);
void update_Ai(AI_T *ai,int Stage[WIDTH][HEIGHT]);
void update_Tagger(Tagger *tagger,int Stage[WIDTH][HEIGHT]);
void setview_Ai(AI_T *ai,int Stage[WIDTH][HEIGHT]);
int death_Ai(AI_T ai,Tagger tagger);
void result(AI_T ai[],int death[]);
void update_stage(int Stage[WIDTH][HEIGHT],AI_T ai[],Tagger tagger);