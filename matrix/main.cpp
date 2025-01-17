#include "matrix.h"
#include "entity.h"
#include <vector>

using namespace IMD;

int main() {
	matrix<int> mrx1 = generate_random_matrix(3, 3, -2, 2);
	println(mrx1);
	reverse_cols(mrx1);
	println(mrx1);
}