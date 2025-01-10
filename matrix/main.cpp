
#include "matrix.h"
#include "entity.h"

int main() {
	IMD::matrix<IMD::entity> mrx1{4, 2 };
	mrx1.tr();
	IMD::print(mrx1, " ");
}