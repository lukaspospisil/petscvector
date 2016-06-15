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
    PetscVector H;
    
    H.load_global("Koordinaten_EEG_P.bin");

    std::cout << "H:" << H << std::endl;

	petscvector::PETSC_INITIALIZED = false;
	PetscFinalize();

	return 0;
}
