#ifndef JIGSAW_UTILS_H
#define JIGSAW_UTILS_H

#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>

#include "colors.h"

typedef struct tester_info_t {
	int tests_total;
	int tests_passed;

} tester_info;

void print_header();
tester_info get_and_run_tests(char*);

int fork_and_exec(char*, int, char *, char **);
int compile_program(char *);
int run_program(char *);
char *lazy_strcat(char*, char *);
void print_footer(tester_info);
void print_for(char*, int);



#endif
