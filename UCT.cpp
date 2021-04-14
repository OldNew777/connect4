#include "UCT.h"

#ifndef DEBUG
//#define DEBUG
#endif // !DEBUG

#ifndef TIMES_DEBUG
//#define TIMES_DEBUG
#endif // TIMES_DEBUG

//----------------------------------------------------------------------

int M, N, noX, noY;

//----------------------------------------------------------------------


void UCT::copyBoardTop()
{
	for (int i = 0; i < M; ++i) {
		memmove(board[i], _board[i], sizeof(int) * N);
	}
	memmove(top, _top, sizeof(int) * N);
}

Node* UCT::TreePolicy(int** board, int* top, Node* nodeNow)
{
	while (!nodeNow->gameEnd(board, top)) {
		if (nodeNow->Expandable()) {
			return nodeNow = nodeNow->expand(board, top);
		}
		nodeNow = nodeNow->bestChild(board, top);
	}
	return nodeNow;
}

int UCT::getProfit(int** board, int* top, int turn, int x, int y) const
{
	if (turn == USER && userWin(x, y, M, N, board))
		return USER_WIN_PROFIT;
	if (turn == AI && machineWin(x, y, M, N, board))
		return AI_WIN_PROFIT;
	if (isTie(N, top))
		return TIE_PROFIT;
	return NOT_END;
}

void UCT::randomChoice(int** board, int* top, int turn, int& x, int& y)
{
	//随机选择落子列
	y = rand() % N;
	while (top[y] == FULL)
		y = rand() % N;
	x = --top[y];
	board[x][y] = turn;
	//不可落子点处理
	if (x - 1 == noX && y == noY)
		--top[y];
}

double UCT::defaultPolicy(int** board, int* top, Node* nodeNow)
{
	int turn = nodeNow->getTurn(), x = nodeNow->getX(), y = nodeNow->getY();
	int profit = getProfit(board, top, changeTurn(turn), x, y);
	while (profit == NOT_END) {		//肯定expandable
		randomChoice(board, top, turn, x, y);
		profit = getProfit(board, top, turn, x, y);
		turn = changeTurn(turn);
	}
	return profit;
}

Node* UCT::nextStep()
{
	Node* ansBest = nullptr;
	double maxProfit = -1e30;
	for (int i = 0; i < N; i++) {
		if (root->children[i] == nullptr)
			continue;

		double profitChild = root->children[i]->profit;
		int visitedSumChild = root->children[i]->visitedSum;
		double confidenceUpper = profitChild / visitedSumChild;

		if (confidenceUpper > maxProfit || (confidenceUpper == maxProfit && rand() % 2 == 0)) {
			maxProfit = confidenceUpper;
			ansBest = root->children[i];
		}
	}
	return ansBest;
}

UCT::UCT(int _M, int _N, int _noX, int _noY) : root(nullptr), _board(nullptr), _top(nullptr)
{
	startTime = clock();
	M = _M;
	N = _N;
	noX = _noX;
	noY = _noY;

	board = new int* [M];
	top = new int[N];
	for (int i = 0; i < M; ++i)
		board[i] = new int[N];
}

UCT::~UCT()
{
	if (root != nullptr)
		delete root;

	for (int i = 0; i < M; ++i)
		delete[] board[i];
	delete[] board;
	delete[] top;
}

Node* UCT::search(int** board, int* top)
{
	this->_board = board;
	this->_top = top;
	srand(time(nullptr));	//重置随机种子

	root = new Node(this->_board, this->_top);
	while (clock() - startTime <= MAX_TIME) {
		
		copyBoardTop();
		Node* v1 = TreePolicy(this->board, this->top, root);
		double deltaProfit = defaultPolicy(this->board, this->top, v1);
		v1->backUp(deltaProfit);
	}
	
	return nextStep();
}
