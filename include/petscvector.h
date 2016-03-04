/** @file petscvector.h
 *  @brief Header file with class delarations.
 *
 *  This defines the basic layout of all used classes. In the end of the file,
 *  the files with specific implementations are included.
 *  This is the file which has to be included in the project to provide the work 
 *  with Petsc vectors in Min-Lin Matlab style.
 *
 *  @author Lukas Pospisil
 *  @bug No known bugs.
 */

#ifndef PETSCVECTOR_H
#define	PETSCVECTOR_H

/* include petsc */
#include "petsc.h"

/* std:list for linear combinations */
#include <list>

/* basic input/output in c++ */
#include <iostream>

/* for manipulating with strings */
#include <string>

/* to deal with errors, call Petsc functions with TRY(fun); */
static PetscErrorCode ierr; /**< to deal with PetscError */

/**
 * \def TRY(f)
 * Macro for dealing with PetscError. Each original Petsc function could by called using TRY(...).
*/
#define TRY( f) {ierr = f; do {if (PetscUnlikely(ierr)) {PetscError(PETSC_COMM_SELF,__LINE__,PETSC_FUNCTION_NAME,__FILE__,ierr,PETSC_ERROR_IN_CXX,0);}} while(0);}

/* we are using namespace petscvector */
namespace petscvector {

int DEBUG_MODE_PETSCVECTOR = true; /**< defines the debug mode of the functions */
bool PETSC_INITIALIZED = false; /**< to deal with PetscInitialize and PetscFinalize outside this class */

/* define "all" stuff */
class petscvector_all_type {} all; /**< brings an opportunity to call PetscVector(all) */

/* wrapper to allow manipulation with linear combinations of vectors */
class PetscVectorWrapperComb;

/* one node of previous wrapper */
class PetscVectorWrapperCombNode;

/* wrapper to allow subvectors */
class PetscVectorWrapperSub; 

/** \class PetscVector
 *  \brief General class for manipulation with vectors.
 *
*/
class PetscVector {
	private:
		Vec inner_vector; /**< original Petsc Vector */
		
	public:

		/** @brief The basic constructor.
		* 
		*  Sets the inner vector to NULL.
		*
		*/
		PetscVector();

		/** @brief Create constructor.
		*
		*  Create new vector of given size n.
		*
		*  @param n global size of new vector
		*/ 
		PetscVector(int n);
		
		/** @brief Duplicate constructor.
		*
		*  Create new vector by duplicating given one.
		*
		*  @param vec original vector to be duplicated
		*/ 
		PetscVector(const PetscVector &vec1);

		/** @brief Constructor from Vec.
		*
		*  Construct new vector from given Vec.
		*
		* @param new_inner_vector original Vec
		*/
		PetscVector(const Vec &new_inner_vector);

		/** @brief Constructor from linear combination.
		*
		*  Creates a new vector from given linear combination.
		*
		*  @param new_inner_vector original Vec
		*/ 
		PetscVector(const PetscVectorWrapperComb &comb);

		/** @brief Destructor.
		*
		*  If inner vector is present, then destroy it.
		*
		*/ 
		~PetscVector();

		/** @brief Update values in inner vector.
		*
		*  Calls VecAssemblyBegin and VecAssemblyEnd.
		*
		*/
		void valuesUpdate() const;

		/** @brief Scale all values in inner vector.
		*
		*  Call VecScale.
		* 
		*  @param alpha the scaling coeficient
		*/ 
		void scale(double alpha);

		/** @brief Get inner vector.
		*
		*  @return original inner vector
		*  @todo this function is temporary
		*/ 
		Vec get_vector() const; // TODO: temp, direct access to inner vector should be forbidden

		/** @brief Get size of inner vector.
		*
		*  @return global size of inner vector
		*  @todo control if inner_vector was allocated
		*/ 
		int size() const;

		/** @brief Get local size of inner vector.
		*
		*  @return local size of inner vector
		*  @todo control if inner_vector was allocated
		*/ 
		int local_size() const;

