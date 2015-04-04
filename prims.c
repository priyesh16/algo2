/*
 * Datastructure is a graph : adjacency list
 *	-> initialize a graph
 *		-> A node contains vertex info
 *						-> next pointer 
 *						-> edge cost
 *		-> list of above nodes make the adjacency list 
 *		-> Graph is an array of all the adjacency lists.
 *			-> it contains total number of vertices also.
 *	-> add edges to vertex 
 *	-> print graph
 *
 * Prim's algo
 *	-> initialize a MST as an array, with size equal to number of vertices
 *  -> while all vertces are explored.
 *		-> find minimum edge from an explored vertex in the graph's adjacency 
 *			list to an unexplored vertex  
 *		-> find the minimum edge amongst all such adjacency lists.
 *		-> add that vertex to explored set; 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define LIMITER " "
#define LINELENGTH 20


typedef struct node_s {
	int vertex;
	int cost;
	struct node_s *next;
} node_t;

typedef struct list_s {
	node_t *head;
} list_t;

typedef struct graph_s {
	int tot_vertex;
	list_t *adjlist;
} graph_t;
	
char *readfile(char *filename, char ***arr, int *vertices, int *edges); 
node_t *createvertex(int vertex, int cost); 
void printgraph(graph_t *g); 
void addedge(graph_t *graph, int vertex1, int vertex2, int cost); 
graph_t *graphinit(int lines); 
void readline(char **linesarr, int line, int *vertex1, int *vertex2, int *cost); 
node_t *findminkey(node_t **mst, node_t *head, int exploredcount);
node_t **findprims(graph_t *g);
void printmst(node_t **mst, int vertices); 
void addinitvertex(node_t **head); 

int main(int argc , char *argv[]) {
	char    *filename;
	int     vertices, edges;
	char	**linesarr;
	int		i;
	graph_t	*g;
	int vertex1, vertex2, cost;
	node_t **mst = NULL;

	if (argc != 2) {
		printf("Too few or many arguments \n");
		exit(EXIT_FAILURE);
	}

	filename = argv[1];

	readfile(filename, &linesarr, &vertices, &edges);

	printf("vertices %d : %d \n", vertices, edges); 

	//for(i = 0; i < edges; i++)
	//	printf("mainarr %d : %s", i, linesarr[i]);
	
	g = graphinit(vertices);

	for(i = 0; i < edges; i++) {
		readline(linesarr, i, &vertex1, &vertex2, &cost);
		addedge(g, vertex1, vertex2, cost);
	}

	addinitvertex(&g->adjlist[0].head);

	printf(" \n ---------------------  \n ");
	printgraph(g);
	printf(" \n ---------------------  \n ");
	mst = findprims(g);
	printmst(mst, vertices);
	printf(" \n ");
}

node_t *findminkey(node_t **mst, node_t *head, int exploredcount) {
	node_t *tmpnode = head;
	node_t *savednode = NULL;
	int		mincost = INT_MAX;
	int		found = 0;
	int		i;
		 
	while (tmpnode != NULL) {
		found = 0;
		/* if node already explored then continue */
		printf("\t\t checking if vertex %d (%d) explored ", tmpnode->vertex, tmpnode->cost);
		for (i = 0; i < exploredcount; i++) {
			if (tmpnode->vertex == mst[i]->vertex) {
				found = 1;
				break;
			}
		}
		if (found == 1) {
			printf("--> yes \n ");
			tmpnode = tmpnode->next;
			continue;
		}
		else
			printf("--> no \n ");

		/* figure out min edge of the adjacent list */
		if (tmpnode->cost < mincost) {
			mincost = tmpnode->cost;
			savednode = tmpnode;
		}
		tmpnode = tmpnode->next;
	}

	if (savednode != NULL)
		printf("\t\tleast found in adjacent list vertex %d (%d) \n ", savednode->vertex, savednode->cost);
	return savednode;
}

