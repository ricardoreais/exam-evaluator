# Exam evaluator

This is an automatic exam evaluator using parallel processing. (OpenMP)

Given folder with quizes with the structure:

Q[exam-id]Solved_[student-id].txt  *(e.g. Q1Solved_a52333.txt)*

being the file structure:

* [Exam id]
* [Student id]
* [Number of questions]
* [Question weight]
* [Student answers]
* [Expected answers]

![screenshot 1](https://github.com/ricardoreais/exam-evaluator/blob/master/examples/intro.png "Test 1")

[Folder containing example exams](https://github.com/ricardoreais/exam-evaluator/tree/master/Q1)

## Code Example

The function evaluates a folder containing exams solved by students, using parallel processing.

```Processing
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
```

## Getting Started
### Compiling the program

Using gnu compiler:

```sh 
gcc -fopenmp -std=c99 -Wall exam-evaluator.c
```

## Running the program

You can run the game using this syntax:

```sh
./a.out [Solved quizes folder] [Number of Threads]
```

or using Windows

```sh
./a.exe [Solved quizes folder] [Number of Threads]
```

## Running the tests

Using the folder Q1 as example and running on two threads:

```sh
./a.exe Q1 2
```
![screenshot 1](https://github.com/ricardoreais/exam-evaluator/blob/master/examples/test1.png "Test 1")

### Results

The automatic evaluator, compared each solved exam answers to the expected answers and returned a graded evaluation for each exam.

## Built With

* [Sublime text 3](https://www.sublimetext.com/) - The IDE used.
* [GNU Compiler](https://gcc.gnu.org/) - The compiler.

## Authors

* **Ricardo Reais** - *Initial work* - [My profile](https://github.com/ricardoreais)

See also the list of [contributors](https://github.com/ricardoreais/colored-lines/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
