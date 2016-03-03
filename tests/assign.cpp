#include "petscvector.h"


using namespace petscvector;

extern int petscvector::DEBUG_MODE_PETSCVECTOR;
extern bool petscvector::PETSC_INITIALIZED;

int main( int argc, char *argv[] )
{
	DEBUG_MODE_PETSCVECTOR = 0;

	int n = 5;

	PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
	petscvector::PETSC_INITIALIZED = true;
	
    // allocate storage
    PetscVector H(n);
    PetscVector D = H;
    PetscVector E;

    // initialize input vectors
    H(0) = 3;  D(0) = 6;
    H(1) = 4;  D(1) = 7;
    H(2) = 0;  D(2) = 2;
    H(3) = 8;  D(3) = 1;
    H(4) = 2;  D(4) = 8;

    std::cout << "H:" << H << std::endl;
    std::cout << "D:" << D << std::endl;

	E = D;
	E = 1;

    std::cout << "E:" << E << std::endl;

	std::cout << 1 + 3*E - D << std::endl;
	std::cout << 3*E + 1 - D << std::endl;

DEBUG_MODE_PETSCVECTOR = 99;
	E += 0.5 + 2*E - D + 0.5;
DEBUG_MODE_PETSCVECTOR = 0;

	std::cout << E << std::endl;


	petscvector::PETSC_INITIALIZED = false;
	PetscFinalize();

	return 0;
}
