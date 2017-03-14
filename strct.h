/******************************************************************************
* FILE: strct.h
* USE: #include "strct.h"
* AUTHOR: Ricardo Reais
* LAST REVISED: 16/11/16
******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#define BUFFER_SIZE 256

// -----------------
// Memory allocation
char *str_dup(const char *s)
{
  char *result = (char *) malloc(strlen(s) + 1);
  strcpy(result, s);
  return result;
}

int *ints_new (int n)
{
  return (int *) malloc (n * sizeof(int));
}

char *string_new (int n)
{
  return (char *) malloc (n + 1);
}

char **strings_new (int n)
{
  return (char **) malloc (n * sizeof(char *));
}
// -----------------
// QSolved

typedef struct
{
	int quizID;
  char *studentID;
	int nQuestions;
	int *ratings;
	char **solutions;
  char **answers;
} QSolved;

QSolved qSolved(int quizID, char *studentID, int nQuestions, int *ratings, char **solutions, char **answers)
{
  QSolved result;
  result.quizID=quizID;
  result.studentID=studentID;
  result.nQuestions=nQuestions;
  result.ratings=ratings;
  result.solutions=solutions;
  result.answers=answers;
  return result;
}

QSolved qSolved_read(FILE *f)
{
  int quizID, nQuestions;
  int *ratings; 
  char *studentID;
  char **solutions, **answers;
  char* BUFFER = calloc(BUFFER_SIZE, sizeof(char));
  //---------------------------------------------//
  fscanf(f, "%d", &quizID);
  //---------------------------------------------//
  fscanf(f, "%s", BUFFER);
  studentID = str_dup(BUFFER);
  //---------------------------------------------//
  fscanf(f, "%d", &nQuestions);
  //---------------------------------------------//
  ratings = ints_new(nQuestions);
  for (int i = 0; i < nQuestions; i++)
      fscanf(f, "%d", &ratings[i]);
  //---------------------------------------------//
  solutions = strings_new(nQuestions);
  for (int i = 0; i < nQuestions; i++)
  {
      fscanf(f, "%s", BUFFER);
      char* solution = str_dup(BUFFER);
      solutions[i] = solution;
  }
  //---------------------------------------------//
  answers = strings_new(nQuestions);
  for (int i = 0; i < nQuestions; i++)
  {
      fscanf(f, "%s", BUFFER);
      char* answer = str_dup(BUFFER);
      answers[i] = answer;
  }
  free(BUFFER);

  //---------------------------------------------//
  return qSolved(quizID, studentID, nQuestions, ratings, solutions, answers);
}

void qSolved_write(QSolved qs)
{
    printf("Quiz ID: %d\n", qs.quizID);
    //---------------------------------------------//
    printf("Student ID: %s\n", qs.studentID);
    //---------------------------------------------//
    printf("Number of questions: %d\n", qs.nQuestions);
    //---------------------------------------------//
    printf("Question ratings: ");
    for (int i = 0; i < qs.nQuestions; i++)
        printf("%d ", qs.ratings[i]);
    //---------------------------------------------//
    printf("\nQuiz solution: ");
    for (int i = 0; i < qs.nQuestions; i++)
        printf("%s ", qs.solutions[i]);
    //---------------------------------------------//
    printf("\nStudent answers: ");
    for (int i = 0; i < qs.nQuestions; i++)
        printf("%s ", qs.answers[i]);
    printf("\n");
}

// -----------------
// Evaluation
typedef struct
{
  char **answerResults;
  int nCorrectAnswers;
  int nAnswers;
  float correctAnswersPercentage;
  int studentPoints;
  int totalRating;
  int studentResult;
} Evaluation;

Evaluation evaluation(char **answerResults, int nCorrectAnswers, int nAnswers, float correctAnswersPercentage, int studentPoints, int totalRating, int studentResult)
{
  Evaluation result;
  result.answerResults=answerResults;
  result.nCorrectAnswers=nCorrectAnswers;
  result.nAnswers=nAnswers;
  result.correctAnswersPercentage=correctAnswersPercentage;
  result.studentPoints=studentPoints;
  result.totalRating=totalRating;
  result.studentResult=studentResult;
  return result;
}

// -----------------
// QEvaluated

typedef struct
{
  int quizID;
  char *studentID;
  char **answers;
  Evaluation e;  
} QEvaluated;

QEvaluated qEvaluated(int quizID, char *studentID, char **answers, Evaluation e)
{
  QEvaluated result;
  result.quizID=quizID;
  result.studentID=studentID;
  result.answers=answers;
  result.e=e;
  return result;
}

void qEvaluated_write(QEvaluated qe)
{
  int n = qe.e.nAnswers;
  //---------------------------------------------//
  printf("Quiz ID: %d\n", qe.quizID);
  //---------------------------------------------//
  printf("Student ID: %s\n", qe.studentID);
  //---------------------------------------------//
  printf("Student answers: ");
  for (int i = 0; i < n; i++)
    printf("%s ", qe.answers[i]);
  //---------------------------------------------//
  printf("\nAnswers results: ");
  for (int i = 0; i < n; i++)
    printf("%s ", qe.e.answerResults[i]);
  //---------------------------------------------//
  printf("\nNumber of correct answers: %d\n", qe.e.nCorrectAnswers);
  //---------------------------------------------//
  printf("Total number of Answers: %d\n", qe.e.nAnswers);
  //---------------------------------------------//
  printf("Correct answer percentage: %d%%\n", (int)qe.e.correctAnswersPercentage);
  //---------------------------------------------//
  printf("Number of points: %d\n", qe.e.studentPoints);
  //---------------------------------------------//
  printf("Total number points: %d\n", qe.e.totalRating);
  //---------------------------------------------//
  printf("Student final result: %d\n", qe.e.studentResult);
}

void qEvaluated_write_for_client(QEvaluated qe, FILE *out)
{
  int n = qe.e.nAnswers;
  //---------------------------------------------//
  fprintf(out, "%d correct answers in %d (%d %%)\n", qe.e.nCorrectAnswers, n, (int)qe.e.correctAnswersPercentage);
  //---------------------------------------------//
  fprintf(out, "%d points in %d possible (Final score = %d)\n", qe.e.studentPoints, qe.e.totalRating, qe.e.studentResult);
}
