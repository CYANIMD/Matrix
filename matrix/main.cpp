#include "matrix.h"
#include "entity.h"
#include <vector>
#include "tasks.h"

using namespace IMD;

int main() {
	auto mrx1 = generate_central_spiral_matrix(2, 5);
	print(mrx1);
}