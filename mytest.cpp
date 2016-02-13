#include "petsc.h"

#include "petscvector.h"

int DEBUG_MODE = 100;
bool PETSC_INITIALIZED = false;

using namespace minlin::threx;

int main( int argc, char *argv[] )
{
	DEBUG_MODE = 0;

	PetscErrorCode ierr;
	Vec x;
	int n = 5;

	PetscInitialize(&argc,&argv,PETSC_NULL,PETSC_NULL);
	PETSC_INITIALIZED = true;
	
	PetscPrintf(PETSC_COMM_WORLD,"Hello World\n");

	PetscVector vec1(n);
	PetscVector vec2(n);
	PetscVector vec3;

	vec1.set(10.2);
	vec2.set(-5.3);

	if(false){
		std::cout << "------------------------------------" << std::endl;
		std::cout << "TEST assignment" << std::endl;
		vec3 = vec1;
	}


	if(false){
		std::cout << "------------------------------------" << std::endl;
		std::cout << "TEST <<" << std::endl;
		std::cout << "vec1: " << vec1 << std::endl;
		std::cout << "vec2: " << vec2 << std::endl;
	}

	if(false){
		std::cout << "------------------------------------" << std::endl;
		std::cout << "TEST linear combinations" << std::endl;

		PetscVector test_lc;
		test_lc = 2*vec1 - 1*vec2 + vec1;
		std::cout << "test_lc: " << test_lc << std::endl;		
	}

	if(true){
		std::cout << "------------------------------------" << std::endl;
		std::cout << "TEST subvector" << std::endl;

		IS is;
		int idxs[2];
		idxs[0] = 1;
		idxs[1] = 2;
	
		TRY( ISCreateGeneral(PETSC_COMM_WORLD, 2, idxs, PETSC_COPY_VALUES, &is));

		
		PetscVector test;
		
	
		test = vec1(is);

	DEBUG_MODE = 100;

		std::cout << vec1(2,4) << std::endl;

		vec1(3) += vec1(1) + 2*vec1(4);

	DEBUG_MODE = 0;

//		vec1(is) = 1.0;

		std::cout << "test: " << test << std::endl;
		std::cout << "vec1: " << vec1 << std::endl;
//		std::cout << "vec1: " << vec1(is) << std::endl;
//		std::cout << "vec1: " << vec1 << std::endl;
	}




//	PetscVectorWrapperComb prdel;
//	prdel = 5*vec1;

//	std::cout << "vec1 + vec2: " << vec1 + vec2 << std::endl;
/*
	PetscVector vec4, vec5, vec6(n);

	vec4 = vec3 + vec1;
	std::cout << "vec4 = vec3 + vec1 = " << vec4 << std::endl;

*/






	PetscPrintf(PETSC_COMM_WORLD,"\nBye World\n");

	PETSC_INITIALIZED = false;
	PetscFinalize();



	return 0;
}
