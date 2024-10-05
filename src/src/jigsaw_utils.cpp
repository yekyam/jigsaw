#include "jigsaw_utils.hpp"

void print_header() {
	print_for("*", 20);
	printf("Test Start");
	print_for("*", 20);
	printf("\n");
}

void print_footer(TesterInfo t)
{
	print_for("*", 20);
	std::cout << "[" << t.num_passed << "/" << t.num_tests << "] tests passed";
	print_for("*", 20);
	std::cout << "\n\n";
}

/* Forks the current process and execs the executable with the given args.
 * \param program The name of the executable (can be relative).
 * \param args The arguments to pass to execvp, must be null terminated.
 * \return Returns 0 on success, other if error.
 */
int fork_and_exec(std::string_view output_file, int flags, const char* program,  const char** args)
{
	int ret = fork();
	if (ret == -1)
	{
		perror("couldn't fork?");
		return -1;
	}
	else if (ret == 0)
	{
		if (output_file.size() != 0)
		{
			int close_ret = close(STDOUT_FILENO);
			int other_close_ret = close(STDERR_FILENO);
			if ((close_ret == -1) || (other_close_ret == -1))
			{
				puts("couldn't close");
			}
			int ret = open(output_file.data(), flags, S_IWUSR | S_IRUSR);
			if (ret < 0)
			{
				puts("Couldn't open file??");
			}
			dup2(ret, STDOUT_FILENO);
			dup2(STDERR_FILENO, STDOUT_FILENO);
			print_for("*", 20);
		}
		execvp(program, (char* const*)args);
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
int compile_program(const std::string& path)
{
	char const *gcc_args[] = {"gcc", path.c_str(), NULL};
	return fork_and_exec("./logfile.txt", O_WRONLY | O_CREAT | O_APPEND, gcc_args[0], gcc_args);
}

/* Runs a executable in a child process.
 * \param path The path to the program (can be relative).
 * \return Returns 0 on success, other if error.
 */
int run_program(const std::string& path)
{
	const char *program_args[] = {path.c_str(), NULL};
	return fork_and_exec("./program_output.txt", O_WRONLY | O_CREAT | O_APPEND, program_args[0], program_args);
}

TesterInfo get_and_run_tests(char* test_dir)
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

	TesterInfo tests;

	int num_ents = scandir(".", &dirs, NULL, NULL);

	std::string out_file = "./a.out";


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

			tests.num_tests++;
			int ret = compile_program(file_name);

			if (ret != 0)
			{
				printf("\t-couldn't compile file: %s\n", file_name);
				continue;
			}
			
			ret = run_program(out_file);
			remove(out_file.c_str());

			printf("\t-");
			if (ret == 0)
			{
				tests.num_passed++;
				// printf("%s%s\tPASSED", COLORS::GREEN, file_name);
				std::cout << COLORS::GREEN << file_name << "\t\tPASSED";
			}
			else
			{
				//printf("%s%s\tFAILED", COLORS::RED, file_name);
				std::cout << COLORS::RED << file_name << "\t\tFAILED";
			}
			std::cout << COLORS::NORMAL << '\n';
		}
		free(dirs[num_ents]);
	}
	
	free(dirs);
	return tests;
}

void print_for(std::string_view s, int count) {
	for (int i = 0; i < count; i++) {
		std::cout << s;
	}
}
