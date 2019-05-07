#include "my.h"



Node::Node(){ }

/*Node::Node(int ** _map, int * _top, Point _last, int _parent, bool _myTurn, bool _isOver) :map(_map), top(_top), last(_last), parent(_parent), myTurn(_myTurn), isOver(_isOver) {
	maxChildrenNum = 0;
	for (int i = 0; i < N; i++) {
		if (top[i] > 0) {
			maxChildrenNum++;
		}
	}
	if (maxChildrenNum > 0) {
		child = new Node*[maxChildrenNum];
	}
}*/

void Node::init(int ** _map, int * _top, Point _last, Node * _parent, bool _myTurn, bool _isOver){
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			map[i][j] = _map[i][j];
		}
	}
	for (int i = 0; i < N; i++) {
		top[i] = _top[i];
	}
	init(_last, _parent, _myTurn, _isOver);
}

void Node::init(Point _last, Node * _parent, bool _myTurn, bool _isOver) {
	last = _last;
	parent = _parent;
	myTurn = _myTurn;
	isOver = _isOver;
	childrenNum = 0;
	timeOfVisit = 0;
	timeOfWin = 0;
	maxChildrenNum = 0;
	for (int i = 0; i < N; i++) {
		if (top[i] > 0) {
			maxChildrenNum++;
		}
	}
}

NodePool::NodePool(int num) : /*recycle(new int[num]), */usedNodeNum(0), /*usedRecycleNum(0),*/ totalNum(num), nodeNumInOnePool(num){ 
	pool.push_back(new Node[num]);
}

int NodePool::getNewNode(){
	if (recycle.empty()) {
		if (usedNodeNum >= totalNum) {
			pool.push_back(new Node[nodeNumInOnePool]);
			totalNum += nodeNumInOnePool;
			//_cprintf("wow\n");
		}
		return usedNodeNum++;
	}
	else {
		int res = recycle.top();
		recycle.pop();
		return res;
	}
}

Node * NodePool::getNode(int num){
	if (num < 0) {
		return NULL;
	}
	else {
		return &pool[num / nodeNumInOnePool][num % nodeNumInOnePool];
	}
}

void NodePool::deleteNode(int num){
	/*if(usedRecycleNum > totalNum){
		_cprintf("cao\n");
	}*/
	//recycle[usedRecycleNum++] = num;
	recycle.push(num);
}

void Node::print(){
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			_cprintf("%d", map[i][j]);
		}
		_cprintf("\n");
	}
}

Point uctSearch(int ** map, int * top, clock_t restTime){
	clock_t startTime, endTime;
	startTime = clock();
	int M = Node::M;
	int N = Node::N;
	int rootInt = NodePool::nodePool->getNewNode();
	NodePool::nodePool->getNode(rootInt)->init(map, top, Point(-1, -1), NULL, true, false);
	//Node * root = new Node(newmap, newtop, Point(-1, -1), NULL, true, false);
	endTime = clock();
	return uctSearch(rootInt, restTime - (endTime - startTime));
}

Point uctSearch(int root, clock_t restTime) {
	clock_t totalTime = 0;
	clock_t startTime, endTime;

	bool flag500 = true, flag1000 = true, flag2000 = true;

	startTime = clock();
	if (Node::root != -1) {
		deleteTree(Node::root, root);
	}
	Node::root = root;
	//_cprintf("start use %d ms\n", totalTime);
	Point resForFastFind(-1, -1);
	int loopCount = 0;
	while (totalTime < restTime) {


		//if (fastFind(resForFastFind)) {
			//return  resForFastFind;
		//}

		//_cprintf("ok3");
		Node * v = treePolicy(root);
		//_cprintf("ok4");
		//_cprintf("ok0");
		int profit = defaultPolicy(v);
		//_cprintf("ok5");
		//_cprintf("ok1");
		backup(v, profit);
		//_cprintf("ok2");
		//v->print();
		//_cprintf("%d\n", profit);



		/*if ((totalTime >= 500 && flag500) || (totalTime >= 1000 && flag1000) || (totalTime >= 2000 && flag2000)) {
			if (flag500) {
				flag500 = false;
			}
			else if(flag1000){
				flag1000 = false;
			}
			else {
				flag2000 = false;
			}
			//_cprintf("result in %d ms is ", totalTime);
			Node * res = bestChild(root, 0);
		}*/
		if (++loopCount % 10000 == 0) {

			endTime = clock();
			totalTime += endTime - startTime;
			startTime = clock();

		}

	}
	//_cprintf("result in %d ms is ", totalTime);
	startTime = clock();
	int res = bestChild(NodePool::nodePool->getNode(root), 0);
	endTime = clock();

	//_cprintf("choose bestChild use %d ms\n", endTime - startTime);
	Node::next = res;
	/*deleteNode(root, res);*/
	//_cprintf("\n");
	return NodePool::nodePool->getNode(res)->last;
}

