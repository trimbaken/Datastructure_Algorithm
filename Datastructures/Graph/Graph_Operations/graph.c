#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

#define MAX_NODES 1000
#define MAX_EDGES 499500
#define INT_MAX 1000000

typedef struct adjacency_list_node_s
{
        int adjacency_node_value;
	int weight;
	int is_active;
        struct adjacency_list_node_s* next;
} adjacency_list_node_t;

typedef struct graph_node_s
{
        int node_value;
        struct adjacency_list_node_s* adjacency_list;
}graph_node_t;

typedef struct stack_s
{
	int stack_node_value;
	int src_node_value;
	int weight;
}stack_t;

typedef struct queue_s
{
	int queue_node_value;
	struct queue_s* next;
	struct queue_s* prev;
}queue_t;

typedef struct tree_s
{
	int graph_node_value;
	struct tree_s* parent;
}tree_t;

graph_node_t graph_node[MAX_NODES];
adjacency_list_node_t adjacency_list_node[MAX_EDGES];
stack_t stack_dfs[MAX_EDGES];
queue_t queue_bfs[MAX_EDGES];
tree_t mst_tree[MAX_NODES];

int graph_node_count = 0;
int adjacency_list_node_count = -1;
int stack_dfs_top =0;
int queue_bfs_count =0;
queue_t* queue_bfs_front = NULL;
queue_t* queue_bfs_end = NULL;

void initialize_graph()
{
	int i =0;
	graph_node_count =0;
	for(i =0; i<MAX_NODES; i++)
	{
		graph_node[i].node_value = -1;
	}
	for(i=0; i<MAX_EDGES; i++)
	{
		adjacency_list_node[i].adjacency_node_value = -1;
	}

}

adjacency_list_node_t* create_adjacency_list_node(int adjacency_node_value, int weight)
{
	if(adjacency_list_node_count >= MAX_EDGES)
		return NULL;
	adjacency_list_node_count++;
	adjacency_list_node[adjacency_list_node_count].adjacency_node_value = adjacency_node_value;
	adjacency_list_node[adjacency_list_node_count].weight = weight;
	adjacency_list_node[adjacency_list_node_count].next = NULL;
	adjacency_list_node[adjacency_list_node_count].is_active = 1;

	return &adjacency_list_node[adjacency_list_node_count];
}

int insert_edge(int start_node, int end_node, int weight)
{
	printf("\n Insert Edge start_node : %d, end_node : %d, weight : %d", start_node, end_node, weight);

	if(graph_node[start_node].node_value == -1 || graph_node[start_node].adjacency_list == NULL)
	{
		graph_node[start_node].node_value = start_node;
		graph_node[start_node].adjacency_list = create_adjacency_list_node(end_node, weight);
		graph_node_count++;
	}
	else
	{
		adjacency_list_node_t *list_node = graph_node[start_node].adjacency_list;
		while(list_node->next != NULL)
		{
			if(list_node->adjacency_node_value == end_node)
			{
				printf("Edge present ");
				return 0;
			}

			list_node = list_node->next;
		}
		if(list_node->adjacency_node_value == end_node)
		{
			printf("Edge present ");
			return 0;
		}
		list_node->next = create_adjacency_list_node(end_node, weight);
	}
	graph_node[end_node].node_value = end_node;
	return 1;
}

int serach_node(int node_index)
{
	if(graph_node[node_index].node_value != -1)
	{
		return 1;
	}
	return 0;
}

int delete_node(int node_index)
{
	if(!serach_node(node_index))
	{
		printf("\n Node [%d] is not present  \n", node_index);
		return 0;
	}
	else
	{
		graph_node[node_index].node_value = -1;
		graph_node[node_index].adjacency_list = NULL;
		graph_node_count--;
	}
	return 1;
}

int search_edge(int start_node, int end_node, int weight)
{
	adjacency_list_node_t* list_node;
	if(!serach_node(start_node))
	{
		printf("\nNode not present \n");
		return 0;
	}
	list_node = graph_node[start_node].adjacency_list;
	while(list_node != NULL)
	{
		if(list_node->adjacency_node_value == end_node && list_node->weight == weight)
		{
			return 1;
		}
		list_node = list_node->next;
	}
	return 0;
}

int delete_edge(int start_node, int end_node, int weight)
{
	adjacency_list_node_t* list_node;
	if(!serach_node(start_node))
	{
		printf("\nNode not present \n");
		return 0;
	}
	list_node = graph_node[start_node].adjacency_list;
	// Delete first edge
	if(list_node->adjacency_node_value == end_node && list_node->weight == weight)
	{
		graph_node[start_node].adjacency_list = list_node->next;
		return 1;
	}
	adjacency_list_node_t* previous_node = list_node;
	list_node = list_node->next;
	while(list_node != NULL)
	{
		if(list_node->adjacency_node_value == end_node && list_node->weight == weight)
		{
			previous_node->next = list_node->next;
			return 1;
		}
		previous_node = list_node;
		list_node = list_node->next;
	}
	return 0;
}

