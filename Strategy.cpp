#include <iostream>
#include "Point.h"
#include "Strategy.h"

#include "my.h"

using namespace std;

int Node::M = -1;
int Node::N = -1;
int Node::root = -1;
int Node::next = -1;

NodePool * NodePool::nodePool = new NodePool(100000);

/*
	策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误
	
	input:
		为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
		M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计， 左上角为坐标原点，行用x标记，列用y标记
		top : 当前棋盘每一列列顶的实际位置. e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0
		_board : 棋盘的一维数组表示, 为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
				你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
				board[x][y]表示第x行、第y列的点(从0开始计)
				board[x][y] == 0/1/2 分别对应(x,y)处 无落子/有用户的子/有程序的子,不可落子点处的值也为0
		lastX, lastY : 对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
				落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
		noX, noY : 棋盘上的不可落子点(注:其实这里给出的top已经替你处理了不可落子点，也就是说如果某一步
				所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
				所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
				当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
		以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX lastY),你要做的就是在这些信息下给出尽可能明智的落子点
	output:
		你的落子点Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, 
	const int lastX, const int lastY, const int noX, const int noY){
	/*
		不要更改这段代码
	*/
	int x = -1, y = -1;//最终将你的落子点存到x,y中
	/*int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}*/
	
	/*
		根据你自己的策略来返回落子点,也就是根据你的策略完成对x,y的赋值
		该部分对参数使用没有限制，为了方便实现，你可以定义自己新的类、.h文件、.cpp文件
	*/
	//Add your own code below
	AllocConsole();


	if (checkForStart(_board, M, N)/*Node::M == -1*/) {
		_cprintf("ok0");
		srand((int)time(0));

		clock_t startTime, endTime;
		startTime = clock();

		if (Node::root != -1) {
			deleteTree(Node::root, -1);
			Node::root = -1;
			Node::next = -1;
		}

		int** board = new int*[M];
		for (int i = 0; i < M; i++) {
			board[i] = new int[N];
			for (int j = 0; j < N; j++) {
				board[i][j] = _board[i * N + j];
			}
		}
		board[noX][noY] = 3;
		Node::M = M;
		Node::N = N;

		int * myTop = new int[N];

		for (int i = 0; i < N; i++) {
			myTop[i] = top[i];
		}

		endTime = clock();
		Point res = uctSearch(board, myTop, SUMTIME - (endTime - startTime));

		x = res.x;
		y = res.y;
		delete[] myTop;
		clearArray(M, N, board);
	}
	else {
		NodePool * pool = NodePool::nodePool;
		for (int i = 0; i < pool->getNode(Node::next)->childrenNum; i++) {
			int child = pool->getNode(Node::next)->child[i];
			if (pool->getNode(child)->last.y == lastY) {
				//deleteNode(Node::root, child);
				Point res = uctSearch(child, SUMTIME);
				x = res.x;
				y = res.y;
				break;
			}
		}
	}

	_cprintf("usedNodeNum %d, usedRecycleNum %d\n", NodePool::nodePool->usedNodeNum, NodePool::nodePool->recycle.size());
	/*
	treenode * root = new treenode;

	int eva = strategy(M, N, myTop, board, x, y, DEPTH, true, INF - 1, SUP + 1, root);*/
	
	/*
	if (eva == INF - 1) {



		_cprintf("root: \n%d\n", root->eva);

		_cprintf("first: \n");
		for (int i = 0; i < root->son.size(); i++) {
			_cprintf("%d ", root->son[i]->eva);
		}
		_cprintf("\n");
		_cprintf("second: \n");
		for (int i = 0; i < root->son.size(); i++) {
			for (int j = 0; j < root->son[i]->son.size(); j++) {
				_cprintf("%d ", root->son[i]->son[j]->eva);
			}
			_cprintf("\n");
		}
		_cprintf("third: \n");
		for (int i = 0; i < root->son.size(); i++) {
			for (int j = 0; j < root->son[i]->son.size(); j++) {
				for (int k = 0; k < root->son[i]->son[j]->son.size(); k++) {
					_cprintf("%d ", root->son[i]->son[j]->son[k]->eva);
				}
				_cprintf("\n");
			}
		}
		_cprintf("fourth: \n");
		for (int i = 0; i < root->son.size(); i++) {
			for (int j = 0; j < root->son[i]->son.size(); j++) {
				for (int k = 0; k < root->son[i]->son[j]->son.size(); k++) {
					for (int p = 0; p < root->son[i]->son[j]->son[k]->son.size(); p++) {
						_cprintf("%d ", root->son[i]->son[j]->son[k]->son[p]->eva);
					}
					_cprintf("\n");
				}
			}
		}

		_cprintf("%d, %d, %d\n", x, y, eva);
	}*/


	
     //a naive example
	/*for (int i = N-1; i >= 0; i--) {
		if (top[i] > 0) {
			x = top[i] - 1;
			y = i;
			break;
		}
	}*/
    
	
	
	/*
		不要更改这段代码
	*/
	
	return new Point(x, y);
}


/*
	getPoint函数返回的Point指针是在本dll模块中声明的，为避免产生堆错误，应在外部调用本dll中的
	函数来释放空间，而不应该在外部直接delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	清除top和board数组
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}

bool checkForStart(const int * map, int M, int N) {
	for (int i = M * N - 1; i >= 0; i--) {
		if (map[i] == 2) {
			return false;
		}
	}
	return true;
}

/*
	添加你自己的辅助函数，你可以声明自己的类、函数，添加新的.h .cpp文件来辅助实现你的想法
*/
