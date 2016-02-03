#include "petsc.h"

#include "petscvector.h"

int main( int argc, char *argv[] )
{

	PetscErrorCode ierr;
	Vec x;
	int n = 5;

	PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
	PetscPrintf(PETSC_COMM_WORLD,"Hello World\n");

	PetscVector vec1(n);
	PetscVector vec2(n);

	/* --------- set values ------------- */
	std::cout << "TEST: set values" << std::endl;

	vec1.set(1.0); // TODO: vec1(all) = 2
	vec2.set(2.0);

	vec1(1) = 3.2;

	std::cout << "vec1: " << vec1 << std::endl;
	std::cout << "vec2: " << vec2 << std::endl << std::endl;

	/* --------- copy vectors ------------- */
	std::cout << "TEST: copy vectors" << std::endl;

	vec2 = vec1;
	vec2(0) = -1.0;
	std::cout << "vec1: " << vec1 << std::endl;
	std::cout << "vec2: " << vec2 << std::endl << std::endl;
	
	/* --------- scalar multiplication ----------- */
	std::cout << "TEST: scalar multiplication" << std::endl;

	vec1 *= -2.0;
	std::cout << "vec1: " << vec1 << std::endl;
	std::cout << "vec2: " << vec2 << std::endl << std::endl;

	/* --------- addition ----------- */
	std::cout << "TEST: additions" << std::endl;

	vec1 = vec1 - vec2;
	std::cout << "vec1: " << vec1 << std::endl;
	std::cout << "vec2: " << vec2 << std::endl << std::endl;
	
	

	PetscFinalize();

	return 0;
}