		/** @brief Get single value.
		*
		*  Return single value with given index of component.
		* 
		*  @note works only with local id, really slow
		*  @return global size of inner vector
		*  @todo control if inner_vector was allocated
		*/ 
		double get(int index);
		
		/** @brief Get ownership of global vector.
		*
		*  Call VecGetOwnershipRange, get the indeces of local components.
		* 
		*  @param low start index
		*  @param high end index + 1
		*  @todo control if inner_vector was allocated
		*/ 
		void get_ownership(int *low, int *high);

		/** @brief Get local array from vector.
		*
		*  Call VecGetArray.
		* 
		*  @note call restore_array after changes in array
		*  @param arr array of vector
		*  @todo control if inner_vector was allocated
		*/ 
 		void get_array(double **arr);

		/** @brief Restore local array to vector.
		*
		*  Call VecRestoreArray.
		* 
		*  @note has to be called after get_array	
		*  @param arr array of vector
		*  @todo control if get_array was called
		*/ 
		void restore_array(double **arr);

		/** @brief Set values in inner vector.
		*
		*  Set all values of the vector to given value, this function is called from overloaded operator.
		*
		*  @param new_value new value of all components
		*  @todo control if inner_vector was allocated
		*/ 
		void set(double new_value);

		/** @brief Update value in inner vector.
		*
		*  Set one specific component of the vector to given value, this function is called from overloaded operator.
		*
		*  @param index index of component
		*  @param new_value new value of component
		*  @todo control if inner_vector was allocated
		*/ 
		void set(int index, double new_value);

		/* assignment */
		PetscVector &operator=(const PetscVector &vec2);
		PetscVector &operator=(double scalar_value);
		PetscVector &operator=(PetscVectorWrapperComb comb);

		friend void operator*=(PetscVector &vec1, double alpha);
		friend void operator+=(const PetscVector &vec1, const PetscVectorWrapperComb comb);
		friend void operator-=(PetscVector &vec1, const PetscVectorWrapperComb comb);

		/* subvector */
		PetscVectorWrapperSub operator()(int index) const;
		PetscVectorWrapperSub operator()(int index_begin,int index_end) const;
		PetscVectorWrapperSub operator()(const IS new_subvector_is) const;
		PetscVectorWrapperSub operator()(petscvector_all_type all_type) const; /* define operator PetscVector(all)  */

		/* print */
		friend std::ostream &operator<<(std::ostream &output, const PetscVector &vector);

		/* binary operations */
		friend double dot(const PetscVector &vec1, const PetscVector &vec2);
		friend double max(const PetscVector &vec1);
		friend double sum(const PetscVector &vec1);
		friend double norm(const PetscVector &vec1);

		/* other operations */
		friend const PetscVector operator/(const PetscVector &vec1, const PetscVector &vec2);

};


/** \class PetscVectorWrapperComb
 *  \brief Wrapper to allow linear combinations of vectors.
 *
 *  The linar combinations of vector is stored in the list of PetscVectorWrapperComb.
 *  Finally, using the assignment operator in PetscVector, function compute() is called.
*/
class PetscVectorWrapperComb
{
	private:
		std::list<PetscVectorWrapperCombNode> comb_list; /* the list of linear combination nodes */
		
		int vector_size; /* store the global size of the last added vector */
		
	public:
		/* constructors and destructor */
		PetscVectorWrapperComb();
		PetscVectorWrapperComb(const PetscVector &vec);
		PetscVectorWrapperComb(const PetscVectorWrapperCombNode &comb_node);
		PetscVectorWrapperComb(PetscVectorWrapperSub subvector);

		~PetscVectorWrapperComb();

		/* general functions */
		int get_listsize() const;
		int get_vectorsize() const;
		Vec get_first_vector();
		void append(const PetscVectorWrapperCombNode &new_node);
		void merge(const PetscVectorWrapperComb &comb);
		void compute(const Vec &y, double init_scale);
		
		/* print */
		friend std::ostream &operator<<(std::ostream &output, PetscVectorWrapperComb comb);