/*bool fastFind(Point & res) {
	if (Node::root->childrenNum < Node::root->maxChildrenNum) {
		return false;
	}
	for (int i = 0; i < Node::root->childrenNum; i++) {
		if (Node::root->child[i]->isOver && Node::root->child[i]->myWin) {
			res = Node::root->child[i]->last;
			return true;
		}
	}
}*/

Node * treePolicy(int root){
	Node * v = NodePool::nodePool->getNode(root);
	while (!(v->isOver)) {
		if (v->childrenNum < v->maxChildrenNum) {
			return expand(v);
		}
		else {
			v = NodePool::nodePool->getNode(bestChild(v, UCTC));
		}
	}
	return v;
}

Node * expand(Node * v){
	//_cprintf("ok3");
	int M = Node::M;
	int N = Node::N;
	NodePool * pool = NodePool::nodePool;
	int newInt = pool->getNewNode();
	Node * newNode = pool->getNode(newInt);
	int(*newmap)[12] = newNode->map;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			newmap[i][j] = v->map[i][j];
		}
	}
	int * newtop = newNode->top;
	for (int i = 0; i < N; i++) {
		newtop[i] = v->top[i];
	}
	int count = 0;
	int x, y;
	for (int i = 0; i < N; i++) {
		if (v->top[i] > 0) {
			count++;
		}
		if (count > v->childrenNum) {
			newmap[v->top[i] - 1][i] = (v->myTurn ? 2 : 1);
			newtop[i]--;
			if (v->top[i] >= 2 && newmap[v->top[i] - 2][i] == 3) {
				newtop[i]--;
			}
			x = v->top[i] - 1;
			y = i;
			break;
		}
	}
	bool isOver = false;
	bool myWin = false;
	if (v->myTurn) {
		isOver = machineWin(x, y, M, N, newmap);
		myWin = isOver;
	}
	else {
		isOver = userWin(x, y, M, N, newmap);
	}
	if (!isOver) {
		isOver = isTie(N, newtop);
	}
	newNode->init(Point(x, y), v, !v->myTurn, isOver);
		
	//Node * newnode = new Node(newmap, newtop, Point(x, y), v, !v->myTurn, isOver);
	if (isOver) {
		newNode->myWin = myWin;
	}
	v->child[v->childrenNum++] = newInt;
	return newNode;
}

int bestChild(Node * parent, double c){
	//_cprintf("ok4");
	int maxNum = -1;
	double maxArg = -1;
	int maxTime = -1, maxChildTime = -1;;
	for (int i = 0; i < parent->childrenNum; i++) {
		Node * child = NodePool::nodePool->getNode(parent->child[i]);
		//double v = ((double)child->timeOfWin) / child->timeOfVisit - (((double)child->timeOfWin) / child->timeOfVisit) * (((double)child->timeOfWin) / child->timeOfVisit) + sqrt(2 * log(parent->timeOfVisit) / child->timeOfVisit);
		//double arg = ((double)child->timeOfWin) / child->timeOfVisit + c * sqrt(log(parent->timeOfVisit) / child->timeOfVisit * min(1 / 4, v)); //((double)child->timeOfWin) / child->timeOfVisit + c * sqrt(2 * log(parent->timeOfVisit) / child->timeOfVisit);
		double arg = ((double)child->timeOfWin) / child->timeOfVisit + c * sqrt(2 * log(parent->timeOfVisit) / child->timeOfVisit);
		if (arg > maxArg) {
			maxNum = i;
			maxArg = arg;
			maxTime = parent->timeOfVisit;
			maxChildTime = child->timeOfVisit;
		}
	}
	if (maxNum > -1) {
		if (c == 0) {
			_cprintf("its value is %f, search time is %d / %d\n", maxArg, maxChildTime, maxTime);
		}
		return parent->child[maxNum];
	}
	else {
		//_cprintf("qwerqwerqwerq");
		return -1;
	}
}

