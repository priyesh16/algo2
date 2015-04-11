/*
 * -> main 
 *	-> read file
 *		 -> function to read file line by line
 *			 -> get the total number of vertices and lines in the file.
 *			 -> read line by line and store in an array(mainarr);
 *	-> sort mainarr based on cost 
 *	-> cluster the data based on input k-spacing 
 *		-> initialize an array whose leader is itself
 *		-> while there are k clusters
 *			-> if leader of the vertices are same then continue
 *			-> else update leader of all members of the cluster with the same
 *			leader 
 *	-> compute maximum spacing
 *		-> since already the mainarr is sorted iterate over array 
 *		-> find vertices that have different leaders but smallest cost
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define LINELENGTH 20
#define LIMITER " "
#define MAXLINES 500

typedef struct node_s {
	int vertex1;
	int vertex2;
	int cost;
	int leader;
} node_t;

int cmpfunc(const void *a, const void *b);
char *readfile(char *filename, node_t ***arr, int *vertices, int *lines); 
void cluster(node_t **mainarr, int **clusterarr, int vertices, int clustersize); 
int maxspacing(node_t **mainarr, int *clusterarr, int lines); 

int main(int argc , char *argv[]) {
	char	*filename;
	node_t	**mainarr;
	int		i = 0;
	int		lines;
	int		vertices;
	int		spacing;
	int		*clusterarr;
	int		space;

	if (argc != 3) {
		printf("Too few or many arguments \n");
		exit(EXIT_FAILURE);
	}

	filename = argv[2];
	space	= atoi(argv[1]);

	readfile(filename, &mainarr, &vertices, &lines);

	qsort((void *)mainarr, lines, sizeof(char *), cmpfunc); 

	while (mainarr[i] != 0) {
		printf("mainarr %d = %d %d %d \n", i , mainarr[i]->vertex1, mainarr[i]->vertex2, mainarr[i]->cost);
		i++;
	}

	cluster(mainarr, &clusterarr, vertices, space);
	spacing	= maxspacing(mainarr, clusterarr, lines);
	printf("max spacing is %d \n", spacing);
}

int maxspacing(node_t **mainarr, int *clusterarr, int lines) {
	int	i = 0;
	int spacing = 0;
	int vertex1, vertex2;
	node_t	*tmpnode;

	while (i != lines) {
		tmpnode = mainarr[i];
		vertex1 = tmpnode->vertex1;
		vertex2 = tmpnode->vertex2;
		if (clusterarr[vertex1] == clusterarr[vertex2]){
		   i++;
		   continue;
		}
		if (spacing == 0 || tmpnode->cost < spacing)
			spacing = tmpnode->cost;
		i++;
	}
	return spacing;
}

   
void cluster(node_t **mainarr, int **clusterar, int vertices, int clustersize) {
	int clusters = vertices;
	int vertex1, vertex2;
	int	i = 0, j = 0;
	int oldleader;
	node_t *tmpnode;
	int	*clusterarr = (int *)malloc(sizeof(int) * vertices);

	for (i = 1; i <= vertices; i++)
		clusterarr[i] = i;

	i = 0;
	while (clusters != clustersize) {
		tmpnode	= mainarr[i];
		vertex1	= tmpnode->vertex1;
		vertex2 = tmpnode->vertex2;
		if (clusterarr[vertex1] == clusterarr[vertex2]) { 
			i++;
			printf("computed %d %d : no change %d \n", vertex1, vertex2, clusters );
			for (j = 1; j <= vertices; j++)
				printf("%d : %d \t", j , clusterarr[j]);
			printf("\n");
			continue;
		}
		oldleader = clusterarr[vertex2];
		for (j = 1; j <= vertices; j++) {
			if (clusterarr[j] == oldleader) 
			  clusterarr[j] = clusterarr[vertex1]; 
		}
		i++;
		clusters--;
		printf("computed %d %d : cluster %d \n", vertex1, vertex2, clusters );
		for (j = 1; j <= vertices; j++)
			printf("%d : %d \t", j , clusterarr[j]);
		printf("\n");
	}
	*clusterar = clusterarr;
}

int cmpfunc(const void *a, const void *b) {
	int	cost1 = (*(node_t **)a)->cost;
	int	cost2 = (*(node_t **)b)->cost;

	if (cost1 > cost2)
		return (1);
	if (cost1 < cost2)
		return (-1);
	if (cost1 == cost2) {
		return (0);
	}

}

char *readfile(char *filename, node_t ***arr, int *vertices, int *lines) {
	FILE	*fp;
	char	*lineptr = NULL;
	size_t	n = 0;
	int		i = 0;
	char	*countstr;
	int		count;
	node_t	**mainarr;
	node_t	*tmpnode;
	char	*vertex1;
	char	*vertex2;
	char	*cost;

	fp = fopen(filename, "r");
	if (fp == NULL)
		printf("file couldn't be opened for reading");

	if (getline(&lineptr, &n, fp) != -1) {
	   countstr = strtok(lineptr, LIMITER);
	   *vertices = atoi(countstr);
	   printf("Total no of vertices is %d \n", *vertices);
	}

	mainarr = (node_t **) calloc(1, sizeof(node_t *) * MAXLINES);

	while (getline(&lineptr, &n, fp) != -1) {
		mainarr[i] = (node_t *) malloc(sizeof(node_t));
		vertex1 = strtok(lineptr, LIMITER);
		vertex2 = strtok(NULL, LIMITER);
		cost	= strtok(NULL, LIMITER);
		mainarr[i]->vertex1	= atoi(vertex1);
		mainarr[i]->vertex2	= atoi(vertex2);
		mainarr[i]->cost	= atoi(cost);
		mainarr[i]->leader	= -1;
		i++;
	}

	*lines = i;
	printf("Total no of lines is %d \n", *lines);

	*arr = mainarr;
}  

