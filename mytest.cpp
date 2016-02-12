#include "petsc.h"

#include "petscvector.h"

int DEBUG_MODE = 100;

using namespace minlin::threx;

int main( int argc, char *argv[] )
{

	PetscErrorCode ierr;
	Vec x;
	int n = 5;

	PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
	PetscPrintf(PETSC_COMM_WORLD,"Hello World\n");

	PetscVector vec1(n);
	PetscVector vec2(n);

	PetscVector vec3;





	PetscFinalize();

	return 0;
}
