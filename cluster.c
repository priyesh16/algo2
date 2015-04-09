/*
 * -> main 
 *	-> read file
 *		 -> function to read file line by line
 *			 -> read line by line and store in an array(mainarr);
 *			 -> get the total number of jobs from the file and return it.
 *	-> sort mainarr based on score 
 *		-> get score of a job by subtracting length of job from its weight
 *		-> sort mainarr based on score 
 *	-> calculate completion time	
 *		-> compute completion time as sum of product of weights and lengths 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define LINELENGTH 20
#define LIMITER " "
#define MAXLINES 500

int cmpfunc(const void *a, const void *b);
char *readfile(char *filename, char ***arr, int *lines); 
unsigned long long completiontime(char **mainarr, int lines); 

int main(int argc , char *argv[]) {
	char	*filename;
	char	**mainarr;
	int		i = 0;
	int		lines;
	long long int sum;

	if (argc != 2) {
		printf("Too few or many arguments \n");
		exit(EXIT_FAILURE);
	}

	filename = argv[1];

	readfile(filename, &mainarr, &lines);

	qsort((void *)mainarr, lines, sizeof(char *), cmpfunc); 

	while (mainarr[i] != 0) {
		printf("mainarr %d = %s ", i , mainarr[i]);
		i++;
	}

//	sum = completiontime(mainarr, lines);
//	printf("completion time is %lld \n", sum);
	return sum;

}

unsigned long long completiontime(char **mainarr, int lines) {
	unsigned long long sum = 0;
	int weight;
	int length;
	unsigned long long timetillnow = 0;
	char *str = (char *)malloc(sizeof(char) * LINELENGTH);
	int	i;

	for (i = 0; i < lines; i++) {
		strcpy(str, mainarr[i]);
		weight = atoi(strtok(str, LIMITER));
		length = atoi(strtok(NULL, LIMITER));
		sum = sum + (weight * (timetillnow + length));
		timetillnow += length;
	}
	
	return sum;
}

int cmpfunc(const void *a, const void *b) {
	char *firststr = (char *)malloc(sizeof(char) * LINELENGTH);
	char *secondstr = (char *)malloc(sizeof(char) * LINELENGTH);
	char *coststr1, *coststr2;
	int	cost1, cost2;

	strcpy(firststr, *(char **)a);
	strcpy(secondstr, *(char **)b);


	/* get cost of firststring */

	strtok(firststr, LIMITER);
	strtok(NULL, LIMITER);
	coststr1= strtok(NULL, LIMITER);

	cost1 = atoi(coststr1); 
	printf("cost1 %d \n", cost1);

	/* get cost of firststring */

	strtok(secondstr, LIMITER);
	strtok(NULL, LIMITER);
	coststr2= strtok(NULL, LIMITER);

	printf("coststr %s \n", coststr2);
	cost2 = atoi(coststr2); 
	printf("cost2 %d \n", cost2);

//	printf("string %s : score %d \n", *(char **)a, score1);

	if (cost1 > cost2)
		return (1);
	if (cost1 < cost2)
		return (-1);
	if (cost1 == cost2) {
		return (0);
	}

}

char *readfile(char *filename, char ***arr, int *lines) {
	FILE	*fp;
	char	*lineptr = NULL;
	size_t	n = 0;
	int		i = 0;
	char	*countstr;
	int		count;
	char	**mainarr;

	fp = fopen(filename, "r");
	if (fp == NULL)
		printf("file couldn't be opened for reading");

	if (getline(&lineptr, &n, fp) != -1) {	
	   countstr = strtok(lineptr, LIMITER);
	   count = atoi(countstr);
	   printf("Total no of vertices is %d \n", count);
	}

	mainarr = (char **) calloc(1, sizeof(char *) * MAXLINES);

	while (getline(&lineptr, &n, fp) != -1) {
		mainarr[i] = (char *) malloc(sizeof(char ) * LINELENGTH);
		strcpy(mainarr[i], lineptr);  
		i++;
	}

	printf("Total no of lines is %d \n", i+1);

	*arr = mainarr;
	*lines = count;
}  

