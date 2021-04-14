#include "Node.h"

Node::Node(int** board, int* top, int depth, int lastX, int lastY, int turn, Node* father) :
	depth(depth), x(lastX), y(lastY), turn(turn), father(father)
{
	expandableSum = 0;
	profit = 0;
	visitedSum = 0;

	for (int i = 0; i < N; ++i) {
		if (top[i] != FULL) {
			expandableIndex[expandableSum++] = i;
		}
		children[i] = nullptr;
	}
}

Node::~Node()
{
	for (int i = 0; i < N; ++i) {
		if (children[i] != nullptr)
			delete children[i];
	}
}

bool Node::gameEnd(int** board, int* top) const
{
	if (x == -1 && y == -1)	//根节点
		return false;
	if (turn == USER && machineWin(x, y, M, N, board))
		return true;
	if (turn == AI && userWin(x, y, M, N, board))
		return true;
	if (isTie(N, top))
		return true;
	return false;
}

Node* Node::expand(int** board, int* top)
{
	// 随机确定一个扩展点
	int index = rand() % expandableSum;

	int newY = expandableIndex[index];
	int newX = --top[newY];
	board[newX][newY] = turn;

	//如果落在不可落子点上方，说明top要+2而非+1
	if (newX - 1 == noX && newY == noY)
		--top[newY];

	int nextTurn = changeTurn(turn);
	children[newY] = new Node(board, top, depth + 1, newX, newY, nextTurn, this);

	//该节点的实际可扩展子节点数量-1
	swap(expandableIndex[index], expandableIndex[--expandableSum]);
	return children[newY];
}

Node* Node::bestChild(int** board, int* top)
{
	Node* ansBest = nullptr;
	double maxProfit = -1e30;
	for (int i = 0; i < N; i++) {
		if (children[i] == nullptr)
			continue;

		//根据目前是极大点还是极小点决定profit
		double profitChild = ((turn == AI) ? 1 : -1) * children[i]->profit;

		int visitedSumChild = children[i]->visitedSum;
		//计算信心上界
		double confidenceUpper = profitChild / visitedSumChild + sqrt(2 * log(visitedSum) / visitedSumChild) * PARAMETER_C;
		if (confidenceUpper > maxProfit || (confidenceUpper == maxProfit && rand() % 2 == 0)) {
			maxProfit = confidenceUpper;
			ansBest = children[i];
		}
	}
	board[ansBest->x][ansBest->y] = turn;
	--top[ansBest->y];
	//不可落子点处理
	if (ansBest->x - 1 == noX && ansBest->y == noY)
		--top[ansBest->y];
	return ansBest;
}

void Node::backUp(double deltaProfit)
{
	Node* ptr = this;
	while (ptr != nullptr) {
		ptr->visitedSum++;
		ptr->profit += deltaProfit;
		ptr = ptr->father;
	}
}


//----------------------------------------------------------

int changeTurn(int turn)
{
	if (turn == USER)
		return AI;
	if (turn == AI)
		return USER;
	return -1;
}
