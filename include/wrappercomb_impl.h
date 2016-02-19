#ifndef PETSCVECTOR_WRAPPERCOMB_IMPL_H
#define	PETSCVECTOR_WRAPPERCOMB_IMPL_H

namespace petscvector {

/* --------------------- PetscVectorWrapperComb ----------------------*/

/* constructor from node */
PetscVectorWrapperComb::PetscVectorWrapperComb(const PetscVectorWrapperCombNode &comb_node){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperComb)CONSTRUCTOR: WrapperComb" << std::endl;

	/* append node */
	this->append(comb_node);
}

/* constructor from vec */
PetscVectorWrapperComb::PetscVectorWrapperComb(const PetscVector &vec){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperComb)CONSTRUCTOR: from given PetscVector" << std::endl;

	/* create node from vector */
	PetscVectorWrapperCombNode comb_node(vec);

	/* append new node to newly created combination */
	this->append(comb_node);
}

/* constructor from subvector */
PetscVectorWrapperComb::PetscVectorWrapperComb(PetscVectorWrapperSub subvector){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperComb)CONSTRUCTOR: WrapperSub" << std::endl;

	/* create node from vector */
	PetscVectorWrapperCombNode comb_node(1.0,subvector.get_subvector());

	/* append new node to newly created combination */
	this->append(comb_node);

}

/* destructor */
PetscVectorWrapperComb::~PetscVectorWrapperComb(){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperComb)DESTRUCTOR" << std::endl;

	// TODO: destroy list

}

/* append new node to the list */
void PetscVectorWrapperComb::append(const PetscVectorWrapperCombNode &new_node){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperComb)FUNCTION: append(WrapperCombNode)" << std::endl;
	
	comb_list.push_back(new_node);
	
	/* store the size of the new node */
	this->vector_size = new_node.get_size();
}

/* append new list to the end of old list (merge without sort), will be called from overloaded operator+ */
void PetscVectorWrapperComb::merge(const PetscVectorWrapperComb &comb){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperComb)FUNCTION: merge(WrapperComb)" << std::endl;

	comb_list.insert(comb_list.end(), comb.comb_list.begin(), comb.comb_list.end());
}

/* get length of the list */
int PetscVectorWrapperComb::get_listsize() const {
	return comb_list.size();
}

/* get size of the vectors in the list */
int PetscVectorWrapperComb::get_vectorsize() const {
	return vector_size;
}

/* get frist vector from the list */
Vec PetscVectorWrapperComb::get_first_vector() {
	std::list<PetscVectorWrapperCombNode>::iterator list_iter; /* iterator through list */
	Vec vector;

	/* get first element and obtain a size of the vector */
	list_iter = comb_list.begin();
	vector = list_iter->get_vector();

	return vector;
}

/* prepare arrays from linear combination list, I assume that arrays are allocated */
void PetscVectorWrapperComb::get_arrays(PetscScalar *coeffs, Vec *vectors){
	std::list<PetscVectorWrapperCombNode>::iterator list_iter; /* iterator through list */
	int list_size = this->get_listsize();
	int j;

	/* set to the begin of the list */
	list_iter = comb_list.begin();

	/* go through the list and fill the vectors */
	for(j=0;j<list_size;j++){
		coeffs[j] = list_iter->get_coeff();
		vectors[j] = list_iter->get_vector();
		
		if(j < list_size-1){
			/* this is not the last element */
			list_iter++;
		}
	}

}

/* perform maxpy and store it into given Vec (allocated) */
void PetscVectorWrapperComb::maxpy(const Vec &y){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperComb)FUNCTION: maxpy(Vec)" << std::endl;

	int list_size = get_listsize();
	PetscScalar *alphas;
	Vec *vectors;

	/* allocate memory */
	TRY(PetscMalloc(sizeof(PetscScalar)*list_size,&alphas));
	TRY(PetscMalloc(sizeof(Vec)*list_size,&vectors));

	/* get array with coefficients and vectors */
	get_arrays(alphas,vectors);

	/* vec1 = vec1 + sum (coeff*vector) */
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << " - perform MAXPY" << std::endl;

	TRY( VecMAXPY(y,list_size,alphas,vectors) );

	/* free memory */
	TRY(PetscFree(alphas));
	TRY(PetscFree(vectors));

}


/* print linear combination without instance, f.x << alpha*vec1 + beta*vec2 */
std::ostream &operator<<(std::ostream &output, PetscVectorWrapperComb comb)
{
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperComb)OPERATOR: << comb" << std::endl;
		
	PetscInt i,j,vector_size,list_size;
	double value;
	
	std::list<PetscVectorWrapperCombNode>::iterator list_iter; /* iterator through list */
		
	output << "[";
	
	list_size = comb.get_listsize();
	vector_size = comb.get_vectorsize();
	
	/* go through components in the vector */
	for(i=0;i<vector_size;i++){
		list_iter = comb.comb_list.begin();
		
		/* for each component go throught the list */
		for(j=0;j<list_size;j++){
			/* print coeff, if coeff < 0, then print it in () */
			if(list_iter->get_coeff() < 0.0){
				output << "(" << list_iter->get_coeff() << ")";
			} else {
				output << list_iter->get_coeff(); 
			}

			output << "*";

			/* print value, if value < 0, then print it in () */
			value = list_iter->get_value(i);
			if(value < 0.0){
				output << "(" << value << ")";
			} else {
				output << value; 
			}

			if(j < list_size-1){ 
				/* this is not the last node */
				output << "+";
				list_iter++;
			}
		}
		
		if(i < vector_size-1){ 
			/* this is not the last element */
			output << ", ";
		}
	}

	output << "]";

	return output;
}

