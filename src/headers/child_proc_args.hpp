#ifndef CHILD_PROC_ARGS_HPP
#define CHILD_PROC_ARGS_HPP

#include "common.hpp"
#include "utils.hpp"

#define REDIRECT_ERR_MSG "Couldn't setup redirects\n"

enum RedirectStatus
{
	ERROR = -1,
	SUCCESS_STDOUT_ONLY,
	SUCCESS_STDERR_ONLY,
	SUCCESS
};

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

	RedirectStatus setup() {
		if (stdout_filename.size() != 0) {
			int ret = close(STDOUT_FILENO);
			int fd = open(stdout_filename.data(), O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

			if ((ret == -1 ) || (fd != STDOUT_FILENO)) {
				std::cout << REDIRECT_ERR_MSG << " for stdout\n";
				return RedirectStatus::ERROR;
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
					return RedirectStatus::SUCCESS_STDOUT_ONLY;
				}

				int ret_dup2 = dup2(fd, STDERR_FILENO);
				if (ret_dup2 == -1)
				{
					std::cout << "couldn't dup2???\n";
					return RedirectStatus::SUCCESS_STDOUT_ONLY;
				}

				if (fd != STDERR_FILENO) {
					std::cout << "stderr and fd don't match\n";
					return RedirectStatus::SUCCESS_STDOUT_ONLY;
				}
			}
		}
		return RedirectStatus::SUCCESS;
	}
};

class ChildProcArgs {
protected:
	std::vector<char*> args;
	std::optional<Redirects> redirects;
	const std::string_view source;

	void execute() {
		if (redirects.has_value())
		{
			RedirectStatus r = redirects->setup();
			if (r == RedirectStatus::ERROR)
			{
				std::cout << "Couldn't setup redirects!\n";
			}
			else
			{
				if ((r == RedirectStatus::SUCCESS) || (r==RedirectStatus::SUCCESS_STDERR_ONLY))
				{
					print_for("*", 20, std::cerr);
					std::cerr << source << " | " << args[0];
					print_for("*", 20, std::cerr);
					std::cerr << '\n';
				}
				if ((r == RedirectStatus::SUCCESS) || (r==RedirectStatus::SUCCESS_STDOUT_ONLY))
				{
					print_for("*", 20);
					std::cout << source << " | " << args[0];
					print_for("*", 20);
					std::cout << '\n';
				}
			}
		}

		char **commands = &args[0];

		execvp(args[0], commands);

		std::cerr << "Issue with exec!\n";
		perror(args[0]);
		exit(EXIT_FAILURE);
	}

public:

	ChildProcArgs(std::vector<char*> args, std::optional<Redirects> redirects, std::string_view source):
		args(args), 
		redirects(redirects),
		source(source)
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