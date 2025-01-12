
#include "matrix.h"
#include "entity.h"

int main() {
	auto mrx = IMD::generate_random_matrix(3, 3, 0, 5);
	print(mrx);
	std::cout << determinate(mrx);
}