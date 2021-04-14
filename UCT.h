#pragma once

#include "Node.h"
#include <time.h>
#include <string.h>

#define FULL 0

#define EMPTY_UNAVAILIBLE 0		//空置，或者不可下的位置
#define USER 1				//玩家的棋子、轮次
#define AI 2					//我方AI的棋子、轮次

#define MAX_TIME (2.8 * CLOCKS_PER_SEC)			//最大运行时间

#define NOT_END -100

using namespace std;


class UCT
{
public:
	UCT(int _M, int _N, int _noX, int _noY);
	~UCT();

	Node* search(int** board, int* top);

protected:
	//-----------------------------成员变量-----------------------------
	//根节点对应的_board和_top，全程不改变
	int** _board;
	int* _top;

	//暂时的缓冲board和top
	int** board;
	int* top;

	Node* root;
	long startTime;

	//-----------------------------成员函数-----------------------------
	void copyBoardTop();		//将根节点对应的数据拷贝一份
	Node* TreePolicy(int** board, int* top, Node* nodeNow);
	int getProfit(int** board, int* top, int turn, int x, int y) const;
	void randomChoice(int** board, int* top, int turn, int& x, int& y);
	double defaultPolicy(int** board, int* top, Node* nodeNow);

	Node* nextStep();	//模拟完之后，找出最终
};