		/* operations with combinations */
		friend const PetscVectorWrapperComb operator*(double alpha, PetscVectorWrapperComb comb);
		friend const PetscVectorWrapperComb operator+(PetscVectorWrapperComb comb1, PetscVectorWrapperComb comb2);
		friend const PetscVectorWrapperComb operator-(PetscVectorWrapperComb comb1, PetscVectorWrapperComb comb2);

		friend const PetscVectorWrapperComb operator+(PetscVectorWrapperComb comb1, double scalar);
		friend const PetscVectorWrapperComb operator+(double scalar,PetscVectorWrapperComb comb2);
		
};


/*! \class PetscVectorWrapperComb
    \brief Wrapper with one node in the linear combinations.

*/
class PetscVectorWrapperCombNode
{
	private:
		Vec inner_vector; /* pointer to vector (original Petsc Vec) in linear combination */
		double coeff; /* coefficient in linear combination */

	public:
		/* constructors and destructor */
		PetscVectorWrapperCombNode();
		PetscVectorWrapperCombNode(const PetscVector &vec);
		PetscVectorWrapperCombNode(double new_coeff, Vec new_vector);
		PetscVectorWrapperCombNode(double new_coeff );

		~PetscVectorWrapperCombNode();


		/* general functions */
		void set_vector(Vec new_vector);
		Vec get_vector() const;
		int get_size() const;
		int get_value(int index) const;
		
		void set_coeff(double new_coeff);
		void scale(double alpha);
		double get_coeff() const;



};

/*! \class PetscVectorWrapperSub
    \brief Wrapper with subvectors.

	For manipulation with subvectors. Based on Petsc function GetSubvector (in constructor) and RestoreSubVector (in destructor).
*/
class PetscVectorWrapperSub
{
	private:
		IS subvector_is;
		Vec inner_vector; /* original vector */
		Vec subvector; /* subvector */

		bool free_is; /* free index set in destructor */

	public:

		PetscVectorWrapperSub(Vec inner_vector, IS subvector_is, bool new_free_is);
		~PetscVectorWrapperSub();

		void valuesUpdate() const;
		void scale(PetscScalar alpha) const;

		void set(double new_value);		
		Vec get_subvector();

		double get(int index);

		/* print */
		friend std::ostream &operator<<(std::ostream &output, const PetscVectorWrapperSub &wrapper);				

		/* assignment operator */
		PetscVectorWrapperSub &operator=(PetscVectorWrapperSub subvec); /* subvec = subvec */
		PetscVectorWrapperSub &operator=(double scalar_value);	 /* subvec = const */
		PetscVectorWrapperSub &operator=(const PetscVector &vec2); /* subvec = vec */
		PetscVectorWrapperSub &operator=(PetscVectorWrapperComb comb);	

		friend void operator*=(const PetscVectorWrapperSub &subvec1, double alpha); /* subvec = alpha*subvec */
		friend void operator+=(const PetscVectorWrapperSub &subvec1, const PetscVectorWrapperComb comb);
		friend void operator-=(const PetscVectorWrapperSub &subvec1, const PetscVectorWrapperComb comb);
		friend void operator/=(const PetscVectorWrapperSub &subvec1, const PetscVectorWrapperSub subvec2);

		/* boolean operations */
		friend bool operator==(PetscVectorWrapperSub subvec1, double alpha);
		friend bool operator==(PetscVectorWrapperSub subvec1, PetscVectorWrapperSub subvec2);

		friend bool operator>(PetscVectorWrapperSub vec1, PetscVectorWrapperSub vec2);

		/* binary operations */
		friend double sum(const PetscVectorWrapperSub subvec1);
		friend double dot(const PetscVectorWrapperSub subvec1, const PetscVectorWrapperSub subvec2);


};




} /* end of petsc vector namespace */


/* add implementations */
#include "petscvector_impl.h"
#include "wrappercomb_impl.h"
#include "wrappersub_impl.h"

#endif
