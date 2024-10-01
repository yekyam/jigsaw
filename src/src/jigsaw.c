#include "jigsaw_utils.h"

/*TODO: make a redirect system to avoid clogging output*/

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		puts("usage: jigsaw (path_to_directory)");
		return 0;
	}
	char *test_dir = argv[1];
	print_header();
	tester_info t = get_and_run_tests(test_dir);
	print_footer(t);	
}