int modify_edge(int start_node, int end_node, int weight, int new_weight)
{
	adjacency_list_node_t* list_node;
	if(!serach_node(start_node))
	{
		printf("\nNode not present \n");
		return 0;
	}
	list_node = graph_node[start_node].adjacency_list;
	while(list_node != NULL)
	{
		if(list_node->adjacency_node_value == end_node && list_node->weight == weight)
		{
			list_node->weight = new_weight;
			return 1;
		}
		list_node = list_node->next;
	}
	return 0;
}

void print_graph_nodes()
{
	int number_of_nodes = 0;
	int i =0;
	printf("\nAll graph nodes :\n");
	for(i =0; (i<MAX_NODES) && (number_of_nodes < graph_node_count); i++)
	{
		if(graph_node[i].node_value != -1)
		{
			number_of_nodes++;
			printf(" Node : %d \n", graph_node[i].node_value);
		}
	}
}

void insert_bi_directional_edge(int start_node, int end_node, int weight)
{
	if(!insert_edge(start_node, end_node, weight))
	{
		printf("\nFail to insert edge\n");
	}
	if(!insert_edge(end_node, start_node, weight))
	{
		printf("\nFail to insert edge\n");
	}
}

void create_graph()
{
	initialize_graph();
	insert_bi_directional_edge(0, 1, 4);
	insert_bi_directional_edge(0, 7, 8);
	insert_bi_directional_edge(1, 2, 8);
	insert_bi_directional_edge(1, 7, 11);
	insert_bi_directional_edge(2, 3, 7);
	insert_bi_directional_edge(2, 8, 2);
	insert_bi_directional_edge(2, 5, 4);
	insert_bi_directional_edge(3, 4, 9);
	insert_bi_directional_edge(3, 5, 14);
	insert_bi_directional_edge(5, 6, 2);
	insert_bi_directional_edge(6, 8, 6);
	insert_bi_directional_edge(6, 7, 1);
	insert_bi_directional_edge(7, 8, 7);
	insert_bi_directional_edge(5, 4, 10);
}

int is_stack_dfs_empty()
{
	if(stack_dfs_top == -1)
		return 1;
	return 0;
}

int stack_dfs_push(int graph_node)
{
//	printf("\n DFS Traverser Node [%d]", graph_node);
	stack_dfs_top++;
	stack_dfs[stack_dfs_top].stack_node_value = graph_node;
	return 1;
}

int stack_dfs_pop()
{
	int ret =-1;
	if(is_stack_dfs_empty())
	{
		printf("\nDFS Stack is Empty \n");
		return ret;
	}
	ret = stack_dfs[stack_dfs_top].stack_node_value;
	stack_dfs_top--;
	return ret;
}

int get_stack_dfs_top_node()
{
	if(is_stack_dfs_empty())
	{
		printf("\nDFS Stack is Empty \n");
		return -1;
	}
	return (stack_dfs[stack_dfs_top].stack_node_value);

}

int dfs(int start_node)
{
	int visited[MAX_NODES];
	int i =0;
	int pop_current_node =0;
	int current_node =0;
	adjacency_list_node_t* list_node = NULL;
	stack_dfs_top = -1;
	if(!serach_node(start_node))
	{
		printf("\n Node not present  \n");
		return 0;
	}
	for(i =0; i<MAX_NODES; i++)
	{
		visited[i] = 0;
	}
	if(stack_dfs_push(start_node) == -1)
	{
		printf("\n Fail to push into stack\n");
		return 0;
	}
	visited[start_node] =1;
	while(!is_stack_dfs_empty())
	{
		current_node = get_stack_dfs_top_node();
		pop_current_node = 1;
		list_node = graph_node[current_node].adjacency_list;
		while(list_node != NULL)
		{
			if(visited[list_node->adjacency_node_value] == 0)
			{
				if(stack_dfs_push(list_node->adjacency_node_value) == -1)
				{
					printf("\nFail to push into stack\n");
					return 0;
				}
				pop_current_node = 0;
				visited[list_node->adjacency_node_value] =1;
				break;
			}
			list_node = list_node->next;
		}
		if(pop_current_node)
		{
			if(stack_dfs_pop() == -1)
			{
				printf("\nFail to pop from stack\n");
				return 0;
			}
		}
	}
	return 1;
}

