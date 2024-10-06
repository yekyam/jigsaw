#include "redirects.hpp"


Redirects::Redirects():
	stdout_filename(), 
	stderr_filename()
	{}


Redirects::Redirects(std::string_view stdout_filename, std::string_view stderr_filename):
	stdout_filename(stdout_filename),
	stderr_filename(stderr_filename)
	{}

RedirectStatus Redirects::setup() {
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