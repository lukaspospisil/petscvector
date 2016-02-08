#include "petsc.h"

#include "petscvector.h"

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

	/* --------- set values ------------- */
	std::cout << "TEST: set values" << std::endl;

	vec1.set(1.0); // TODO: vec1(all) = 2
	vec2.set(2.0);

	std::cout << "vec1: " << vec1 << std::endl;
	std::cout << "vec2: " << vec2 << std::endl << std::endl;

	/* --------- copy vectors ------------- */
	std::cout << "TEST: copy vectors" << std::endl;

	vec2 = vec1;
	std::cout << "vec1: " << vec1 << std::endl;
	std::cout << "vec2: " << vec2 << std::endl << std::endl;
	
	/* --------- scalar multiplication ----------- */
	std::cout << "TEST: scalar multiplication" << std::endl;

	vec1 *= -2.0;
	std::cout << "vec1: " << vec1 << std::endl;
	std::cout << "vec2: " << vec2 << std::endl << std::endl;

	/* --------- addition ----------- */
	std::cout << "TEST: additions" << std::endl;

	vec1 = vec1 + vec2;
	std::cout << "vec1: " << vec1 << std::endl;
	std::cout << "vec2: " << vec2 << std::endl << std::endl;
	
	/* --------- dot product ----------- */
	std::cout << "TEST: dot product" << std::endl;

	std::cout << "dot(vec1,vec2): " << dot(vec1,vec2) << std::endl << std::endl;

	/* --------- test smart print of linear combination ---- */
	std::cout << "TEST: smart print of linear combination " << std::endl;

	std::cout << "linear combination: " << vec1 - 3*vec2 + 0.2*vec1 << std::endl << std::endl;
	

	/* --------- subvector ----------- */
	std::cout << "TEST: subvector" << std::endl;

	PetscVector vec3(n);
	vec3.set(5.0);

	vec3(1) = 5*vec1(2) - 0.5;

	std::cout << "vec3: " << vec3 << std::endl;


	PetscFinalize();

	return 0;
}
