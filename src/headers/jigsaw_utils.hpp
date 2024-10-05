#ifndef JIGSAW_UTILS_HPP
#define JIGSAW_UTILS_HPP

#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>

#include <iostream>
#include <string>
#include <string_view>

#include "colors.hpp"

struct TesterInfo {
	int num_tests;
	int num_passed;

	TesterInfo() {
		num_tests = 0;
		num_passed = 0;
	}

	TesterInfo(int num_tests, int num_passed)
	{
		this->num_tests = num_tests;
		this->num_passed = num_passed;
	}
};

void print_header();
TesterInfo get_and_run_tests(char*);

int fork_and_exec(std::string_view, int, const char *, const char **);
int compile_program(const std::string&);
int run_program(const std::string&);
void print_footer(TesterInfo);
void print_for(std::string_view, int);



#endif
