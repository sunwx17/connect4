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
	���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������
	
	input:
		Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
		M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
		top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
		_board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
				��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
				board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
				board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
		lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
				����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
		noX, noY : �����ϵĲ������ӵ�(ע:��ʵ���������top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
				������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
				��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
				��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
		���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
	output:
		������ӵ�Point
*/
extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, 
	const int lastX, const int lastY, const int noX, const int noY){
	/*
		��Ҫ������δ���
	*/
	int x = -1, y = -1;//���ս�������ӵ�浽x,y��
	/*int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}*/
	
	/*
		�������Լ��Ĳ������������ӵ�,Ҳ���Ǹ�����Ĳ�����ɶ�x,y�ĸ�ֵ
		�ò��ֶԲ���ʹ��û�����ƣ�Ϊ�˷���ʵ�֣�����Զ����Լ��µ��ࡢ.h�ļ���.cpp�ļ�
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
		��Ҫ������δ���
	*/
	
	return new Point(x, y);
}


/*
	getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
	�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
*/
extern "C" __declspec(dllexport) void clearPoint(Point* p){
	delete p;
	return;
}

/*
	���top��board����
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
	������Լ��ĸ�������������������Լ����ࡢ����������µ�.h .cpp�ļ�������ʵ������뷨
*/
