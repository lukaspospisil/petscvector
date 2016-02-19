#include "petscvector.h"

extern int DEBUG_MODE_PETSCVECTOR;
extern bool PETSC_INITIALIZED;


using namespace petscvector;



int main( int argc, char *argv[] )
{
	DEBUG_MODE_PETSCVECTOR = 0;

	int n = 5;

	PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
	PETSC_INITIALIZED = true;
	
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

    std::cout << H << std::endl;
    
    std::cout << D << std::endl;
    
    D = H;
    
    std::cout << D << std::endl;

    D(all) = 1;
    
    std::cout << D << std::endl;

DEBUG_MODE_PETSCVECTOR = 100;
	E = H + D;
//	PetscVectorWrapperCombNode aa = H;
DEBUG_MODE_PETSCVECTOR = 0;

	std::cout << E << std::endl;

//	std::cout << 2*H+D << std::endl;


	PETSC_INITIALIZED = false;
	PetscFinalize();

	return 0;
}
