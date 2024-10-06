#ifndef JIGSAW_UTILS_HPP
#define JIGSAW_UTILS_HPP

#define _DEFAULT_SOURCE

#include "common.hpp"
#include "child_proc_args.hpp"
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

enum class CompileAndRunStatus
{
	FAILURE_RUNTIME,
	FAILURE_COMPILE,
	SKIPPED,
	SUCCESS
};

void print_header();
TesterInfo get_and_run_tests(char*);

int compile_and_run_program(std::string_view);
void print_footer(TesterInfo);
void print_for(std::string_view, int);



#endif
