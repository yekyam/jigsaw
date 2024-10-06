#include "jigsaw_utils.hpp"

void print_header() {
	print_for("*", 20);
	std::cout << "Test Start";

	print_for("*", 20);
	std::cout << '\n';
}

void print_footer(TesterInfo t)
{
	print_for("*", 20);
	std::cout << "[" << t.num_passed << "/" << t.num_tests << "] tests passed";
	print_for("*", 20);
	std::cout << "\n\n";
}


CompileAndRunStatus compile_and_run_program(std::string_view filename, Redirects r)
{
	bool skipped = true;

	if (filename.find(".c") != filename.npos) // c program, compile w gcc
	{
		skipped = false;
		std::vector<char *> proc_args;

		// args.args = {"gcc", filename.data(), "-Waall", "-Werror", NULL};
		proc_args.reserve(5);
		proc_args.push_back(const_cast<char*>("gcc"));
		proc_args.push_back(const_cast<char*>(filename.data()));
		proc_args.push_back(const_cast<char*>("-Wall"));
		proc_args.push_back(const_cast<char*>("-Werror"));
		proc_args.push_back(NULL);

		int ret = ChildProcArgs(proc_args, r, filename).fork_and_exec();

		if (ret != 0)
		{
			return CompileAndRunStatus::FAILURE_COMPILE;
		}

		std::vector<char *> run_args;

		run_args.reserve(2);
		run_args.push_back(const_cast<char *>("./a.out"));
		run_args.push_back(NULL);

		ret = ChildProcArgs(run_args, r, filename).fork_and_exec();

		if (ret != 0)
		{
			return CompileAndRunStatus::FAILURE_RUNTIME;
		}
	}
	else if (filename.find(".cpp") != filename.npos) // cpp program, compile w g++
	{
		skipped = false;
		std::vector<char *> proc_args;

		//args.args = {"g++", filename.data(), "-Waall", "-Werror", "-std=C++20", NULL};
		proc_args.reserve(6);
		proc_args.push_back(const_cast<char*>("g++"));
		proc_args.push_back(const_cast<char*>(filename.data()));
		proc_args.push_back(const_cast<char*>("-Wall"));
		proc_args.push_back(const_cast<char*>("-Werror"));
		proc_args.push_back(const_cast<char*>("-std=c++20"));
		proc_args.push_back(NULL);

		int ret = ChildProcArgs(proc_args, r, filename).fork_and_exec();

		if (ret != 0)
		{
			return CompileAndRunStatus::FAILURE_COMPILE;
		}

		std::vector<char *> run_args;

		run_args.reserve(2);
		run_args.push_back(const_cast<char *>("./a.out"));
		run_args.push_back(NULL);

		ret =  ChildProcArgs(run_args, r, filename).fork_and_exec();

		if (ret != 0)
		{
			return CompileAndRunStatus::FAILURE_RUNTIME;
		}
	}
	else if (filename.find(".py") != filename.npos) // py program, don't compile and just run
	{

	}

	if (skipped)
		return CompileAndRunStatus::SKIPPED;

	return CompileAndRunStatus::SUCCESS;
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
			char* file_name = dirs[num_ents]->d_name;


			CompileAndRunStatus status = compile_and_run_program(file_name, Redirects("logfile.out", "logfile.err"));

			remove(out_file.c_str());
			if (status == CompileAndRunStatus::SUCCESS)
			{
				tests.num_tests++;
				tests.num_passed++;
				// printf("%s%s\tPASSED", COLORS::GREEN, file_name);
				std::cout << "\t-" << COLORS::GREEN << file_name << "\t\tPASSED";
			}
			else if (status == CompileAndRunStatus::FAILURE_RUNTIME)
			{
				tests.num_tests++;
				//printf("%s%s\tFAILED", COLORS::RED, file_name);
				std::cout << "\t-"<< COLORS::RED << file_name << "\t\tFAILED";
			}
			else if (status == CompileAndRunStatus::FAILURE_COMPILE)
			{
				tests.num_tests++;

				std::cout << "\t-" << COLORS::RED << "couldn't compile file `" << file_name << "`";
			}
			else
			{
				continue;
			}
			std::cout << COLORS::NORMAL << '\n';
		}
		free(dirs[num_ents]);
	}
	
	free(dirs);
	return tests;
}
