#pragma once

#include "Judge.h"
#include <random>
#include <locale>
#include <string.h>

#define FULL 0					//该列已经下满了

#define EMPTY_UNAVAILIBLE 0		//空置，或者不可下的位置
#define USER 1				//玩家的棋子代表数字
#define AI 2					//我方AI的棋子代表数字

#define TIE_PROFIT 0			//平局收益
#define USER_WIN_PROFIT -1	//玩家获胜时的收益
#define AI_WIN_PROFIT 1			//我方AI获胜时的收益

#define PARAMETER_C 1.38

using namespace std;

//-------------------------变量---------------------------

int extern M, N, noX, noY;

//-------------------------函数---------------------------

int changeTurn(int turn);

//-------------------------类---------------------------

enum class NodeState
{
	UNKNOW = 0,
	AI_WIN = 1,
	PLAYER_WIN = 2
};

class UCT;

class Node
{
protected:
	Node* father;

	int x, y;	//记录该节点的上一步对手的落子位置
	int turn;	//记录该节点的棋权为AI还是USER
	int depth;	//该节点的深度（暂时没什么用）
	int visitedSum;	//该节点的访问总数

	int expandableSum;	//N列中，未满、可以落子的有几列
	int expandableIndex[12];		//前expandableSum个位置记录可以扩展的结点列标
	Node* children[12];				//子结点

	double profit;		//该结点收益

public:
	friend class UCT;

	Node(int** board, int* top, int depth = 0, int lastX = -1, int lastY = -1, int turn = AI, Node* father = nullptr);
	~Node();

	int getX() const { return this->x; }		//该节点是走了哪一步过来的
	int getY() const { return this->y; }		//---
	int getTurn() const { return this->turn; }		//该节点下面轮到谁走

	bool Expandable() const { return expandableSum > 0; }	//是否可以扩展子节点，即棋局是否还能往后面下
	bool gameEnd(int** board, int* top) const;		//游戏是否已经结束

	Node* expand(int** board, int* top);		//扩展子结点
	Node* bestChild(int** board, int* top);	//寻找最优解
	void backUp(double deltaProfit);		//回溯更新自己以及各父结点收益

};