int is_queue_bfs_empty()
{
	if(queue_bfs_front == NULL)
	{
		printf("\nBFS Queue is empty\n");
		return 1;
	}
	return 0;
}

queue_t* create_queue_bfs_node(int value)
{
	queue_bfs[queue_bfs_count].queue_node_value = value;
	queue_bfs[queue_bfs_count].next = NULL;
	queue_bfs[queue_bfs_count].prev = NULL;

	return &queue_bfs[queue_bfs_count++];
}

int queue_bfs_push(int value)
{
	if(queue_bfs_count >= MAX_NODES)
	{
		printf("\nQueue size is full\n");
		return 0;
	}
	if(queue_bfs_front == NULL)
	{
		queue_bfs_front = queue_bfs_end = create_queue_bfs_node(value);
	}
	else
	{
		queue_bfs_end->next = create_queue_bfs_node(value);
		queue_bfs_end = queue_bfs_end->next;
	}
	printf("\n BFS Traverser Node [%d]", value);
	return 1;
}

int queue_bfs_pop()
{
	int queue_element;
	if(is_queue_bfs_empty())
	{
		return -1;
	}
	queue_element = queue_bfs_front->queue_node_value;
	if(queue_bfs_front == queue_bfs_end)
	{
		queue_bfs_front = queue_bfs_end = NULL;
		return queue_element;
	}
	queue_bfs_front = queue_bfs_front->next;
	return queue_element;
}

int get_queue_bfs_front_element()
{
	if(is_queue_bfs_empty())
	{
		return -1;
	}
	return queue_bfs_front->queue_node_value;
}

int bfs(int start_node)
{
	int visited[MAX_NODES];
	int i =0;
	int pop_current_node =0;
	int current_node =0;
	queue_bfs_count = 0;
	queue_bfs_front = NULL;
	queue_bfs_end = NULL;
	adjacency_list_node_t* list_node = NULL;
	if(!serach_node(start_node))
	{
		printf("\n Node not present  \n");
		return 0;
	}
	for(i =0; i<MAX_NODES; i++)
	{
		visited[i] = 0;
	}
	if(queue_bfs_push(start_node) == 0)
	{
		printf("\n Fail to push into queue\n");
		return 0;
	}
	visited[start_node] =1;
	while(!is_queue_bfs_empty())
	{
		current_node = queue_bfs_pop();
		list_node = graph_node[current_node].adjacency_list;
		while(list_node != NULL)
		{
			if(visited[list_node->adjacency_node_value] == 0)
			{
				if(queue_bfs_push(list_node->adjacency_node_value) == 0)
				{
					printf("\nFail to push into queue\n");
					return 0;
				}
				visited[list_node->adjacency_node_value] =1;
			}
			list_node = list_node->next;
		}
	}
	return 1;
}

void create_directed_cyclic_graph()
{
	initialize_graph();
	if(!insert_edge(0, 1, 4))
        {
                printf("\nFail to insert edge\n");
        }
        if(!insert_edge(1, 2, 5))
        {
                printf("\nFail to insert edge\n");
        }
	if(!insert_edge(2, 3, 2))
        {
                printf("\nFail to insert edge\n");
        }
        if(!insert_edge(3, 0, 3))
        {
                printf("\nFail to insert edge\n");
        }
}

int is_node_present_in_dfs_stack(int node)
{
	int i =0;
	for(i =0; i<= stack_dfs_top; i++)
	{
		if(stack_dfs[i].stack_node_value == node)
		{
			return 1;
		}
	}
	return 0;
}

int find_cycle_in_graph(int start_node)
{
	if(!serach_node(start_node))
	{
		printf("\n Node not present");
		return -1;
	}
	int visited[MAX_NODES];
	int i =0;
	int pop_current_node =0;
	int current_node =0;
	adjacency_list_node_t* list_node = NULL;
	stack_dfs_top = -1;
	for(i =0; i<MAX_NODES; i++)
	{
		visited[i] = 0;
	}
	if(stack_dfs_push(start_node) == -1)
	{
		printf("\n Fail to push into stack\n");
		return -1;
	}
	visited[start_node] =1;
	while(!is_stack_dfs_empty())
	{
		current_node = get_stack_dfs_top_node();
		pop_current_node = 1;
		list_node = graph_node[current_node].adjacency_list;
		while(list_node != NULL)
		{
			if(visited[list_node->adjacency_node_value] == 0)
			{
				if(stack_dfs_push(list_node->adjacency_node_value) == -1)
				{
					printf("\nFail to push into stack\n");
					return -1;
				}
				pop_current_node = 0;
				visited[list_node->adjacency_node_value] =1;
				break;
			}
			else if (is_node_present_in_dfs_stack(list_node->adjacency_node_value))
			{
				return 1;
			}
			list_node = list_node->next;
		}
		if(pop_current_node)
		{
			if(stack_dfs_pop() == -1)
			{
				printf("\nFail to pop from stack\n");
				return -1;
			}
		}
	}
	return 0;

}

