#include "petsc.h"

#include "petscvector.h"

int main( int argc, char *argv[] )
{

	PetscErrorCode ierr;
	Vec x;
	int n = 5;

	PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
	PetscPrintf(PETSC_COMM_WORLD,"Hello World\n");

	PetscVector myvector(n);

	x = myvector.get_vector();
	
	ierr = VecSet(x,2); CHKERRQ(ierr);

	myvector(2) = 3.2;

	std::cout << "vector:" << myvector << std::endl;


	PetscFinalize();

	return 0;
}
