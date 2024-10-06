#ifndef REDIRECTS_HPP
#define REDIRECTS_HPP

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
	Redirects();

	Redirects(std::string_view stdout_filename, std::string_view stderr_filename);

	RedirectStatus setup();
};

#endif
