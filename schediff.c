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

int cmpfunc(const void *a, const void *b);
char *readfile(char *filename, char ***arr, int *lines); 

int main(int argc , char *argv[]) {
	char	*filename;
	char	**mainarr;
	int		i;
	int		lines;
	long long int sum;

	if (argc != 2) {
		printf("Too few or many arguments \n");
		exit(EXIT_FAILURE);
	}

	filename = argv[1];

	readfile(filename, &mainarr, &lines);

	qsort((void *)mainarr, lines, sizeof(char *), cmpfunc); 

	for (i = 0; i < lines; i++)
		printf("mainarr %d = %s ", i , mainarr[i]);

	sum = completiontime(mainarr, lines);
	printf("completion time is %lld \n", sum);
	return sum;

}

int completiontime(char **mainarr, int lines) {
	long long int sum = 0;
	int weight;
	int length;
	char *str = (char *)malloc(sizeof(char) * LINELENGTH);
	int	i;

	for (i = 0; i < lines; i++) {
		strcpy(str, mainarr[i]);
		weight = atoi(strtok(str, LIMITER));
		length = atoi(strtok(NULL, LIMITER));
		sum = sum + (weight * length);
	}
	
	return sum;
}

int cmpfunc(const void *a, const void *b) {
	char *firststr = (char *)malloc(sizeof(char) * LINELENGTH);
	char *secondstr = (char *)malloc(sizeof(char) * LINELENGTH);
	char *token;
	char *weight1, *weight2;
	char *length1, *length2;
	int	score1, score2;

	/*
	 * get score of firststring
	 */

	strcpy(firststr, *(char **)a);
	strcpy(secondstr, *(char **)b);

	weight1 = strtok(firststr, LIMITER);
	length1 = strtok(NULL, LIMITER);
	score1	= atoi(weight1) - atoi(length1);

	/*
	 * get score of secondstring
	 */
	weight2 = strtok(secondstr, LIMITER);
	length2 = strtok(NULL, LIMITER);
	score2	= atoi(weight2) - atoi(length2);

//	printf("string %s : score %d \n", *(char **)a, score1);

	if (score1 > score2)
		return (-1);
	if (score1 < score2)
		return (1);
	if (score1 == score2) {
		if (atoi(weight1) > atoi(weight2))
			return (-1);
		if  (atoi(weight1) < atoi(weight2))
			return (1);
		else  
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
	   printf("Total data to be read is %d \n", count);
	}

	mainarr = (char **) malloc(sizeof(char *) * count);

	while (getline(&lineptr, &n, fp) != -1) {
		mainarr[i] = (char *) malloc(sizeof(char ) * LINELENGTH);
		strcpy(mainarr[i], lineptr);  
		i++;
	}

	*arr = mainarr;
	*lines = count;
}  

