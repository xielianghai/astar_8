/*
 * author:	Atom
* date:		2012/12/04
* file:		astar_8.c
*/
#include "astar_8.h"

int main(int argc, char* argv[])
{
	int i,j;
	
	init_node_distance(node_distance);
	int start[] = {2,3,4,1,8,5,0,7,6};
	int end[] = {1,2,3,8,0,4,7,6,5};
#if 1
	if(check_whether_reach(start, end))
	{
		printf("sorry,No solution!\n");
	}
	else
		astar_8(start, end);
#else
		astar_8(start, end);
#endif
	return (0);
}
static int check_whether_reach(int start[9], int end[9])
{
	int re_st = 0, re_end = 0;
	int tmp = 0;
	int i, j;
	for (i = 0; i < 9; i++)
	{
		for(j = 0; j < i; j++)
			if (start[i] > start[j])
				re_st++;
	}
	
	for (i = 0; i < 9; i++)
	{
		for(j = 0; j < i; j++)
			if (end[i] > end[j])
				re_end++;
	}

	return ((re_st^re_end)&0x01);
}
void astar_8(int start[9], int end[9])
{
	int space_idx;
	int* tmp_step = NULL;
	struct Bheap *o_heap = NULL, *c_heap = NULL;
	struct Bheap_node *inode = NULL, *onode = NULL;
	struct step_node *fnode = NULL;
	struct step_node *omnode = NULL;
	struct Bheap_node *exist_node = NULL;
	size_t idx;
	
	o_heap = Bheap_create(128, BHEAP_TYPE_SMALL);
	c_heap = Bheap_create(128, BHEAP_TYPE_SMALL);
	
	/* 第一步判断可否由start状态到end状态 */
	
	/* 将起始点入open heap */
	if (NULL == (fnode = MALLOC(struct step_node, 1)))
	{
		fprintf(stderr, "malloc fnode error!\n");
		return;
	}
	
	if (NULL == (inode = MALLOC(struct Bheap_node, 1)))
	{
		fprintf(stderr, "malloc inode error!\n");
		return;
	}
	
	if (NULL == (tmp_step = MALLOC(int, 9)))
	{
		fprintf(stderr, "malloc tmp_step error!\n");
		return;
	}

	memset(fnode, 0x00, sizeof(struct step_node));
	memset(inode, 0x00, sizeof(struct Bheap_node));
	memset(tmp_step, 0x00, sizeof(int) * 9);
	
	memcpy(fnode->step_status, start, sizeof(int) * 9);
	fnode->g = 0;
	fnode->h = calc_distance(fnode->step_status, end);
	fnode->f = fnode->g + fnode->h;
	fnode->parent = NULL;

	inode->value = fnode;	
	Bheap_push(o_heap, inode, _comp);
	
	for ( ; ; )
	{
		omnode = NULL;
		
		if (NULL == (onode = Bheap_pop(o_heap, _comp)))
		{
			break;
		}
		else
		{
			omnode = (struct step_node*)onode->value;
			if (is_end(omnode->step_status, end))
				break;
			Bheap_push(c_heap, onode, _comp);
			
			memset(tmp_step, 0x00, sizeof(int) * 9);
			memcpy(tmp_step, omnode->step_status, sizeof(int) * 9);
			space_idx = arr_idx(0, tmp_step);
			/* 上 */
			if (is_reachable(space_idx, (UP)))
			{
				fnode = NULL;
				inode = NULL;
				if (NULL == (fnode = MALLOC(struct step_node, 1)))
				{
					fprintf(stderr, "malloc fnode error!\n");
					return;
				}
	
				if (NULL == (inode = MALLOC(struct Bheap_node, 1)))
				{
					fprintf(stderr, "malloc inode error!\n");
					return;
				}
				
				move_space(tmp_step, (UP));
				memcpy(fnode->step_status, tmp_step, sizeof(int) * 9);
				fnode->g = omnode->g + 1;
				fnode->h = calc_distance(tmp_step, end);
				fnode->f = fnode->g + fnode->h;
				fnode->parent = omnode;
				inode->value = fnode;
				/* 不在open heap和closed heap 中 */
				if (-1 == is_Bheap_contain(o_heap, inode, _eq) 
					&& -1 == is_Bheap_contain(c_heap, inode, _eq))
				{
					Bheap_push(o_heap, inode, _comp);
					if (is_end(tmp_step, end))
						continue;
				}
				else if(-1 != (idx = is_Bheap_contain(o_heap, inode, _eq)))
				{
					/* 在open heap 中*/
					if (NULL != (exist_node = Bheap_get(o_heap, idx)))
					{
						if (fnode->f < ((struct step_node *)exist_node->value)->f)
						{
							((struct step_node *)(exist_node->value))->f = fnode->f;
							((struct step_node*)(exist_node->value))->parent = fnode->parent;
						}
					}
					free(fnode);
					free(inode);
				}
				else
				{
					free(fnode);
					free(inode);
				}
				
				Bheap_push(c_heap, onode, _comp);
			}
			
			memset(tmp_step, 0x00, sizeof(int) * 9);
			memcpy(tmp_step, omnode->step_status, sizeof(int) * 9);
			space_idx = arr_idx(0, tmp_step);
			/* 下 */
			if (is_reachable(space_idx, (DOWN)))
			{
				
				fnode = NULL;
				inode = NULL;
				if (NULL == (fnode = MALLOC(struct step_node, 1)))
				{
					fprintf(stderr, "malloc fnode error!\n");
					return;
				}
	
				if (NULL == (inode = MALLOC(struct Bheap_node, 1)))
				{
					fprintf(stderr, "malloc inode error!\n");
					return;
				}
				move_space(tmp_step, (DOWN));
				memcpy(fnode->step_status, tmp_step, sizeof(int) * 9);
				fnode->g = omnode->g + 1;
				fnode->h = calc_distance(tmp_step, end);
				fnode->f = fnode->g + fnode->h;
				fnode->parent = omnode;
				inode->value = fnode;
				/* 不在open heap和closed heap 中 */
				if (-1 == is_Bheap_contain(o_heap, inode, _eq) 
					&& -1 == is_Bheap_contain(c_heap, inode, _eq))
				{
					Bheap_push(o_heap, inode, _comp);
					if (is_end(tmp_step, end))
						continue;
				}
				else if(-1 != (idx = is_Bheap_contain(o_heap, inode, _eq)))
				{
					/* 在open heap 中*/
					if (NULL != (exist_node = Bheap_get(o_heap, idx)))
					{
						if (fnode->f < ((struct step_node *)exist_node->value)->f)
						{
							((struct step_node *)(exist_node->value))->f = fnode->f;
							((struct step_node*)(exist_node->value))->parent = fnode->parent;
						}
					}
					free(fnode);
					free(inode);
				}
				else
				{
					free(fnode);
					free(inode);
				}
				
				Bheap_push(c_heap, onode, _comp);
			}
			
			memset(tmp_step, 0x00, sizeof(int) * 9);
			memcpy(tmp_step, omnode->step_status, sizeof(int) * 9);
			space_idx = arr_idx(0, tmp_step);
			/* 左 */
			if (is_reachable(space_idx, (LEFT)))
			{
				
				fnode = NULL;
				inode = NULL;
				if (NULL == (fnode = MALLOC(struct step_node, 1)))
				{
					fprintf(stderr, "malloc fnode error!\n");
					return;
				}
	
				if (NULL == (inode = MALLOC(struct Bheap_node, 1)))
				{
					fprintf(stderr, "malloc inode error!\n");
					return;
				}
				move_space(tmp_step, (LEFT));
				memcpy(fnode->step_status, tmp_step, sizeof(int) * 9);
				fnode->g = omnode->g + 1;
				fnode->h = calc_distance(tmp_step, end);
				fnode->f = fnode->g + fnode->h;
				fnode->parent = omnode;
				inode->value = fnode;
				/* 不在open heap和closed heap 中 */
				if (-1 == is_Bheap_contain(o_heap, inode, _eq) 
					&& -1 == is_Bheap_contain(c_heap, inode, _eq))
				{
					Bheap_push(o_heap, inode, _comp);
					if (is_end(tmp_step, end))
						continue;
				}
				else if(-1 != (idx = is_Bheap_contain(o_heap, inode, _eq)))
				{
					/* 在open heap 中*/
					if (NULL != (exist_node = Bheap_get(o_heap, idx)))
					{
						if (fnode->f < ((struct step_node *)exist_node->value)->f)
						{
							((struct step_node *)(exist_node->value))->f = fnode->f;
							((struct step_node*)(exist_node->value))->parent = fnode->parent;
						}
					}
					free(fnode);
					free(inode);
				}
				else
				{
					free(fnode);
					free(inode);
				}
				
				Bheap_push(c_heap, onode, _comp);
			}
			
			memset(tmp_step, 0x00, sizeof(int) * 9);
			memcpy(tmp_step, omnode->step_status, sizeof(int) * 9);
			space_idx = arr_idx(0, tmp_step);
			/* 右 */
			if (is_reachable(space_idx, (RIGHT)))
			{
				
				fnode = NULL;
				inode = NULL;
				if (NULL == (fnode = MALLOC(struct step_node, 1)))
				{
					fprintf(stderr, "malloc fnode error!\n");
					return;
				}
	
				if (NULL == (inode = MALLOC(struct Bheap_node, 1)))
				{
					fprintf(stderr, "malloc inode error!\n");
					return;
				}
				move_space(tmp_step, (RIGHT));
				memcpy(fnode->step_status, tmp_step, sizeof(int) * 9);
				fnode->g = omnode->g + 1;
				fnode->h = calc_distance(tmp_step, end);
				fnode->f = fnode->g + fnode->h;
				fnode->parent = omnode;
				inode->value = fnode;
				/* 不在open heap和closed heap 中 */
				if (-1 == is_Bheap_contain(o_heap, inode, _eq) 
					&& -1 == is_Bheap_contain(c_heap, inode, _eq))
				{
					Bheap_push(o_heap, inode, _comp);
					if (is_end(tmp_step, end))
						continue;
				}
				else if(-1 != (idx = is_Bheap_contain(o_heap, inode, _eq)))
				{
					/* 在open heap 中*/
					if (NULL != (exist_node = Bheap_get(o_heap, idx)))
					{
						if (fnode->f < ((struct step_node *)exist_node->value)->f)
						{
							((struct step_node *)(exist_node->value))->f = fnode->f;
							((struct step_node*)(exist_node->value))->parent = fnode->parent;
						}
					}
					free(fnode);
					free(inode);
				}
				else
				{
					free(fnode);
					free(inode);
				}
				Bheap_push(c_heap, onode, _comp);
			}
		}
	}
	
	if (NULL == omnode)
	{
		printf("not found!\n");
	}
	else
	{
		while(NULL != omnode)
		{
			print_step_status(omnode->step_status);
			printf("\n");
			omnode = omnode->parent;
		}
			
	}
	
}

