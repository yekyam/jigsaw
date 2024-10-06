#include "child_proc_args.hpp"


void ChildProcArgs::execute()
{
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

ChildProcArgs::ChildProcArgs(std::vector<char*> args, std::optional<Redirects> redirects, std::string_view source):
		args(args), 
		redirects(redirects),
		source(source)
	{}


int ChildProcArgs::fork_and_exec()
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