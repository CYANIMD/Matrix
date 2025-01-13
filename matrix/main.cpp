
#include "matrix.h"
#include "entity.h"

int main() {
	auto mrx = IMD::generate_indentity_matrix<long>(3);
	mrx(2, 1) = -43;
	mrx(1, 1) = 5;
	println(mrx);
	std::cout << rang(mrx);
}