void create_graph_with_connected_components()
{
	initialize_graph();
	insert_bi_directional_edge(0, 1, 4);
	insert_bi_directional_edge(1, 2, 8);
	insert_bi_directional_edge(3, 4, 1);
}

void create_graph_with_bridge()
{
	initialize_graph();
	insert_bi_directional_edge(0, 1, 4);
	insert_bi_directional_edge(1, 2, 8);
	insert_bi_directional_edge(2, 0, 1);
	insert_bi_directional_edge(1, 3, 5);
}


int connected_component_dfs(int start_node, int visited[MAX_NODES])
{
	int i =0;
	int pop_current_node =0;
	int current_node =0;
	adjacency_list_node_t* list_node = NULL;
	stack_dfs_top = -1;
	if(!serach_node(start_node))
	{
		printf("\n Node not present  \n");
		return 0;
	}
	if(stack_dfs_push(start_node) == -1)
	{
		printf("\n Fail to push into stack\n");
		return 0;
	}
	visited[start_node] =1;
	while(!is_stack_dfs_empty())
	{
		current_node = get_stack_dfs_top_node();
		pop_current_node = 1;
		list_node = graph_node[current_node].adjacency_list;
		while(list_node != NULL)
		{
			if(visited[list_node->adjacency_node_value] == 0)
			{
				if(stack_dfs_push(list_node->adjacency_node_value) == -1)
				{
					printf("\nFail to push into stack\n");
					return 0;
				}
				pop_current_node = 0;
				visited[list_node->adjacency_node_value] =1;
				break;
			}
			list_node = list_node->next;
		}
		if(pop_current_node)
		{
			if(stack_dfs_pop() == -1)
			{
				printf("\nFail to pop from stack\n");
				return 0;
			}
		}
	}
	return 1;

}

int find_connected_components()
{
	int number_of_nodes = 0;
	int i =0;
	int visited[MAX_NODES];
	for(i=0; i<MAX_NODES; i++)
	{
		visited[i] = 0;
	}
	for(i =0; (i<MAX_NODES) && (number_of_nodes < graph_node_count); i++)
	{
		if(graph_node[i].node_value != -1)
		{
			number_of_nodes++;
			if(visited[graph_node[i].node_value] == 0)
			{
				printf("\nConnected Component :\n");
				if(!connected_component_dfs(graph_node[i].node_value, visited))
				{
					printf("\nfail to find connected component\n");
				}
			}
		}
	}
	return 1;
}

int find_shortest_path(int start_node)
{
	int visited[MAX_NODES];
	int distance[MAX_NODES];
	int i =0;
	int pop_current_node =0;
	int current_node =0;
	queue_bfs_count = 0;
	queue_bfs_front = NULL;
	queue_bfs_end = NULL;
	adjacency_list_node_t* list_node = NULL;
	if(!serach_node(start_node))
	{
		printf("\n Node not present  \n");
		return 0;
	}
	for(i =0; i<MAX_NODES; i++)
	{
		visited[i] = 0;
		distance[i] = INT_MAX;
	}
	if(queue_bfs_push(start_node) == 0)
	{
		printf("\n Fail to push into queue\n");
		return 0;
	}
	visited[start_node] =1;
	distance[start_node] = 0;
	while(!is_queue_bfs_empty())
	{
		current_node = queue_bfs_pop();
		list_node = graph_node[current_node].adjacency_list;
		while(list_node != NULL)
		{
			if(distance[list_node->adjacency_node_value] > distance[current_node] + list_node->weight)
			{
				distance[list_node->adjacency_node_value] = distance[current_node] + list_node->weight;
			}
			if(visited[list_node->adjacency_node_value] == 0)
			{
				if(queue_bfs_push(list_node->adjacency_node_value) == 0)
				{
					printf("\nFail to push into queue\n");
					return 0;
				}
				visited[list_node->adjacency_node_value] =1;
			}
			list_node = list_node->next;
		}
	}
	i =0;
	while(distance[i] != INT_MAX)
	{
		printf("\n Source Node [%d], Destination Node [%d], Shortest Distance [%d]", start_node, i, distance[i]);
		i++;
	}
	return 1;
}

