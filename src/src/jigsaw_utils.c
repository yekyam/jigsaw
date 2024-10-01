#include "jigsaw_utils.h"

void print_header() {
	print_for("*", 20);
	printf("Test Start");
	print_for("*", 20);
	printf("\n");
}

void print_footer(tester_info t)
{
	print_for("*", 20);
	printf("[%d/%d] Tests Passed", t.tests_passed, t.tests_total);
	print_for("*", 20);
	printf("\n\n");

}

/* Forks the current process and execs the executable with the given args.
 * \param program The name of the executable (can be relative).
 * \param args The arguments to pass to execvp, must be null terminated.
 * \return Returns 0 on success, other if error.
 */
int fork_and_exec(char* output_file, char* program, char** args)
{
	int ret = fork();
	if (ret == -1)
	{
		perror("couldn't fork?");
		return -1;
	}
	else if (ret == 0)
	{
		if (output_file != NULL)
		{
			int close_ret = close(STDOUT_FILENO);
			if (close_ret == -1)
			{
				puts("couldn't close");
			}
			int ret = open(output_file, O_WRONLY);
			if (ret < 0)
			{
				puts("Couldn't open file??");
			}
			dup2(STDERR_FILENO, STDOUT_FILENO);
		}
		execvp(program, args);
		perror("couldn't exec");
		exit(EXIT_FAILURE);
	}
	else {
		int wstatus;
		wait(&wstatus);
		if (WIFEXITED(wstatus))
		{
			return WEXITSTATUS(wstatus);
		}
		return -1;
	}
}

/* Compiles a program at the given path to the default (a.out).
 * \param path The path to the program (can be relative).
 * \return Returns 0 on success, other if error.
 */
int compile_program(char* path)
{
	char *gcc_args[] = {"gcc", path, NULL};
	return fork_and_exec("./logfile", gcc_args[0], gcc_args);
}

/* Runs a executable in a child process.
 * \param path The path to the program (can be relative).
 * \return Returns 0 on success, other if error.
 */
int run_program(char* path)
{
	char *program_args[] = {path, NULL};
	return fork_and_exec(NULL, program_args[0], program_args);
}

tester_info get_and_run_tests(char* test_dir)
{
	chdir(test_dir);

	/* Procedure Overview
	 * 1) Get all files in the dir
	 * 2) For file in dir
	 *  - inc tests_total
	 * 	- compile program (assume a.out is the output)
	 *  - run (aka fork + exec) program
	 *  - if pass (aka ret == 0), inc tests_passed
	 *  - output program name (with colors!)
	 *  - delete compiled program
	 * 3) Print total
	 */
	struct dirent **dirs = NULL;

	tester_info tests = {.tests_passed = 0, .tests_total = 0};

	int num_ents = scandir(".", &dirs, NULL, NULL);

	char *out_file = "./a.out";


	if (num_ents == -1)
	{
		perror("couldn't use scandir");
		return tests;
	}

	while (num_ents--)
	{
		if (dirs[num_ents]->d_type == DT_REG)
		{
			char *file_name = dirs[num_ents]->d_name;

			tests.tests_total++;
			int ret = compile_program(file_name);

			if (ret != 0)
			{
				printf("\t-couldn't compile file: %s\n", file_name);
				continue;
			}
			
			ret = run_program(out_file);
			remove(out_file);

			printf("\t-");
			if (ret == 0)
			{
				tests.tests_passed++;
				printf("%s%s\tPASSED", COLORS.GREEN, file_name);
			} else {
				printf("%s%s\tFAILED", COLORS.RED, file_name);
			}
			printf("%s\n", COLORS.NORMAL);
		}
		free(dirs[num_ents]);
	}
	
	free(dirs);
	return tests;
}

void print_for(char* s, int count) {
	for (int i = 0; i < count; i++) {
		printf("%s", s);
	}
}

char* lazy_strcat(char* s1, char* s2)
{
	int s1_size = strlen(s1);
	int s2_size = strlen(s2);
	int total_size = s1_size + s2_size + 1;

	char *new_str = malloc(total_size);

	memcpy(new_str, s1, s1_size);
	memcpy(new_str + s1_size, s2, s2_size);
	new_str[total_size-1] = 0;

	return new_str;
}
