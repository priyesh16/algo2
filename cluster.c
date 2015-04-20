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
#define MAXLINES 300000

typedef struct node_s {
	long long vertex1;
	long long vertex2;
	long long cost;
	long long leader;
} node_t;

int cmpfunc(const void *a, const void *b);
char *readfile(char *filename, node_t ***arr, long long *vertices, long long *lines); 
void cluster(node_t **mainarr, long long **clusterarr, long long vertices, long long clustersize); 
long long maxspacing(node_t **mainarr, long long *clusterarr, long long lines); 

int main(int argc , char *argv[]) {
	char	*filename;
	node_t	**mainarr;
	long long		i = 0;
	long long		lines;
	long long		vertices;
	long long		spacing;
	long long		*clusterarr;
	long long		space;

	if (argc != 3) {
		printf("Too few or many arguments \n");
		exit(EXIT_FAILURE);
	}

	filename = argv[2];
	space	= atoi(argv[1]);

	readfile(filename, &mainarr, &vertices, &lines);

	qsort((void *)mainarr, lines, sizeof(char *), cmpfunc); 

	while (mainarr[i] != 0) {
		printf("mainarr %lld = %lld %lld %lld \n", i , mainarr[i]->vertex1, mainarr[i]->vertex2, mainarr[i]->cost);
		i++;
	}

	cluster(mainarr, &clusterarr, vertices, space);
	spacing	= maxspacing(mainarr, clusterarr, lines);
	printf("max spacing is %lld \n", spacing);
}

long long maxspacing(node_t **mainarr, long long *clusterarr, long long lines) {
	long long	i = 0;
	long long spacing = 0;
	long long vertex1, vertex2;
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

   
void cluster(node_t **mainarr, long long **clusterar, long long vertices, long long clustersize) {
	long long clusters = vertices;
	long long vertex1, vertex2;
	long long	i = 0, j = 0;
	long long oldleader;
	node_t *tmpnode;
	long long	*clusterarr = (long long *)malloc(sizeof(long long) * vertices);

	for (i = 1; i <= vertices; i++)
		clusterarr[i] = i;

	i = 0;
	while (clusters != clustersize) {
		tmpnode	= mainarr[i];
		vertex1	= tmpnode->vertex1;
		vertex2 = tmpnode->vertex2;
		if (clusterarr[vertex1] == clusterarr[vertex2]) { 
			i++;
			/*
			printf("computed %lld %lld : no change %lld \n", vertex1, vertex2, clusters );
			for (j = 1; j <= vertices; j++)
				printf("%lld : %lld \t", j , clusterarr[j]);
			printf("\n");
			*/
			continue;
		}
		oldleader = clusterarr[vertex2];
		for (j = 1; j <= vertices; j++) {
			if (clusterarr[j] == oldleader) 
			  clusterarr[j] = clusterarr[vertex1]; 
		}
		i++;
		clusters--;
		/*
		printf("computed %lld %lld : cluster %lld \n", vertex1, vertex2, clusters );
		for (j = 1; j <= vertices; j++)
			printf("%lld : %lld \t", j , clusterarr[j]);
		printf("\n");
		*/
	}
	*clusterar = clusterarr;
}

int cmpfunc(const void *a, const void *b) {
	long long	cost1 = (*(node_t **)a)->cost;
	long long	cost2 = (*(node_t **)b)->cost;

	if (cost1 > cost2)
		return (1);
	if (cost1 < cost2)
		return (-1);
	if (cost1 == cost2) {
		return (0);
	}

}

char *readfile(char *filename, node_t ***arr, long long *vertices, long long *lines) {
	FILE	*fp;
	char	*lineptr = NULL;
	size_t	n = 0;
	long long		i = 0;
	char	*countstr;
	long long		count;
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
	   printf("Total no of vertices is %lld \n", *vertices);
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
	printf("Total no of lines is %lld \n", *lines);

	*arr = mainarr;
}  