int find_connected_nodes(int start_node, int end_node, int weight)
{
	int visited[MAX_NODES];
	int i =0;
	int pop_current_node =0;
	int current_node =0;
	int connected_nodes =0;
	adjacency_list_node_t* list_node = NULL;
	stack_dfs_top = -1;
	if(!serach_node(start_node))
	{
		printf("\n Node not present  \n");
		return 0;
	}
	for(i =0; i<MAX_NODES; i++)
	{
		visited[i] = 0;
	}
	if(stack_dfs_push(start_node) == -1)
	{
		printf("\n Fail to push into stack\n");
		return 0;
	}
	visited[start_node] =1;
	connected_nodes++;
	while(!is_stack_dfs_empty())
	{
		current_node = get_stack_dfs_top_node();
		pop_current_node = 1;
		list_node = graph_node[current_node].adjacency_list;
		while(list_node != NULL)
		{
			if(list_node->is_active ==1 && visited[list_node->adjacency_node_value] == 0)
			{
				if(stack_dfs_push(list_node->adjacency_node_value) == -1)
				{
					printf("\nFail to push into stack\n");
					return 0;
				}
				pop_current_node = 0;
				visited[list_node->adjacency_node_value] =1;
				connected_nodes++;
				break;
			}
			list_node = list_node->next;
		}
		if(pop_current_node)
		{
			if(stack_dfs_pop() == -1)
			{
				printf("\nFail to pop from stack\n");
				return 0;
			}
		}
	}
	if(connected_nodes < graph_node_count)
	{
		printf("\nBridge is present [%d][%d][%d]\n", start_node, end_node, weight);
	}
	else
	{
		printf("\nBridge is not present\n");
		return 0;
	}
	return 1;
}

void find_bridge()
{
	int number_of_nodes = 0;
	int i =0;
	adjacency_list_node_t* list_node;
	adjacency_list_node_t* list_node_temp;
	for(i =0; (i<MAX_NODES) && (number_of_nodes < graph_node_count); i++)
	{
		if(graph_node[i].node_value != -1)
		{
			list_node = graph_node[i].adjacency_list;
			while(list_node != NULL)
			{
				list_node->is_active = 0;
				find_connected_nodes(i, list_node->adjacency_node_value, list_node->weight);
				list_node->is_active = 1;
				list_node = list_node->next;
				number_of_nodes++;
			}
		}
	}
}

void get_minimum_edge(int *src_node, int *dest_node, int *weight)
{
	int number_of_nodes = 0;
	int i =0;
	int min_weight = INT_MAX;
	adjacency_list_node_t* list_node;
	adjacency_list_node_t* min_list_node =NULL;
	for(i =0; (i<MAX_NODES) && (number_of_nodes < graph_node_count); i++)
	{
		if(graph_node[i].node_value != -1)
		{
			list_node = graph_node[i].adjacency_list;
			while(list_node != NULL)
			{
				if(list_node->is_active && min_weight > list_node->weight)
				{
					min_weight = list_node->weight;
					*src_node = i;
					*dest_node = list_node->adjacency_node_value;
					*weight = list_node->weight;
					min_list_node = list_node;
				}
				list_node = list_node->next;
			}
			number_of_nodes++;
		}
	}
	min_list_node->is_active = 0;

}

int find_mst()
{
	int number_of_nodes = 0;
	int i =0;
	int src_node = -1;
	int dest_node = -1;
	int weight = -1;
	adjacency_list_node_t* list_node;
	tree_t* src_parent = NULL;
	tree_t* dest_parent = NULL;

	for(i =0; (i<MAX_NODES) && (number_of_nodes < graph_node_count); i++)
	{
		if(graph_node[i].node_value != -1)
		{
			mst_tree[i].graph_node_value = i;
			mst_tree[i].parent = &mst_tree[i];
			number_of_nodes++;
		}
	}
	number_of_nodes =0;
	for(i =0; (i<MAX_NODES) && (number_of_nodes < (graph_node_count-1)); i++)
	{
		get_minimum_edge(&src_node, &dest_node, &weight);
//		printf("\n Min Edge [%d][%d][%d]\n", src_node, dest_node, weight);
		src_parent = mst_tree[src_node].parent;
		while(src_parent != src_parent->parent)
		{
			src_parent = src_parent->parent;
		}
		dest_parent = mst_tree[dest_node].parent;
		while(dest_parent != dest_parent->parent)
		{
			dest_parent = dest_parent->parent;
		}
		if(src_parent != dest_parent)
		{
			dest_parent->parent = src_parent;
			printf("\nMST Edge [%d][%d][%d]\n", src_node, dest_node, weight);
			number_of_nodes++;
		}
//		printf("\n number_of_nodes [%d]\n", number_of_nodes);
	}
	return 1;
}

