#include "petscvector.h"

using namespace petscvector;

extern int petscvector::DEBUG_MODE_PETSCVECTOR;
extern bool petscvector::PETSC_INITIALIZED;

typedef petscvector::PetscVector Vector;

int main( int argc, char *argv[] )
{

	int N = 5;

	PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
	petscvector::PETSC_INITIALIZED = true;

    // initialize input vectors
    Vector A(N);
    Vector B(A);

    // initialize input vectors
    A(0) = 3;  B(0) = 6;
    A(1) = 4;  B(1) = 7;
    A(2) = 0;  B(2) = 2;
    A(3) = 8;  B(3) = 1;
    A(4) = 2;  B(4) = 8;

    std::cout << A << std::endl;
    std::cout << B << std::endl;

	DEBUG_MODE_PETSCVECTOR = 100;
	B = A + 1.0;
	DEBUG_MODE_PETSCVECTOR = 1;

    std::cout << B << std::endl;
//    std::cout << Vector(1 + A) << std::endl;
/*
    std::cout << 1 - A << std::endl;
    std::cout << Vector(1 - A) << std::endl;

    std::cout << 2 * A << std::endl;
    std::cout << Vector(2 * A) << std::endl;

    std::cout << A + 1 << std::endl;
    std::cout << Vector(A + 1) << std::endl;

    std::cout << A - 1 << std::endl;
    std::cout << Vector(A - 1) << std::endl;
*/
    A(all) = B + B;
    
	petscvector::PETSC_INITIALIZED = false;
	PetscFinalize();

	return 0;
}
