#ifndef CHILD_PROC_ARGS_HPP
#define CHILD_PROC_ARGS_HPP

#include "common.hpp"
#include "utils.hpp"

#define REDIRECT_ERR_MSG "Couldn't setup redirects\n"

class Redirects {
protected:

	std::string_view stdout_filename;
	std::string_view stderr_filename;

public:
	Redirects() : 
	stdout_filename(), 
	stderr_filename()
	{
	}

	Redirects(std::string_view stdout_filename, std::string_view stderr_filename):
	stdout_filename(stdout_filename),
	stderr_filename(stderr_filename)
	{}

	int setup() {
		if (stdout_filename.size() != 0) {
			int ret = close(STDOUT_FILENO);
			int fd = open(stdout_filename.data(), O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

			if ((ret == -1 ) || (fd != STDOUT_FILENO)) {
				std::cout << REDIRECT_ERR_MSG << " for stdout\n";
				return -1;
			}
		}
		
		if (stderr_filename.size() != 0) {
			if (stdout_filename == stderr_filename)
			{
				dup2(STDOUT_FILENO, STDERR_FILENO);
				std::cout << "here!\n";
			}
			else
			{
				int ret = close(STDERR_FILENO);
				int fd = open(stderr_filename.data(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

				if (ret == -1) {
					std::cout << "couldn't close stderr!\n";
					return -1;
				}

				int ret_dup2 = dup2(fd, STDERR_FILENO);
				if (ret_dup2 == -1)
				{
					std::cout << "couldn't dup2???\n";
					return -1;
				}

				if (fd != STDERR_FILENO) {
					std::cout << "stderr and fd don't match\n";
					return -1;
				}
				else
				{
					// std::cout << "fixed?????\n";
					print_for("*", 20);
					std::cerr << stderr_filename;
					print_for("*", 20);
					std::cerr << '\n';
				}
			}
		}
		return 0;
	}
};

class ChildProcArgs {
protected:
	std::vector<char*> args;
	std::optional<Redirects> redirects;

	void execute() {
		if (redirects.has_value())
		{
			int r = redirects->setup();
			if (r == -1)
			{
				std::cout << "Couldn't setup redirects!\n";
			}
		}

		char **commands = &args[0];

		execvp(args[0], commands);

		std::cerr << "Issue with exec!\n";
		perror(args[0]);
		exit(EXIT_FAILURE);
	}

public:

	ChildProcArgs(std::vector<char*> args, std::optional<Redirects> redirects):
		args(args), 
		redirects(redirects)
	{}

	int fork_and_exec()
	{
		int pid = fork();
		int ret = 0;
		switch (pid)
		{
		case -1: // error
			perror("Couldn't fork; shutting down");
			exit(EXIT_FAILURE);
			break;
		case 0: // child
			execute();
			break;
		default: // parent
			int wstatus;
			waitpid(pid, &wstatus, 0);
			ret = WEXITSTATUS(wstatus);
		}
		return ret;
	}
};

#endif