node_t **findprims(graph_t *g) {
	node_t **mst = NULL;
	int i = 0;
	node_t *tmpnode = NULL;
	node_t *savednode;
	int	exploredcount = 0;
	int		minedge;
	int		index;
	int		vertex;

	mst = (node_t **)calloc(1, g->tot_vertex * sizeof(node_t *));
	mst[0] = g->adjlist[0].head;
	exploredcount++;

	while (exploredcount != g->tot_vertex) {
		minedge = INT_MAX;
		savednode = NULL;

		/* Get minimum of adjacent lists */
		for(i = 0; i < exploredcount; i++) {
			vertex = mst[i]->vertex;
			printf("\tfinding min for vertex %d \n ", vertex);
			tmpnode = findminkey(mst, g->adjlist[vertex].head, exploredcount);

			if (tmpnode == NULL)
				continue;

			if (tmpnode->cost < minedge) {
				minedge = tmpnode->cost;
				savednode = tmpnode;
			}
		}
		/* Add the found min node to end of mst list and mark explored */
		printf("least found amongst list vertex %d (%d) \n ", savednode->vertex, savednode->cost);
		mst[exploredcount++] = savednode;
	}

	return mst;
}

void addinitvertex(node_t **head) {
	/* Init vertex is used as a dummy vertex for the the graph just for
	 * easy implementation. The first vertex with an edge zero (to itself) is
	 * the init vertex. Since the graph vertex starts with 1, the zeroth array
	 * is empty to fill this vertex. Also mst needs to start with a vertex, the
	 * first explored vertex is the init vertex. 
	 */
	node_t *initvertex = (node_t *) malloc(sizeof(node_t)); 
   
	initvertex->vertex = 1;
	/* findminkey should never get the cost of this vertex */
	//initvertex.explored =  1;
	*head = initvertex; 

	return;
}
	
void printmst(node_t **mst, int vertices) {
	int i = 0;
	long long int sum = 0;

	for (i = 0; i < vertices; i++) {
		printf(" vertex %d (%d) \n", mst[i]->vertex, mst[i]->cost);
		sum = sum + mst[i]->cost;
	}
	
	printf("\n overall cost of MST is %lld \n", sum);
}

graph_t *graphinit(int lines) {
	int i;
	graph_t *graph = (graph_t *) malloc(sizeof(graph_t));

	graph->tot_vertex = lines;
	graph->adjlist = (list_t *) malloc(sizeof(list_t) * (lines + 1));

	for (i = 0; i < lines; ++i) {
	   graph->adjlist[i].head = NULL;
	}

	return graph;
}

void printgraph(graph_t *g) {
   int i;
   node_t *tmpnode;
   
   for (i = 0; i <= g->tot_vertex; i++) {
		tmpnode = g->adjlist[i].head;
		printf("\n adjacent list for %d \n", i);
		while(tmpnode != NULL) {
			printf(" -> %d : %d ", tmpnode->vertex, tmpnode->cost);
			tmpnode = tmpnode->next;
		}
   }
}


void readline(char **linesarr, int line, int *vertex1, int *vertex2, int *cost) {
	char *str = (char *)malloc(sizeof(char) * LINELENGTH);

	strcpy(str, linesarr[line]);
	*vertex1 = atoi(strtok(str, LIMITER));
	*vertex2 = atoi(strtok(NULL, LIMITER));
	*cost	 = atoi(strtok(NULL, LIMITER));
}


void addedge(graph_t *graph, int vertex1, int vertex2, int cost) {
	node_t *destvertex, *srcvertex;
	destvertex = createvertex(vertex1, cost);
	destvertex->next = graph->adjlist[vertex2].head;
	graph->adjlist[vertex2].head = destvertex;

	/* Since the graph is undirected, add edge from dest to source also */

	srcvertex = createvertex(vertex2, cost);
	srcvertex->next = graph->adjlist[vertex1].head;
	graph->adjlist[vertex1].head = srcvertex;
}

node_t *createvertex(int vertex, int cost) {
   node_t *newnode = (node_t *) malloc(sizeof(node_t));

   newnode->vertex	 = vertex;
   newnode->cost	 = cost;
   newnode->next	 = NULL;

   return newnode;
}

char *readfile(char *filename, char ***arr, int *vertices, int *edges) {
	FILE    *fp;
	char    *lineptr = NULL;
	size_t  n = 0;
	int     i = 0;
	char    **mainarr;

	fp = fopen(filename, "r");
	if (fp == NULL)
		printf("file couldn't be opened for reading");

	if (getline(&lineptr, &n, fp) != -1) {
		*vertices = atoi(strtok(lineptr, LIMITER));
		*edges	= atoi(strtok(NULL, LIMITER));
		printf("Total data to be read is %d \n", *edges);
	}

	mainarr = (char **) malloc(sizeof(char *) * *edges);

	while (getline(&lineptr, &n, fp) != -1) {
		mainarr[i] = (char *) malloc(sizeof(char ) * LINELENGTH);
		strcpy(mainarr[i], lineptr);
		i++;
	}

	*arr = mainarr;
}

