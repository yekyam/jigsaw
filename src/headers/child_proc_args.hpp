#ifndef CHILD_PROC_ARGS_HPP
#define CHILD_PROC_ARGS_HPP

#include "redirects.hpp"


/* A class to hold the necessary info (program, arguments, and redirects) to run a given program as a child process.
 * Used to abstract over the standard fork/exec pattern
 */
class ChildProcArgs {
protected:
	std::vector<char*> args;
	std::optional<Redirects> redirects;
	const std::string_view source;

	void execute();

public:
	/* Constructs an instance.
	 * \param args A vector of the program name followed by the arguments to the program. Must be NULL terminated.
	 * \param redirects An optional instance of the files to redirect the STDOUT/STDERR of the child
	 * \source Extra info for the STDOUT/STDERR messages
	 */
	ChildProcArgs(std::vector<char *> args, std::optional<Redirects> redirects, std::string_view source);

	/* Forks and then runs the program with the arguments specified in this instance
	 * \return -1 on failure, 0 on success.
	 */
	int fork_and_exec();
};

#endif