static int arr_idx(int n, int step_status[9])
{
	int cnt;
	for(cnt = 0; cnt < 9; cnt++)
		if (n == step_status[cnt])
			return cnt;
	
	return (-1);
}

static void init_node_distance(int node_distance[9][9])
{
	int i, j, val;
	
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			val = abs(i - j);
			node_distance[i][j] = (int)val/3 + val%3;
		}
	}
}

static int calc_distance(int step_status[9], int end[9])
{
	int i, tmp, ret_val = 0;
	for (i = 0; i < 9; i++)
		if (-1 != (tmp = arr_idx(step_status[i], end)))
			ret_val += node_distance[i][tmp];
	
	return (ret_val);
}

static int is_end(int step_status[9], int end[9])
{
	int i;
	for (i = 0; i < 9; i++)
		if (step_status[i] != end[i])
			return (0);
	
	return (1);
}

static int is_reachable(int space_idx, int flag)
{
	int x,y;
	
	x = space_idx/3;
	y = space_idx%3;
	
	if(((UP) == flag) && (x > 0))
		return (1);
	else if (((DOWN) == flag) && (x < 2))
		return (1);
	else if (((LEFT) == flag) && (y > 0))
		return (1);
	else if(((RIGHT) == flag) && (y < 2))
		return (1);
	else
		return (0);
}

