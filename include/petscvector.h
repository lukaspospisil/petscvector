#ifndef PETSCVECTOR_H
#define	PETSCVECTOR_H

#include <iostream>
#include <string>

#define MAX_VIEWER_CHAR 255

class PetscVectorWriter; /* wrapper to allow vector(i) = value */

/* PETSc Vector */
class PetscVector {
		PetscErrorCode ierr; // TODO: I don't know what to do with errors
		Vec inner_vector;
	public:
		/* constructor */
		PetscVector(int n){
			ierr = VecCreate(PETSC_COMM_WORLD,&inner_vector);
			ierr = VecSetSizes(inner_vector,PETSC_DECIDE,n);
			ierr = VecSetFromOptions(inner_vector);

			valuesUpdate();
		}

		/* destructor */
		~PetscVector(){
//			ierr = VecDestroy(&vector);
		}
		
		/* get PETSC original vector */
		Vec get_vector(){ // TODO: temp
			return inner_vector;
		}

		/* after update a variable, it is necessary to call asseble begin */
		void valuesUpdate(){
			VecAssemblyBegin(inner_vector);
			VecAssemblyEnd(inner_vector);
		}
		
		/* set value of the vector, this function is called from overloaded operator */
		void set(int index, PetscScalar new_value){
			VecSetValue(inner_vector,index,new_value, INSERT_VALUES);
			valuesUpdate();
		}

		/* stream insertion operator */
		friend std::ostream &operator<<(std::ostream &output, const PetscVector &vector)		
		{
			PetscScalar *arr_vector;
			PetscInt i,local_size;
	
			VecGetLocalSize(vector.inner_vector,&local_size);
			VecGetArray(vector.inner_vector,&arr_vector);
			for (i=0; i<local_size; i++){
				output << arr_vector[i];
				if(i < local_size-1) output << ", ";
			}
			VecRestoreArray(vector.inner_vector,&arr_vector);
			
			return output;
		}

		/* get value with given id of the vector (works only with local id) */
		PetscScalar operator ()(int i) const
		{
			PetscInt ni = 1;
			PetscInt ix[1];
			PetscScalar y[1];
			
			ix[0] = i;
			VecGetValues(inner_vector,ni,ix,y);			
			
			return y[0];
		}

		/* set value with given id of the vector (works only with local id), will be defined after PetscVector */
		PetscVectorWriter operator()(int index);
	
};

/* wrapper to allow vector(i) = value */
class PetscVectorWriter
{
	PetscVector store; /* in this vector we want to store new value */
	int index; /* index of new value */
	
	public:
		/* constructor */
		PetscVectorWriter(PetscVector &s, int i): store(s), index(i) {}
		
		/* define assigment operator */
		PetscVectorWriter& operator=(PetscScalar const& new_value)
		{
			/* I am not able to access private vector, I pass it to orig class */
			store.set(index,new_value);
		}
	
};

/* return wrapper to be able to overload vector(index) = new_value */ 
PetscVectorWriter PetscVector::operator()(int index)
{   
	return PetscVectorWriter(*this, index);
}

#endif
