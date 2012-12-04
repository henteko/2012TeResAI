/**********************************************************
	AIのサンプル
**********************************************************/

#include "Data.h"
#include <cassert>
#include <queue>
#include "windows.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cstring>

#define NOMEANING_DEBUG
#undef NOMEANING_DEBUG

namespace nomeaning{
	const int MERGIN = 100;
	int tx, ty;
	int dist[MERGIN * 2][MERGIN * 2];
	int mdist[MERGIN * 2][MERGIN * 2];
	int MAP[MERGIN * 2][MERGIN * 2];
	enum{EMPTY,WALL,AI,ENEMY};
	// しょきか
	void init(){
		tx = MERGIN;
		ty = MERGIN;
		for(int i = 0; i < MERGIN * 2; i++){
			for(int j = 0; j < MERGIN * 2; j++){
				MAP[i][j] = WALL;
			}
		}
	}
	// きんぼー
	int dx[4] = {1, 0, -1, 0},
		dy[4] = {0, 1, 0, -1};
	Action d[4] = {E, S, W, N};
	string d_s[4] = {"E", "S", "W", "N"};

	struct Point{
		int x,y;
		Point(int x,int y):x(x),y(y){}
		const bool operator<(const Point &a)const {
			if(x != a.x){
				return x < a.x;
			}else{
				return y < a.y;
			}
		}
	};
	int distance(Point a, Point b){
		return abs(a.x - b.x) + abs(a.y - b.y);
	}
	vector<Point> loop;
}
using namespace nomeaning;

/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void AINomeaningInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\oIcon6.png");  //画像の設定
	strcpy_s(myAi.name, "nomeaningAI");  //自分のAIの名前設定
	init();
}


void output_number(int num){
	stringstream ss;
	ss<<num<<"\n";
	OutputDebugString(ss.str().c_str());
}

/**********************************************************
	AIの行動を返す関数
**********************************************************/
Action AINomeaning(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	const int N = 2 * VISIBLE + 1; // 配列の大きさ
	int px = VISIBLE + tx, py = VISIBLE + ty; // プレイヤーのざひょー
	Action ret = STOP; // もどりち
	int ex = -1, ey = -1; // 敵のざひょー
	
	// マップを確定させる
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			if(view[x][y] != WALL){
				MAP[x + tx][y + ty] = EMPTY;
			}
			if(view[x][y] == ENEMY){
				ex = x + tx; ey = y + ty;
			}
			if((MAP[x + tx][y + ty] == EMPTY && view[x][y] == WALL)){
				init();
			}
		}
	}
	// びーえふえす
	if(true) {
		typedef pair<int,int> P;
		memset(mdist, -1, sizeof(mdist));
		queue<P> que;
		mdist[px][py] = 0;
		que.push(P(px,py));
		while(!que.empty()){
			P now = que.front(); que.pop();
			for(int k = 0; k < 4; k++){
				int nx = now.first + dx[k], ny = now.second + dy[k];
				assert(nx >= 0 && nx < MERGIN * 2 && ny >= 0 && ny < MERGIN * 2);
				if(mdist[nx][ny] == -1 && MAP[nx][ny] != WALL){
					mdist[nx][ny] = mdist[now.first][now.second] + 1;
					que.push(P(nx,ny));
				}
			}
		}
	}
	memset(dist, -1, sizeof(dist));
	if(ex != -1){
		const int INF = -1;
		// BFS
		typedef pair<int,int> P;
		queue<P> que;
		dist[ex][ey] = 0;
		que.push(P(ex,ey));
		while(!que.empty()){
			P now = que.front(); que.pop();
			for(int k = 0; k < 4; k++){
				int nx = now.first + dx[k], ny = now.second + dy[k];
				assert(nx >= 0 && nx < MERGIN * 2 && ny >= 0 && ny < MERGIN * 2);
				if(dist[nx][ny] == INF && MAP[nx][ny] != WALL){
					dist[nx][ny] = dist[now.first][now.second] + 1;
					que.push(P(nx,ny));
				}
			}
		}
#ifdef NOMEANING_DEBUG
	OutputDebugString("-dist-\n");
	for(int y = 0; y < N; y++){
		for(int x = 0; x < N; x++){
			string to;
			to.push_back(dist[x+tx][y+ty] + '0');
			OutputDebugStringA(to.c_str());
		}
		OutputDebugString("\n");
	}
	Sleep(1000);
#endif
	}else{
	}
	typedef pair<int,int> P;

	vector<P> kh, moves;
	/// にげる場所を探す
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			if(MAP[x+tx][y+ty] == WALL){
				int cnt = 0;
				for(int i = -1; i <= 1; i++){
					for(int j = -1; j <= 1; j++){
						if(MAP[x+tx+i][y+ty+j] == EMPTY){
							cnt++;
						}
					}
				}
				if(cnt == 8){
					// にげる場所の候補
					kh.push_back(P(x+tx,y+ty));
					for(int i = -1; i <= 1; i++){
						for(int j = -1; j <= 1; j++){
							if(MAP[x+tx+i][y+ty+j] == EMPTY){
								moves.push_back(P(x+tx+i,y+ty+j));
							}
						}
					}
				}

			}
		}
	}

	sort(moves.begin(), moves.end());
	output_number(kh.size());
	kh.clear();
	if(kh.empty()){
		// output_number(tmp);
		int min_dist = dist[px][py] - 10;
		int ord[4]={0,1,2,3};
		random_shuffle(ord,ord+4);
		for(int k = 0; k < 4; k++){
			int xx = px + dx[ord[k]],
				yy = py + dy[ord[k]];
			if(dist[xx][yy] >= min_dist){
				min_dist = dist[xx][yy];
				ret = d[ord[k]];
				//OutputDebugString(d_s[k].c_str());
				//output_number(min_dist);
			}
		}
	}else{
		int md = 0; // 最も近い場所
		for(int i = 0; i < (int)kh.size(); i++){
			if(mdist[kh[md].first][kh[md].second] > mdist[kh[i].first][kh[i].second]){
				md = i;
			}
		}

	}
	// でばっぐ用の表示
#ifdef NOMEANING_DEBUG
	OutputDebugString("-------\n");
	for(int y = 0; y < N; y++){
		for(int x = 0; x < N; x++){
			string to;
			to.push_back(view[x][y] + '0');
			OutputDebugStringA(to.c_str());
		}
		OutputDebugString("\n");
	}
	Sleep(1000);
#endif

	// 移動場所に応じて情報を更新する
	if(ret == E){
		tx += 1;
		ty += 0;
	}else if(ret == W){
		tx -= 1;
		ty += 0;
	}else if(ret == N){
		tx += 0;
		ty -= 1;
	}else if(ret == S){
		tx += 0;
		ty += 1;
	}
	return ret;
	/**
		Action: E S W N STOPかな
	*/
}

#undef NOMEANING_DEBUG