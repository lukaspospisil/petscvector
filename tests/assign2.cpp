#include "petscvector.h"

using namespace petscvector;

extern int petscvector::DEBUG_MODE_PETSCVECTOR;
extern bool petscvector::PETSC_INITIALIZED;

int main( int argc, char *argv[] )
{
	petscvector::DEBUG_MODE_PETSCVECTOR = 0;

	PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
	petscvector::PETSC_INITIALIZED = true;
	
    int N = 5;

    // allocate storage
    PetscVector D(N);
	PetscVector C(D);
	PetscVector I(C);

	I = 1;

    // initialize input vectors
    C(0) = 3;  D(0) = 6;
    C(1) = 4;  D(1) = 7;
    C(2) = 0;  D(2) = 2;
    C(3) = 8;  D(3) = 1;
    C(4) = 2;  D(4) = 8;

    std::cout << C << std::endl;
    
    std::cout << D << std::endl;
    
    C(all) = D;
    
    std::cout << C << std::endl;

    std::cout << I << std::endl;

    C(all) += I;

    std::cout << C << std::endl;

    C -= I;

    std::cout << C << std::endl;

	petscvector::PETSC_INITIALIZED = false;
	PetscFinalize();

	return 0;
}