/* all nodes in linear combination will be scaled */
const PetscVectorWrapperComb operator*(double alpha, PetscVectorWrapperComb comb){
	/* scale nodes in the list */
	std::list<PetscVectorWrapperCombNode>::iterator list_iter; /* iterator through list */
	int list_size = comb.get_listsize();
	int j;

	/* set to the begin of the list */
	list_iter = comb.comb_list.begin();

	/* go through the list and scale nodes */
	for(j=0;j<list_size;j++){
		list_iter->scale(alpha);
		
		if(j < list_size-1){
			/* this is not the last element */
			list_iter++;
		}
	}
	
	return comb;
}

/* new linear combination created by comb + comb */
const PetscVectorWrapperComb operator+(PetscVectorWrapperComb comb1, PetscVectorWrapperComb comb2){
	/* append second linear combination to the first */
	comb1.merge(comb2);
	
	return comb1;
}

/* new linear combination created by comb - comb */
const PetscVectorWrapperComb operator-(PetscVectorWrapperComb comb1, PetscVectorWrapperComb comb2){
	/* append second linear combination to the first */
	comb2 = -1*comb2;
	comb1.merge(comb2);
	
	return comb1;
}

/* new linear combination created by comb + scalar */
const PetscVectorWrapperComb operator+(PetscVectorWrapperComb comb1, double scalar){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperComb)OPERATOR: comb + scalar" << std::endl;

	/* duplicate vector from comb */
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << " - create new vector of ones" << std::endl;
	Vec temp_vec;
	TRY( VecDuplicate(comb1.get_first_vector(), &temp_vec) );
	TRY( VecSet(temp_vec,scalar));
	
	/* prepare node, tell him to destroy the Vec at the end of the fun */
	PetscVectorWrapperCombNode temp_node(1.0,temp_vec,true);
	
	/* prepare new combination from node */
	PetscVectorWrapperComb comb2(temp_node);
	
	/* append second linear combination to the first */
	comb1.merge(comb2);
	
	return comb1;
}





/* --------------------- PetscVectorWrapperCombNode ----------------------*/

/* constructor default */
PetscVectorWrapperCombNode::PetscVectorWrapperCombNode(){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)CONSTRUCTOR: default" << std::endl;

}

/* constructor from PetscVector */
PetscVectorWrapperCombNode::PetscVectorWrapperCombNode(const PetscVector &vec){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)CONSTRUCTOR: (Vec)" << std::endl;
	set_vector(vec.get_vector());
	set_coeff(1.0);
	
	free_vec = false;
}

/* constructor from vector and coefficient */
PetscVectorWrapperCombNode::PetscVectorWrapperCombNode(double new_coeff, Vec new_vector){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)CONSTRUCTOR: (double,Vec)" << std::endl;
	set_vector(new_vector);
	set_coeff(new_coeff);
	
	free_vec = false;
}

/* constructor from vector, coefficient, and free_vec */
PetscVectorWrapperCombNode::PetscVectorWrapperCombNode(double new_coeff, Vec new_vector, bool new_free_vec){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)CONSTRUCTOR: (double,Vec,bool)" << std::endl;

	set_vector(new_vector);
	set_coeff(new_coeff);

	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "-------------- VEC CreAte in node -----------------" << std::endl;
	
	free_vec = new_free_vec;
}

/* destructor */
PetscVectorWrapperCombNode::~PetscVectorWrapperCombNode(){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)DESTRUCTOR" << std::endl;

	if(free_vec){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "-------------- VEC DEStROY in node -----------------" << free_vec << std::endl;
		
		TRY( VecDestroy(&inner_vector) );
	}
}

/* set vector to the node */
void PetscVectorWrapperCombNode::set_vector(Vec new_vector){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)FUNCTION: set_vector(Vec)" << std::endl;

	this->inner_vector = new_vector;
}

/* return vector from this node */
Vec PetscVectorWrapperCombNode::get_vector() const{
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)FUNCTION: get_vector()" << std::endl;

	return this->inner_vector;
}

/* set new coefficient to this node */
void PetscVectorWrapperCombNode::set_coeff(double new_coeff){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)FUNCTION: set_coeff(double)" << std::endl;

	this->coeff = new_coeff;
}

/* node is multiplied by scalar, now multiply only the coefficient of linear combination */
void PetscVectorWrapperCombNode::scale(double alpha){
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)FUNCTION: scale(double)" << std::endl;

	this->coeff *= alpha;
}

/* get the coefficient from this node */
double PetscVectorWrapperCombNode::get_coeff() const{
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)FUNCTION: get_coeff()" << std::endl;

	return this->coeff;
}

/* get size of the vector */
int PetscVectorWrapperCombNode::get_size() const{
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)FUNCTION: get_size()" << std::endl;

	int global_size;
	TRY( VecGetSize(this->inner_vector,&global_size) );
	return global_size;
}

/* get value from the vector, really slow */
int PetscVectorWrapperCombNode::get_value(int index) const{
	if(DEBUG_MODE_PETSCVECTOR >= 100) std::cout << "(WrapperCombNode)FUNCTION: get_value(int)" << std::endl;

	PetscInt ni = 1;
	PetscInt ix[1];
	PetscScalar y[1];
			
	ix[0] = index;
	TRY( VecGetValues(this->inner_vector,ni,ix,y) );	
			
	return y[0];
}




} /* end of petscvector namespace */

#endif