int is_graph_connected()
{
	int visited[MAX_NODES];
	int i =0;
	int pop_current_node =0;
	int current_node =0;
	int connected_nodes =0;
	int start_node =0;
	adjacency_list_node_t* list_node = NULL;
	stack_dfs_top = -1;
	if(!serach_node(start_node))
	{
		printf("\n Node not present  \n");
		return -1;
	}
	for(i =0; i<MAX_NODES; i++)
	{
		visited[i] = 0;
	}
	if(stack_dfs_push(start_node) == -1)
	{
		printf("\n Fail to push into stack\n");
		return -1;
	}
	visited[start_node] =1;
	connected_nodes++;
	while(!is_stack_dfs_empty())
	{
		current_node = get_stack_dfs_top_node();
		pop_current_node = 1;
		list_node = graph_node[current_node].adjacency_list;
		while(list_node != NULL)
		{
			if(list_node->is_active ==1 && visited[list_node->adjacency_node_value] == 0 && list_node->weight >0)
			{
				if(stack_dfs_push(list_node->adjacency_node_value) == -1)
				{
					printf("\nFail to push into stack\n");
					return -1;
				}
				pop_current_node = 0;
				visited[list_node->adjacency_node_value] =1;
				connected_nodes++;
				break;
			}
			list_node = list_node->next;
		}
		if(pop_current_node)
		{
			if(stack_dfs_pop() == -1)
			{
				printf("\nFail to pop from stack\n");
				return -1;
			}
		}
	}
	if(connected_nodes < graph_node_count)
	{
		return 0;
	}
	return 1;
}

void reduce_minimum_edge_weight(int weight)
{
	int number_of_nodes = 0;
	int i =0;
	int min_weight = INT_MAX;
	adjacency_list_node_t* list_node;
	for(i =0; (i<MAX_NODES) && (number_of_nodes < graph_node_count); i++)
	{
		if(graph_node[i].node_value != -1)
		{
			list_node = graph_node[i].adjacency_list;
			while(list_node != NULL)
			{
				if(list_node->is_active)
				{
					list_node->weight -= weight;
					if(list_node->weight <= 0)
					{
						list_node->is_active =0;
					}
				}
				list_node = list_node->next;
			}
			number_of_nodes++;
		}
	}
}

void create_graph_for_max_flow()
{
	int i =0;
	int count =0;
	initialize_graph();
	if(!insert_edge(0, 1, 16))
        {
                printf("\nFail to insert edge\n");
        }
        if(!insert_edge(0, 2, 13))
        {
                printf("\nFail to insert edge\n");
        }
        if(!insert_edge(1, 2, 10))
        {
                printf("\nFail to insert edge\n");
        }
	if(!insert_edge(1, 3, 12))
        {
                printf("\nFail to insert edge\n");
        }
        if(!insert_edge(2, 1, 4))
        {
                printf("\nFail to insert edge\n");
        }
        if(!insert_edge(2, 4, 14))
        {
                printf("\nFail to insert edge\n");
        }
        if(!insert_edge(3, 2, 9))
        {
                printf("\nFail to insert edge\n");
        }
        if(!insert_edge(3, 5, 20))
        {
                printf("\nFail to insert edge\n");
        }
        if(!insert_edge(4, 5, 4))
        {
                printf("\nFail to insert edge\n");
        }
        if(!insert_edge(4, 3, 7))
        {
                printf("\nFail to insert edge\n");
        }
	for(i =0; i<MAX_NODES; i++)
	{
		if(graph_node[i].node_value != -1)
		{
			count++;
		}
	}
	graph_node_count = count;
}


int stack_dfs_push_(int graph_node, int src_node_value, int weight)
{
	printf("\n DFS Traverser Node [%d]", graph_node);
	stack_dfs_top++;
	stack_dfs[stack_dfs_top].stack_node_value = graph_node;
	stack_dfs[stack_dfs_top].src_node_value = src_node_value;
	stack_dfs[stack_dfs_top].weight = weight;

	return 1;
}