int defaultPolicy(Node * v/*int ** map, int * top, bool myTurn*/){
	int M = Node::M;
	int N = Node::N;
	//_cprintf("ok4");
	if (v->isOver) {
		return v->myTurn ^ v->myWin;
		/*if (isTie(N, v->top)) {
			return 0;
		}
		else {
			if (v->myTurn) {
				return (userWin(v->last.x, v->last.y, M, N, v->map) ? 0 : 1);
			}
			else {
				return (machineWin(v->last.x, v->last.y, M, N, v->map) ? 0 : 1);
			}
		}*/
	}
	NodePool * pool = NodePool::nodePool;
	int temp = pool->getNewNode();
	Node * tempNode = pool->getNode(temp);
	int (*map)[12] = tempNode->map;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			map[i][j] = v->map[i][j];
		}
	}
	int *top = tempNode->top;
	for (int i = 0; i < N; i++) {
		top[i] = v->top[i];
	}
	bool myWin = false;
	bool myTurn = v->myTurn;
	int count = 0;
	//_cprintf("ok5");
	while (true) {
		count++;
		int next = random(Node::N);
		//_cprintf("ok6");
		if (top[next] > 0) {
			map[--top[next]][next] = (myTurn ? 2 : 1);
			if (myTurn) {
				if (machineWin(top[next], next, Node::M, Node::N, map)) {
					myWin = true;
					break;
				}
			}
			else{
				if (userWin(top[next], next, Node::M, Node::N, map)) {
					myWin = false;
					break;
				}
			}
			//_cprintf("ok7");
			if (isTie(Node::N, top)) {
				myWin = false;
				break;
			}
			myTurn = !myTurn;
			//_cprintf("ok8");
		}
	}
	pool->deleteNode(temp);
	//_cprintf("ok9");
	/*
	for (int i = 0; i < Node::M; i++) {
		delete[] map[i];
	}
	delete[] map;
	delete[] top;*/
	//_cprintf("ok10");
	return v->myTurn ^ myWin;
}

void backup(Node * v, int profit){
	while (v != NULL) {
		v->timeOfVisit += 1;
		v->timeOfWin += profit;
		profit = 1 - profit;
		//v->print();
		v = v->parent;

	}
}



void deleteTree(int root, int exception) {
	NodePool * pool = NodePool::nodePool;
	Node * rootNode = pool->getNode(root);
	for (int i = 0; i < rootNode->childrenNum; i++) {
		if (rootNode->child[i] != exception) {
			deleteTree(rootNode->child[i], exception);
		}
	}
	
	pool->deleteNode(root);
	/*
	for (int i = 0; i < Node::M; i++) {
		delete[] root->map[i];
	}
	delete[] root->map;
	delete[] root->top;
	delete[] root->child;
	delete root;*/
	if (exception != -1) {
		pool->getNode(exception)->parent = NULL;
	}
}










