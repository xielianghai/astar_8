/*
 * author:	Atom
* date:		2012/12/04
* file:		astar_8.h
*/
#ifndef ASTAR_8_H
#define ASTAR_8_H

#include "bheap.h"

#define MALLOC(type,n)  (type *)malloc((n)*sizeof(type))

#define MAX(a,b) ((a)>(b))?(a):(b)

#define UP  		-3
#define RIGHT 	1
#define DOWN		3
#define LEFT		-1

int node_distance[9][9];

struct step_node
{
	int step_status[9];
	int f;
	int g;
	int h;
	struct step_node* parent;
};

int _comp(struct Bheap_node* n1, struct Bheap_node* n2);

int _eq(struct Bheap_node* n1, struct Bheap_node* n2);

void astar_8(int start[9], int end[9]);

static int calc_distance(int step_status[9], int end[9]);

static int arr_idx(int n, int step_status[9]);

static void init_node_distance(int node_distance[9][9]);

static int is_end(int step_status[9], int end[9]);

static int is_reachable(int space_idx, int flag);

static int move_space(int step_status[9], int flag);

static void print_step_status(int step_status[9]);

static int check_whether_reach(int start[9], int end[9]);

#endif
