
#include "matrix.h"

int main() {
	IMD::matrix<double> mrx1 { 2, 3, -1 };
	mrx1 *= -2.0;
	mrx1 /= 5.0;
	IMD::print(mrx1, " ");
}