/*
void copyBroad(int ** board_bef, int ** board_new, int M, int N){
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			board_new[i][j] = board_bef[i][j];
		}
	}
}

void copyTop(int * top_bef, int * top_new, int N) {
	for (int i = 0; i < N; i++) {
		top_new[i] = top_bef[i];
	}
}


int dx[] = { 0, 0, 1, -1, 1, 1, -1, -1 };
int dy[] = { 1, -1, 0, 0, 1, -1, -1, 1 };
int value[] = { 5, 5, 1, 7, 7, 2, 2 };

int evaluate(int ** board, int * top, int M, int N){

	int sum = 0;


	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < 8; k++) {
				sum += (int)((getPointOfDirection(M, N, board, i, j, dx[k], dy[k], true) - getPointOfDirection(M, N, board, i, j, dx[k], dy[k], false)) * value[k]);
			}
		}
	}

	//_cprintf("sum: %d\n", sum);
	return sum;





	/*
	int** boardNew = new int*[M];
	for (int i = 0; i < M; i++) {
		boardNew[i] = new int[N];
		for (int j = 0; j < N; j++) {
			boardNew[i][j] = board[i][j];
		}
	}
	for (int i = 0; i < N; i++) {
		if (top[i] > 0) {
			boardNew[top[i] - 1][i] = 1;
		}
	}
	int usrWins0, cmpWins0;
	oneWins(M, N, boardNew, usrWins0, cmpWins0);

	for (int i = 0; i < N; i++) {
		if (top[i] > 0) {
			boardNew[top[i] - 1][i] = 2;
		}
	}
	int usrWins1, cmpWins1;
	oneWins(M, N, boardNew, usrWins1, cmpWins1);

	for (int i = 0; i < M; i++) {
		delete[] board[i];
	}
	delete[] board;

	return usrWins0 - cmpWins1;*
}


int strategy(int M, int N, int * top, int ** board, int& x, int& y, int depth, bool ismax, int alpha, int beta, treenode * node){
	if (depth == 0) {
		int eva = evaluate(board, top, M, N);
		node->eva = eva;
		return eva;
	}
	else {
		int xx = -1, yy = -1;
		int** boardNew = new int*[M];
		for (int i = 0; i < M; i++) {
			boardNew[i] = new int[N];
		}
		int * topNew = new int[N];

		for (int i = 0; i < N; i++) {
			if (top[i] > 0) {
				int eva;
				bool ok = false;

				copyBroad(board, boardNew, M, N);
				copyTop(top, topNew, N);
				boardNew[top[i] - 1][i] = (ismax ? 2 : 1);
				topNew[i]--;

				if (ismax) {
					if (machineWin(top[i] - 1, i, M, N, boardNew)) {
						eva = SUP;
						ok = true;
					}
				}
				else {
					/*for (int i = 0; i < M; i++) {
						for (int j = 0; j < N; j++) {
							_cprintf("%d", board[i][j]);
						}
						_cprintf("\n");
					}
					_cprintf("\n");*
					if (userWin(top[i] - 1, i, M, N, boardNew)) {
						//_cprintf("machinewin\n");
						eva = INF;
						ok = true;
					}
				}
				if (!ok) {
					treenode * son = new treenode;
					node->son.push_back(son);
					eva = strategy(M, N, topNew, boardNew, x, y, depth - 1, !ismax, alpha, beta, son);
				}
				if (ismax) {
					if (eva > alpha) {
						alpha = eva;
						xx = top[i] - 1;
						yy = i;
					}
				}
				else {
					if (eva < beta) {
						beta = eva;
					}
				}
				if (beta <= alpha) {
					break;
				}

				//_cprintf("%d, %d, %d\n", top[i] - 1, i, eva);
			}
		}

		for (int i = 0; i < M; i++) {
			delete[] boardNew[i];
		}
		delete[] boardNew;

		delete[] topNew;

		node->eva = (ismax ? alpha : beta);

		if (depth == DEPTH) {
			x = xx;
			y = yy;
		}
		if (depth == DEPTH && xx == -1) {
			return INF - 1;
		}

		return (ismax ? alpha : beta);
	}
}

int getPointOfFiveChess(int * line, bool isUsr) {
	int now = (isUsr ? 2 : 1);
	int antinow = (isUsr ? 1 : 2);
	if ((line[0] == antinow || line[1] == antinow) && (line[3] == antinow || line[4] == antinow)) return 0;
	if (line[0] == 0 && line[1] == now && line[2] == now && line[3] == now && line[4] == 0) return 10000;
	if ((line[0] == 0 && line[1] == 0 && line[2] == now && line[3] == now && line[4] == now) ||
		(line[0] == now && line[1] == now && line[2] == now && line[3] == 0 && line[4] == 0)) return 1500;
	if ((line[1] == 0 && line[2] == now && line[3] == now && line[4] == now) ||
		(line[0] == now && line[1] == now && line[2] == now && line[3] == 0) ||
		(line[0] == 0 && line[1] == now && line[2] == now && line[3] == now) ||
		(line[1] == now && line[2] == now && line[3] == now && line[4] == 0)) return 1000;
	if ((line[0] == 0 && line[1] == 0 && line[2] == 0 && line[3] == now && line[4] == now) ||
		(line[0] == now && line[1] == now && line[2] == 0 && line[3] == 0 && line[4] == 0)) return 250;
	if ((line[0] == 0 && line[1] == 0 && line[2] == now && line[3] == now && line[4] == 0) ||
		(line[0] == 0 && line[1] == now && line[2] == now && line[3] == 0 && line[4] == 0)) return 260;
	if ((line[1] == 0 && line[2] == 0 && line[3] == now && line[4] == now) ||
		(line[0] == now && line[1] == now && line[2] == 0 && line[3] == 0)) return 130;
	if ((line[1] == 0 && line[2] == now && line[3] == now && line[4] == 0) ||
		(line[0] == 0 && line[1] == now && line[2] == now && line[3] == 0)) return 140;
	if ((line[0] == 0 && line[1] == 0 && line[2] == 0 && line[3] == 0 && line[4] == now) ||
		(line[0] == now && line[1] == 0 && line[2] == 0 && line[3] == 0 && line[4] == 0)) return 5;
	if ((line[0] == 0 && line[1] == 0 && line[2] == 0 && line[3] == now && line[4] == 0) ||
		(line[0] == 0 && line[1] == now && line[2] == 0 && line[3] == 0 && line[4] == 0)) return 7;
	if (line[0] == 0 && line[1] == 0 && line[2] == now && line[3] == 0 && line[4] == 0) return 10;
	if ((line[1] == 0 && line[2] == 0 && line[3] == 0 && line[4] == now) ||
		(line[0] == now && line[1] == 0 && line[2] == 0 && line[3] == 0)) return 1;
	if ((line[1] == 0 && line[2] == 0 && line[3] == now && line[4] == 0) ||
		(line[0] == 0 && line[1] == now && line[2] == 0 && line[3] == 0)) return 3;
	return 0;
}


int getPointOfDirection(int M, int N, int ** board, int x, int y, int directionX, int directionY, bool isUsr) {
	int line[5];
	int now = board[x][y];
	for (int i = 0; i < 5; i++) {
		if (x >= 0 && y >= 0 && x < M && y < N) {
			line[i] = board[x][y];
		}
		else {
			line[i] = (isUsr ? 1 : 2);
		}
		x += directionX;
		y += directionY;
	}
	//_cprintf("%d, %d, %d, %d, %d, eva: %d", line[0], line[1], line[2], line[3], line[4], getPointOfFiveChess(line, isUsr));
	return getPointOfFiveChess(line, isUsr);
}

/*
void oneWins(int M, int N, int** board, int & usrWins, int & cmpWins) {


	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < 8; k++) {

			}
		}
	}






	int sumUsr = 0;
	int sumCmp = 0;
	int count = 0;
	int now = 0;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] == 0) {
				now = 0;
				count = 0;
			}
			else if (board[i][j] == 1) {
				if (now == 1) {
					count++;
				}
				else {
					now = 1;
					count = 1;
				}
			}
			else {
				if (now == 2) {
					count++;
				}
				else {
					now = 2;
					count = 1;
				}
			}
			if (count == 4) {
				if (now == 1) {
					sumUsr++;
				}
				else {
					sumCmp++;
				}
				count = 3;
			}
		}
		count = 0;
		now = 0;
	}

	for (int j = 0; j < N; j++) {
		for (int i = 0; i < M; i++) {
			if (board[i][j] == 0) {
				now = 0;
				count = 0;
			}
			else if (board[i][j] == 1) {
				if (now == 1) {
					count++;
				}
				else {
					now = 1;
					count = 1;
				}
			}
			else {
				if (now == 2) {
					count++;
				}
				else {
					now = 2;
					count = 1;
				}
			}
			if (count == 4) {
				if (now == 1) {
					sumUsr++;
				}
				else {
					sumCmp++;
				}
				count = 3;
			}
		}
		count = 0;
		now = 0;
	}

	for (int i = 0; i < M + N; i++) {
		for (int j = 0; j <= i; j++) {
			if (i - j >= M || j >= N) {
				continue;
			}
			if (board[i - j][j] == 0) {
				now = 0;
				count = 0;
			}
			else if (board[i - j][j] == 1) {
				if (now == 1) {
					count++;
				}
				else {
					now = 1;
					count = 1;
				}
			}
			else {
				if (now == 2) {
					count++;
				}
				else {
					now = 2;
					count = 1;
				}
			}
			if (count == 4) {
				if (now == 1) {
					sumUsr++;
				}
				else {
					sumCmp++;
				}
				count = 3;
			}
		}
		count = 0;
		now = 0;
	}

	for (int i = 0; i < M + N; i++) {
		for (int j = 0; j <= i; j++) {
			if (i - j >= M || j >= N) {
				continue;
			}
			if (board[M - 1 - i + j][j] == 0) {
				now = 0;
				count = 0;
			}
			else if (board[M - 1 - i + j][j] == 1) {
				if (now == 1) {
					count++;
				}
				else {
					now = 1;
					count = 1;
				}
			}
			else {
				if (now == 2) {
					count++;
				}
				else {
					now = 2;
					count = 1;
				}
			}
			if (count == 4) {
				if (now == 1) {
					sumUsr++;
				}
				else {
					sumCmp++;
				}
				count = 3;
			}
		}
		count = 0;
		now = 0;
	}

	usrWins = sumUsr;
	cmpWins = sumCmp;

}*/
