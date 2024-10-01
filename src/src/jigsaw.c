#include "jigsaw_utils.h"

/*TODO: make a redirect system to avoid clogging output*/

int main()
{
	print_header();
	tester_info t = get_and_run_tests("./tests");
	print_footer(t);	
}
