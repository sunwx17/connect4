#pragma once
#include "Judge.h"
#include "Point.h"
#include <conio.h>
#include <atlstr.h>
#include <vector>
#include <stack>
#include <cmath>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define random(x) (rand()%x)

#define UCTC 1
#define SUMTIME 2000


struct Node {
	int map[12][12];
	int top[12];
	Point last;
	int child[12];
	int childrenNum = 0;
	int maxChildrenNum;
	Node * parent;
	bool myTurn;//在这个状态下下一步到我下就为true
	bool isOver, myWin;
	int timeOfVisit = 0, timeOfWin = 0;

	static int M, N;
	static int root;
	static int next;

	Node();
	void init(int ** _map, int * _top, Point _last, Node * _parent, bool _myTurn, bool _isOver);
	void Node::init(Point _last, Node * _parent, bool _myTurn, bool _isOver);
	void print();
};

struct NodePool {
	std::vector<Node *> pool;
	int usedNodeNum;
	int nodeNumInOnePool;
	int totalNum;
	std::stack<int> recycle;
	//int usedRecycleNum;

	static NodePool * nodePool;

	NodePool(int num);
	int getNewNode();
	Node * getNode(int num);
	void deleteNode(int num);
};


Point uctSearch(int ** map, int * top, clock_t restTime);

Point uctSearch(int root, clock_t restTime);

//bool fastFind(Point & res);

Node * treePolicy(int root);

Node * expand(Node * v);

int bestChild(Node * parent, double c);

int defaultPolicy(Node * v);

void backup(Node * v, int profit);

void deleteTree(int root, int exception);





/*
#define SUP 1000000
#define INF -1000000
#define DEPTH 4

struct treenode {
	int eva;
	std::vector<treenode*> son;
};

void copyBroad(int** board_bef, int** board_new, int M, int N);

int evaluate(int **board, int * top, int M, int N);

int strategy(int M, int N, int * top, int ** board, int& x, int& y, int depth, bool ismax, int alpha, int beta, treenode * node);

int getPointOfFiveChess(int * line, bool isUsr);

int getPointOfDirection(int M, int N, int ** board, int x, int y, int directionX, int directionY, bool isUsr);*/