static int move_space(int step_status[9], int flag)
{
	int zero_idx, tmp;
	zero_idx = arr_idx(0, step_status);
	tmp = step_status[zero_idx];
	
	/* 交换空格的位置 */
	if (((UP) == flag) || ((DOWN) == flag) || ((LEFT) == flag) || ((RIGHT) == flag))
	{
		step_status[zero_idx] = step_status[zero_idx + flag];
		step_status[zero_idx + flag] = tmp;

		return (0);
	}
	
	return (-1);
}

static void print_step_status(int step_status[9])
{
	int i;
	for(i = 0; i < 9; i++)
	{
		printf("%d ", step_status[i]);
		if (0 == ((i+1)%3) && (i != 0))
			printf("\n");
	}
}


/* Bheap_compare_t 函数实现 */
int _comp(struct Bheap_node* n1, struct Bheap_node* n2)
{
	struct step_node *sn1 = NULL, *sn2 = NULL;

	if ((NULL != n1) && (NULL != n2))
	{
		sn1 = (struct step_node*)n1->value;
		sn2 = (struct step_node*)n2->value;

		if (sn1->f > sn2->f)
			return (1);
		else if(sn1->f == sn2->f)
			return (0);
		else
			return (-1);
	}
	else
		return (0);
}

/* Bheap_equal_t 函数实现 */
int _eq(struct Bheap_node* n1, struct Bheap_node* n2)
{
	struct step_node *sn1 = NULL, *sn2 = NULL;

	if ((NULL != n1) && (NULL != n2))
	{
		sn1 = (struct step_node*)(n1->value);
		sn2 = (struct step_node*)(n2->value);
		return calc_distance(sn1->step_status, sn2->step_status);
	}
	else
		return (0);
}