int get_source_to_dest_path(int start_node, int end_node)
{
	int visited[MAX_NODES];
	int i =0;
	int pop_current_node =0;
	int current_node =0;
	adjacency_list_node_t* list_node = NULL;
	stack_dfs_top = -1;
	if(!serach_node(start_node))
	{
		printf("\n Node not present  \n");
		return -1;
	}
	for(i =0; i<MAX_NODES; i++)
	{
		visited[i] = 0;
	}
	if(stack_dfs_push_(start_node, start_node, 0) == -1)
	{
		printf("\n Fail to push into stack\n");
		return -1;
	}
	visited[start_node] =1;
	int dest_found =0;
	while(!is_stack_dfs_empty() && dest_found == 0)
	{
//		printf("\n dest_found [%d] \n", dest_found);
		current_node = get_stack_dfs_top_node();
		pop_current_node = 1;
		list_node = graph_node[current_node].adjacency_list;
		while(list_node != NULL)
		{
			if((list_node->weight > 0) && list_node->adjacency_node_value == end_node)
			{
				dest_found = 1;
				pop_current_node = 0;
				if(stack_dfs_push_(list_node->adjacency_node_value, current_node, list_node->weight) == -1)
				{
					printf("\nFail to push into stack\n");
					return -1;
				}
				break;
			}
			if((list_node->weight >0) && visited[list_node->adjacency_node_value] == 0)
			{
				if(stack_dfs_push_(list_node->adjacency_node_value, current_node, list_node->weight) == -1)
				{
					printf("\nFail to push into stack\n");
					return -1;
				}
				pop_current_node = 0;
				visited[list_node->adjacency_node_value] =1;
				break;
			}
			list_node = list_node->next;
		}
		if(pop_current_node)
		{
			if(stack_dfs_pop() == -1)
			{
				printf("\nFail to pop from stack\n");
				return -1;
			}
		}
	}
	int min_weight =INT_MAX;
	if(dest_found)
	{
		for(i =1; i<=stack_dfs_top; i++)
		{
			if(min_weight > stack_dfs[i].weight)
			{
				min_weight = stack_dfs[i].weight;
			}
			printf("\nEdge before Removing Min weight [%d][%d][%d]\n", stack_dfs[i].src_node_value, stack_dfs[i].stack_node_value, stack_dfs[i].weight);
		}
		printf("\n min_weight [%d], stack_dfs_top [%d]\n", min_weight, stack_dfs_top);
		for(i =1; i<=stack_dfs_top; i++)
		{
			modify_edge(stack_dfs[i].src_node_value, stack_dfs[i].stack_node_value, stack_dfs[i].weight, (stack_dfs[i].weight - min_weight));
		}
	}
	else
	{
		min_weight =0;
	}
	return min_weight;
}
int find_max_flow(int start_node, int end_node)
{
	int max_flow =0;
//	max_flow += get_source_to_dest_path(start_node, end_node);

	int ret = get_source_to_dest_path(start_node, end_node);
	while(ret != 0)
	{
		max_flow += ret;
		ret = get_source_to_dest_path(start_node, end_node);
	}

	printf("\nMax Flow is [%d]\n", max_flow);
	return 1;
}

