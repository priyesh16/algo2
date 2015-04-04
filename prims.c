/*
 * Datastructure is a graph : adjacency list
 *	-> initialize a graph
 *		-> A node contains vertex info
 *						-> next pointer 
 *						-> edge cost
 *		-> list of above nodes make the adjacency list with head pointer being
 *		the vertex under consideration
 *		-> Graph is an array of all the adjacency lists.
 *			-> it contains total number of vertices also.
 *	-> add edges to vertex 
 *	-> print graph
 *
 * Prim's algo
 *  -> while all vertex are explored.
 *		-> pick any vertex and add to explored set
 *		-> find the lowest cost next vertex
 *		-> add that vertex to explored set; 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LIMITER " "
#define LINELENGTH 20


typedef struct node_s {
	int vertex;
	int cost;
	int explored;
	struct node_s *next;
} node_t;

typedef struct list_s {
	node_t *head;
} list_t;

typedef struct graph_s {
	int tot_vertex;
	list_t *adj_list;
} graph_t;
	
char *readfile(char *filename, char ***arr, int *vertices, int *edges); 
node_t *createvertex(int vertex, int cost); 
void printgraph(graph_t *g); 
void addedge(graph_t *graph, int vertex1, int vertex2, int cost); 
graph_t *graphinit(int lines); 
void readline(char **linesarr, int line, int *vertex1, int *vertex2, int *cost); 
node_t *findminkey(node_t *head);
node_t *findprims(graph_t *g);
void printmst(node_t *head); 

int main(int argc , char *argv[]) {
	char    *filename;
	int     vertices, edges;
	char	**linesarr;
	int		i;
	graph_t	*g;
	int vertex1, vertex2, cost;
	node_t *mst = NULL;

	if (argc != 2) {
		printf("Too few or many arguments \n");
		exit(EXIT_FAILURE);
	}

	filename = argv[1];

	readfile(filename, &linesarr, &vertices, &edges);

	printf("vertces %d : %d \n", vertices, edges); 

	for(i = 0; i < edges; i++)
		printf("mainarr %d : %s", i, linesarr[i]);
	
	g = graphinit(vertices);

	for(i = 0; i < edges; i++) {
		readline(linesarr, i, &vertex1, &vertex2, &cost);
		addedge(g, vertex1, vertex2, cost);
	}
	printgraph(g);
	mst = findprims(g);
	printf("mst %p \n ", mst);
	printmst(mst);
	printf(" \n \n ");
}

node_t *findminkey(node_t *head) {
	node_t *tmpnode = head;
	int		mincost;
		 
	while (tmpnode != NULL) {
		mincost = tmpnode->cost;
		tmpnode = tmpnode->next;
		if (tmpnode != NULL && tmpnode->explored == 0 && tmpnode->cost < mincost) {
		   mincost = tmpnode->cost;
		}
	}

	return tmpnode;
}

node_t *findprims(graph_t *g) {
	node_t *mst = NULL;
	int i = 0;
	node_t *tmpnode = NULL;
	node_t *savednode = NULL;
	int	exploredcount = 0;
	node_t *msttmpnode = NULL;
	int		minedge;
	int		index;

	mst = (node_t *)malloc(g->tot_vertex * sizeof(node_t));
	printf("mst %p \n ", mst);

	/* Start with any vertex (here first vertex) */
	mst = g->adj_list[1].head;

	while (exploredcount == g->tot_vertex) {
		index = 0;	
		msttmpnode = mst;
		/* Get minimum of adjacent lists */
		for(i = 0; i < exploredcount; i++) {
			tmpnode = findminkey(msttmpnode);

			/* Among min. adjacent lists get min edge */
			if (i == 0) {
				minedge = tmpnode->cost;
				savednode = tmpnode;
			}
			else if (tmpnode->cost < minedge) {
				minedge = tmpnode->cost;
				savednode = tmpnode;
			}
			msttmpnode	= msttmpnode->next;
		}
		/* Add the found min node to end of mst list */
		msttmpnode->next = savednode;
		exploredcount++;		
	}

	return mst;
}

void printmst(node_t *head) {
   node_t *tmpnode = head;

   while (tmpnode != NULL) {
	  printf("vertex %d : cost %d \n", tmpnode->vertex, tmpnode->cost);
	  tmpnode = tmpnode->next;
	}
}

graph_t *graphinit(int lines) {
	int i;
	graph_t *graph = (graph_t *) malloc(sizeof(graph_t));

	graph->tot_vertex = lines;
	graph->adj_list = (list_t *) malloc(sizeof(list_t) * lines);

	for (i = 0; i < lines; ++i) {
	   graph->adj_list[i].head = NULL;
	}

	return graph;
}

void printgraph(graph_t *g) {
   int i;
   node_t *tmpnode;
   
   for (i = 0; i < g->tot_vertex; i++) {
		tmpnode = g->adj_list[i].head;
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
	destvertex->next = graph->adj_list[vertex2].head;
	graph->adj_list[vertex2].head = destvertex;

	/* Since the graph is undirected, add edge from dest to source also */

	srcvertex = createvertex(vertex2, cost);
	srcvertex->next = graph->adj_list[vertex1].head;
	graph->adj_list[vertex1].head = srcvertex;
}

node_t *createvertex(int vertex, int cost) {
   node_t *newnode = (node_t *) malloc(sizeof(node_t));

   newnode->vertex	 = vertex;
   newnode->cost	 = cost;
   newnode->explored = 0;
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

