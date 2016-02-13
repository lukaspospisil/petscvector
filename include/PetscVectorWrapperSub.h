
namespace minlin {

namespace threx { // TODO: maybe choose the different namespace for my own Petsc stuff


/* PetscVectorWrapperSub constructor with given IS = create subvector */
PetscVectorWrapperSub::PetscVectorWrapperSub(Vec new_inner_vector, IS new_subvector_is, bool new_free_is){
	if(DEBUG_MODE >= 100) std::cout << "(WrapperSub)CONSTRUCTOR: WrapperSub(inner_vec, IS)" << std::endl;

	/* free index set during destruction ? */
	free_is = new_free_is;

	/* store old inner vector - will be used in the destructor to return subvector */
	inner_vector = new_inner_vector; 

	/* copy IS */
	subvector_is = new_subvector_is;

	if(DEBUG_MODE >= 100) std::cout << " - get subvector from original vector" << std::endl;

	/* get subvector, restore it in destructor */
	TRY( VecGetSubVector(inner_vector, subvector_is, &subvector) );
	
}

/* PetscVectorWrapperSub destructor */
PetscVectorWrapperSub::~PetscVectorWrapperSub(){
	if(DEBUG_MODE >= 100) std::cout << "(WrapperSub)DESTRUCTOR: ~WrapperSub" << std::endl;

	/* if this was a subvector, then restore values */
	if(DEBUG_MODE >= 100) std::cout << " - restore subvector" << std::endl;
	TRY( VecRestoreSubVector(inner_vector, subvector_is, &subvector) );

	/* if it is necessary to free IS, then free it */
	if(free_is){
		if(DEBUG_MODE >= 100) std::cout << " - destroy IS" << std::endl;
		TRY( ISDestroy(&subvector_is) );
	}

}

/* set all values of the subvector, this function is called from overloaded operator */
void PetscVectorWrapperSub::set(double new_value){
	if(DEBUG_MODE >= 100) std::cout << "(WrapperSub)FUNCTION: set(double)" << std::endl;

	// TODO: control if subvector was allocated

	TRY( VecSet(this->subvector,new_value) );

	valuesUpdate();
}

/* return vector from this node */
Vec PetscVectorWrapperSub::get_subvector(){
	if(DEBUG_MODE >= 100) std::cout << "(WrapperSub)FUNCTION: get_subvector()" << std::endl;	
	
	return this->subvector;
}

/* after update a variable, it is necessary to call asseble begin & end */
void PetscVectorWrapperSub::valuesUpdate(){
	if(DEBUG_MODE >= 100) std::cout << "(WrapperSub)FUNCTION: valuesUpdate()" << std::endl;

	TRY( VecAssemblyBegin(subvector) );
	TRY( VecAssemblyEnd(subvector) );
}

/* subvector = alpha*subvector */
void PetscVectorWrapperSub::scale(PetscScalar alpha){
	if(DEBUG_MODE >= 100) std::cout << "(PetscVector)FUNCTION: scale(double)" << std::endl;

	//TODO: control subvector

	TRY( VecScale(subvector, alpha) );
	valuesUpdate(); // TODO: has to be called?
}

/* stream insertion << operator */
std::ostream &operator<<(std::ostream &output, const PetscVectorWrapperSub &wrapper)		
{
	if(DEBUG_MODE >= 100) std::cout << "(WrapperSub)OPERATOR: <<" << std::endl;

	PetscScalar *arr_vector;
	PetscInt i,local_size;
	const PetscInt *indices;

	output << "[";
	TRY( VecGetLocalSize(wrapper.subvector,&local_size) );

	TRY( VecGetArray(wrapper.subvector,&arr_vector) );
	TRY( ISGetIndices(wrapper.subvector_is,&indices) );

	for (i=0; i<local_size; i++){
		output << "{" << indices[i] << "}=" << arr_vector[i];
		if(i < local_size-1) output << ", ";
	}

	TRY( ISRestoreIndices(wrapper.subvector_is,&indices) );
	TRY( VecRestoreArray(wrapper.subvector,&arr_vector) );
	output << "]";
			
	return output;
}


/* subvec = scalar_value <=> subvec(all) = scalar_value, assignment operator */
PetscVectorWrapperSub &PetscVectorWrapperSub::operator=(double scalar_value){
	if(DEBUG_MODE >= 100) std::cout << "(WrapperSub)OPERATOR: (subvec = double)" << std::endl;

	this->set(scalar_value);
	return *this;	
}

/* subvec1 = vec2, assignment operator (set vector) */
PetscVectorWrapperSub &PetscVectorWrapperSub::operator=(const PetscVector &vec2){
	if(DEBUG_MODE >= 100) std::cout << "(WrapperSub)OPERATOR: (subvec = vec)" << std::endl;

	/* vec1 is not initialized yet */
	if (!subvector){
		//TODO: give error
	}

	/* else copy the values of inner vectors */
	if(DEBUG_MODE >= 100) std::cout << " - copy values" << std::endl;		
	
	VecCopy(vec2.get_vector(),subvector); // TODO: I dont know how to do without this
	this->valuesUpdate(); // TODO: has to be called?
	
	return *this;	
}



/* vec1 = linear_combination_node, perform simple linear combination */
PetscVectorWrapperSub &PetscVectorWrapperSub::operator=(PetscVectorWrapperCombNode combnode){
	if(DEBUG_MODE >= 100) std::cout << "(WrapperSub)OPERATOR: (subvec = combnode)" << std::endl;

	/* vec1 is not initialized yet */
	if (!subvector){
		//TODO: give error
	}

	/* else copy the vector values and then scale */
	if(DEBUG_MODE >= 100) std::cout << " - copy values" << std::endl;		

	TRY( VecCopy(combnode.get_vector(),subvector));
	
    this->scale(combnode.get_coeff());

	return *this;	
}

/* vec1 = linear_combination, perform full linear combination */
PetscVectorWrapperSub &PetscVectorWrapperSub::operator=(PetscVectorWrapperComb comb){
	if(DEBUG_MODE >= 100) std::cout << "(WrapperSub)OPERATOR: (subvec = comb)" << std::endl;

	/* vec1 is not initialized yet */
	if (!subvector){
		//TODO: give error
	}

	/* subvec = 0, we will perform MAXPY (y += lin_comb) */
	if(DEBUG_MODE >= 100) std::cout << " - set vector LHS = 0" << std::endl;		
	TRY( VecSet(subvector,0.0) );

	/* subvec += comb */
//	*this += comb; 

	return *this;	
}

/* subvec *= alpha */
void operator*=(PetscVectorWrapperSub vec1, double alpha)
{
	if(DEBUG_MODE >= 100) std::cout << "(PetscVector)OPERATOR: vec *= double" << std::endl;
	
	vec1.scale(alpha);
}

/* vec1 += comb */
void operator+=(PetscVectorWrapperSub vec1, PetscVectorWrapperComb comb)
{
	if(DEBUG_MODE >= 100) std::cout << "(PetscVector)OPERATOR: vec += comb" << std::endl;
	
	int list_size = comb.get_listsize();
	PetscScalar alphas[list_size];
	Vec vectors[list_size];

	/* get array with coefficients and vectors */
	comb.get_arrays(alphas,vectors);

	/* vec1 = vec1 + sum (coeff*vector) */
	if(DEBUG_MODE >= 100) std::cout << " - perform MAXPY" << std::endl;
	VecMAXPY(vec1.subvector,list_size,alphas,vectors);
	vec1.valuesUpdate();

}

/* vec1 -= comb */
void operator-=(PetscVectorWrapperSub vec1, PetscVectorWrapperComb comb)
{
	if(DEBUG_MODE >= 100) std::cout << "(PetscVector)OPERATOR: vec -= comb" << std::endl;
	
	vec1 += (-1.0)*comb;
}





}

}