void init()
{
	int operation =0;
	int start_node =0;
	int end_node =0;
	int weight =0;
	int new_weight =0;
	int node_index =0;

	int i =0;
	initialize_graph();
	while(1)
	{
		printf("\n\nSelect operation: \n");
		printf("1 Insert Edge: \n");
		printf("2 Search Node: \n");
		printf("3 Delete Node: \n");
		printf("4 Search Edge: \n");
		printf("5 Delete Edge: \n");
		printf("6 Modify Edge: \n");
		printf("7 Print All Nodes: \n");
		printf("8 Create Graph: \n");
		printf("9 Graph Traversal - DFS: \n");
		printf("10 Graph Traversal - BFS: \n");
		printf("11 Create Directed Cyclic Graph: \n");
		printf("12 Find Cycle In Graph: \n");
		printf("13 Create Graph with Connected Component\n");
		printf("14 Find Connected Component\n");
		printf("15 Shortest Path - Source to all node\n");
		printf("16 Create Graph with Bridge\n");
		printf("17 Find Bridge in Graph\n");
		printf("18 Find MST - Kruskal's Algo\n");
		printf("19 Create Graph for Max Flow\n");
		printf("20 Find Max Flow");
		printf("\n0 Exit: \n");

		printf("\nEnter Operation Number:\n");
		scanf("%d", &operation);
		if(operation < 0 || operation > 20)
		{
			system("clear");
			printf("\n Invalid operation selected  \n");
			continue;
		}
		system("clear");
		switch(operation)
		{
			case 1:
				{
					printf("\n Insert Edge  \n");
					printf("Enter Start Node, End Node, Weight of Edge \n" );
					scanf("%d %d %d", &start_node, &end_node, &weight);
					if(!insert_edge(start_node, end_node, weight))
					{
						printf("\nFail to create edge\n");
					}
					else
					{
						printf("\nEdge is created\n");
					}
					break;
				}
			case 2:
				{
					printf("\n Search Node  \n");
					printf("Enter Node Index\n" );
					scanf("%d", &node_index);
					if(!serach_node(node_index))
					{
						printf("\n Node [%d] is not present  \n", node_index);
					}
					else
					{
						printf("\n Node [%d] is present \n", node_index);
					}
					break;
				}
			case 3:
				{
					printf("\n Delete Node  \n");
					printf("\n Enter Node Index  \n");
					scanf("%d", &node_index);
					if(!delete_node(node_index))
					{
						printf("\nFail to delete node [%d]  \n", node_index);
					}
					else
					{
						printf("\nDeleted node [%d]\n", node_index);
					}

					break;
				}

			case 4:
				{
					printf("\n Search Edge  \n");
					printf("Enter Start Node, End Node, Weight of Edge \n" );
					scanf("%d %d %d", &start_node, &end_node, &weight);
					if(!search_edge(start_node, end_node, weight))
					{
						printf("\n Edge [%d][%d][%d] is not present  \n", start_node, end_node, weight);
					}
					else
					{
						printf("\n Edge [%d][%d][%d] is present  \n", start_node, end_node, weight);
					}
					break;
				}
			case 5:
				{
					printf("\n Delete Edge  \n");
					printf("Enter Start Node, End Node, Weight of Edge \n" );
					scanf("%d %d %d", &start_node, &end_node, &weight);
					if(!delete_edge(start_node, end_node, weight))
					{
						printf("\nFail to delete edge [%d][%d][%d]  \n", start_node, end_node, weight);
					}
					else
					{
						printf("\nDeleted edge [%d][%d][%d]  \n", start_node, end_node, weight);
					}

					break;
				}
			case 6:
				{
					printf("\n Modify Edge \n");
					printf("Enter Start Node, End Node, Weight of Edge, New Weight \n" );
					scanf("%d %d %d %d", &start_node, &end_node, &weight, &new_weight);
					if(!modify_edge(start_node, end_node, weight, new_weight))
					{
						printf("\nFail to modify edge [%d][%d][%d]  \n", start_node, end_node, weight);
					}
					else
					{
						printf("\nModified edge [%d][%d][%d]  \n", start_node, end_node, weight);
					}


					break;
				}
			case 7:
				{
					printf("\n Print All Graph Nodes  \n");
					print_graph_nodes();
					break;
				}
			case 8:
				{
					printf("\n Create Graph  \n");
					create_graph();
					break;
				}
			case 9:
				{
					printf("\n Graph Traversal - DFS  \n");
					printf("Enter start node :\n");
					scanf("%d", &start_node);
					if( !dfs(start_node))
					{
						printf("\nGraph traversal using DFS failed\n");
					}
					break;
				}
			case 10:
				{
					printf("\n Graph Traversal - BFS  \n");
					printf("Enter start node :\n");
					scanf("%d", &start_node);
					if(!bfs(start_node))
					{
						printf("\nGraph traversal using BFS failed\n");
					}
					break;
				}
			case 11:
				{
					printf("\n Create Directed Cyclic Graph  \n");
					create_directed_cyclic_graph();
					break;
				}
			case 12:
				{
					printf("\n Find Cycle In Graph  \n");
					printf("Enter Start Node Index\n" );
					scanf("%d", &start_node);
					int ret = find_cycle_in_graph(start_node);
					if(ret == 1)
					{
						printf("\n Cycle is present \n");
					}
					else if(ret == 0)
					{
						printf("\n Cycle is not present \n");
					}
					else
					{
						printf("\nFail to find cycle in graph\n");
					}
					break;
				}
			case 13:
				{
					printf("\nCreate Graph with Connected Components\n");
					create_graph_with_connected_components();
					break;
				}
			case 14:
				{
					printf("\nFind Connected Components of Graph\n");
					if(!find_connected_components())
					{
						printf("\nFail to find connected components\n");
					}
					break;
				}
			case 15:
				{
					printf("\nShortest Path - Source to all nodes \n");
					printf("\nEnter start node : ");
					scanf("%d",&start_node);
					if(!find_shortest_path(start_node))
					{
						printf("\nFail to find shortest path\n");
					}
					break;
				}
			case 16:
				{
					printf("\nCreate Graph with Bridge\n");
					create_graph_with_bridge();
					break;
				}
			case 17:
				{
					printf("\nFind Bridge in Graph\n");
					find_bridge();
					break;
				}
			case 18:
				{
					printf("\nFind MST using Kruskal's Algorithm \n");
					if(!find_mst())
					{
						printf("\nFail to find MST\n");
					}
					break;
				}
			case 19:
				{
					printf("\nCreate Graph for Max Flow\n");
					create_graph_for_max_flow();
					break;
				}
			case 20:
				{
					printf("\nFind Max Flow\n");
					printf("\nEnter source node and destination node\n");
					scanf("%d %d", &start_node, &end_node);
					if(!find_max_flow(start_node, end_node))
					{
						printf("\nFail to find max flow\n");
					}
					break;
				}
			case 0:
				{
					printf("\n Thanks for using our service:  \n");
					return;
				}
			default:
				break;
		}
	}
}


int main(void)
{
	printf("\n Program to execute all graph operations:\n");
	init();
	return false;
}
