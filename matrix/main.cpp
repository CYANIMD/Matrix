
#include "matrix.h"
#include "entity.h"

int main() {
	IMD::matrix<int> mrx1{4, 2, 1 };
	IMD::matrix<int> mrx2{ 4, 2, 1 };
	IMD::matrix<double> mrx3{ 4, 2, 3.14 };
	mrx3 = mrx2 + mrx1;
	println(mrx3);

}