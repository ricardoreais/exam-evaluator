/******************************************************************************
* FILE: evaluator_omp.c
* USE: 	$ gcc -fopenmp -std=c99 -Wall exam-evaluator.c
		$ ./a.out [Solved quizes folder] [Number of Threads]
* DESCRIPTION:
*   
* AUTHOR: Ricardo Reais
* LAST REVISED: 16/11/16
******************************************************************************/
#define MASTER 0
#define _BSD_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <omp.h>
#include "strct.h"

Evaluation get_evaluation(QSolved qs)
{
	int n = qs.nQuestions;
	char **answerResults = strings_new(n);
	int nCorrectAnswers = 0;
	float correctAnswersPercentage = 0.0;
	int studentPoints = 0;
	int totalRating = 0;
	int studentResult = 0;

	for (int i = 0; i < n; i++)
	{
		if(strcmp(qs.solutions[i], qs.answers[i]) == 0)
		{
			answerResults[i] = "Correct ";
			nCorrectAnswers++;
			studentPoints += qs.ratings[i];
		}
		else
			answerResults[i]="Incorrect ";
		totalRating+=qs.ratings[i];
	}

	correctAnswersPercentage = (float)nCorrectAnswers / n * 100.0;
	studentResult = 20 * studentPoints / totalRating;

	return evaluation(answerResults, nCorrectAnswers, n, correctAnswersPercentage, studentPoints, totalRating, studentResult);
}

void test_quiz_read_write(char *fileName)
{
    FILE *f = fopen(fileName, "r");
    QSolved qs = qSolved_read(f);
    fclose(f);
    Evaluation e = get_evaluation(qs);
    QEvaluated qe = qEvaluated(qs.quizID, qs.studentID, qs.answers, e);
    qEvaluated_write_for_client(qe, stdout);
    
}

char* concat(const char *s1, const char *s2)
{
    char *result = string_new(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


char *make_valid_dir(char *dirName) //Avoid error with directory not ending with /
{
	int n = strlen(dirName);
	if(dirName[n-1] != '/')
		return concat(dirName, "/");
	else
		return dirName;
}

void evalute(DIR *d, struct dirent * entry, char *strFinal, char *dirName)
{
	strFinal = concat(dirName, entry->d_name);
	test_quiz_read_write(strFinal);
	free(strFinal);
}

void multiple_quiz_final_evaluation(char *dirName, int NTHREADS)
{
	FILE *ls = popen("ls --quoting-style=escape -U Q1 | wc -l", "r");
	int numOfFiles;
	fscanf(ls, "%d", &numOfFiles);

    struct dirent * entry;
    dirName = make_valid_dir(dirName);
    DIR *d = opendir( dirName );
    char *strFinal;

    omp_set_num_threads(NTHREADS);

    if (d == 0)
        perror("opendir");
    else
    	#pragma omp parallel shared(d, dirName, strFinal, numOfFiles) default(none) 
    	{
    		if(omp_get_thread_num() == MASTER) printf("NUMBER OF THREADS=%d\n", omp_get_num_threads());

    		#pragma omp for private(entry) schedule(dynamic, 1)
		    	for(int i = 0; i < numOfFiles+2; i++)
					if((entry = readdir(d)))
					{
				    	if (!strcmp (entry->d_name, ".") || !strcmp (entry->d_name, ".."));
				    	else
				        	evalute(d, entry, strFinal, dirName);
				    }
	    }	
	printf("Finished!\n");
    closedir(d);
    pclose(ls);
}

int main (int argc, char **argv)
{
	if(argc > 2)
		multiple_quiz_final_evaluation(argv[1], atoi(argv[2]));
	else if (argc > 1)
		printf("Please insert number of threads!\n");
	else if (argc > 0)
		printf("Please insert number of threads!\nPlease insert a folder with exams!\n");
	return 0;
}
