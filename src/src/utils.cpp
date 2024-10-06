#include "utils.hpp"


void print_for(std::string_view s, int count, std::ostream& io) {
	for (int i = 0; i < count; i++) {
		io << s;
	}
}