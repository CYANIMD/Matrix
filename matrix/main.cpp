
#include "matrix.h"
#include "entity.h"

int main() {
	IMD::matrix<int> mrx1{4, 2, 1 };
	IMD::matrix<int> mrx2{ 4, 2, -3 };
	mrx1(2, 1) = 0;
	println(mrx1);
    rotate(mrx1);
    println(mrx1);
}