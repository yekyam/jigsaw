#include "utils.hpp"


void print_for(std::string_view s, int count) {
	for (int i = 0; i < count; i++) {
		std::cout << s;
	}
}