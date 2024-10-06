#ifndef JIGSAW_UTILS_HPP
#define JIGSAW_UTILS_HPP

#define _DEFAULT_SOURCE

#include "common.hpp"
#include "child_proc_args.hpp"
#include "colors.hpp"

/* Holds the number of tests and the number of tests passed
 */
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

/* Status of `compile_and_run_program(...)`
 */
enum class CompileAndRunStatus
{
	FAILURE_RUNTIME,
	FAILURE_COMPILE,
	SKIPPED,
	SUCCESS
};

/* Prints the header of jigsaw (just a bunch of astriesks and test start)
 */
void print_header();

/* Prints the footer of jigsaw (just a bunch of astriesks and test passed/test num)
 */
void print_footer(TesterInfo t);

/* Compiles and runs every file in the given directory, and reports info
 * \param test_dir The directory holding all of the files to test
 */
TesterInfo get_and_run_tests(char* test_dir);

/* Compiles and runs the given program. Also sets up redirects before both steps.
 * \param filename The name of the file
 * \param r The redirects to use 
 */
CompileAndRunStatus compile_and_run_program(std::string_view filename, Redirects r);

#endif
