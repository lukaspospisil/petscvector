# petscvector

Simple C++ Petsc wrapper for manipulation with vectors. The library provides basic vector operations for Petsc Vec in MinLin-Matlab-style using overloaded operations.

Inspirated by [MINLIN](https://github.com/bcumming/minlin) 
(Minimal linear algebra library, Timothy Moroney, Ben Cumming)

>Created by Lukas Pospisil, USI Lugano, 2016 (during `PASC:HPC-Causality project` - Illia Horenko, Patrick Gagliardini, Will Sawyer)

## Installation on PIZ Daint and the first fun
- log into ELA machine using `ssh username@ela.cscs.ch` and afterwards into PIZ Daint using `ssh daint`
- clone repository using `git clone https://github.com/lukaspospisil/petscvector.git`
- load necessary modules using `source util/module_load_daint` 
- create folder `tests/build` (this folder is in `.gitignore`)
- get into created folder and call `cmake ..`, the makefile should be created
- call `make`
- if you want to run an example, you will have to use SLURM, for example at first allocate some nodes to interactive session
```
salloc -N 4 --account=**** --partition=normal
```
and afterwards, load modules again, and run sample using
```
aprun -n 4 ./program
```

## Operations
Currently there are only few operations available. See the list bellow.

###### create (allocate) vector using constructor:

- `PetscVector()` - allocate vector with unallocated inner Vec, Vec should be allocated afterwards
- `PetscVector(int n)` - allocate vector of dimension n
- `PetscVector(const PetscVector &vec1)` - duplicate input vec1 and copy values to new vector
- `PetscVector(Vec new_inner_vector)` - set inner Vec vector (i.e. pointer) to new allocated PetscVector

###### destroy (free) vector using destructor:

- `~PetscVector()` - destroy inner Vec

###### general methods, mostly used inside overloaded operators:

- `void valuesUpdate() const` - call VecAssemblyBegin and VecAssemblyEnd
- `int size() const` - get the global size of inner Vec
- `int local_size() const` - get local size of inner Vec
- `void get_ownership(int *low, int *high)` - get ownership of Vec
- `double get(int index)` - get one value with index from the Vec
- `void get_array(double **arr)` - get the double array from Vec, restore_array should be called consequently
- `void restore_array(double **arr)` - restore array after modifications into Vec
- `void scale(double alpha)` - scale all Vec values, call VecScale
- `void set(double new_value)` - set all values in Vec, call VecSet
- `void set(int index, double new_value)` - set value with index to new_value

###### assignment operators

- `PetscVector &operator=(const PetscVector &vec2)` [ y=x ] - copy values from one vector to another
- `PetscVector &operator=(PetscVectorWrapperCombNode combnode)` [ y=alpha*y, y=alpha*x ] 
- `PetscVector &operator=(PetscVectorWrapperComb comb)` [ y=sum{alpha_i*x_i} ]	
- `PetscVector &operator=(double scalar_value)` [ y=alpha ] - call set(scalar_value)

###### operators with subvectors

- `PetscVectorWrapperSub operator()(int index) const` [ y(index) ] - get subvector of length 1, which includes y(index)
- `PetscVectorWrapperSub operator()(int index_begin,int index_end) const` [ y(begin,end) ] - get subvector of stride index set from begin to end
- `PetscVectorWrapperSub operator()(const IS new_subvector_is) const` [ y(IS) ] - get subvector subject to given Petsc-index set, this index set will be not destroyed in PetscVectorWrapperSub-destroyer 

###### other operators

- `std::ostream &operator<<(std::ostream &output, const PetscVector &vector)` [ std::cout << y ] - print the vector, I decided to write my own function, not to use VecView
- `void operator*=(PetscVector &vec1, double alpha)` [ y*=alpha ] - scale vector values, call scale(alpha)
- `void operator+=(PetscVector &vec1, const PetscVectorWrapperComb comb)` [ y+=sum{alpha_i*x_i} ]
- `void operator-=(PetscVector &vec1, const PetscVectorWrapperComb comb)` [ y*=sum{alpha_i*x_i} ]

###### basic linear algebra functions

- `double dot(const PetscVector &vec1, const PetscVector &vec2)` [ dot(vec1,vec2) ] - compute dot product using VecDot
- `double max(const PetscVector &vec1)` [ max(vec1) ] - get maximum value in vector using VecMax 
- `double sum(const PetscVector &vec1)` [ sum(vec1) ] - compute the sum of the vector values using VecSum
- `double norm(const PetscVector &vec1)` [ norm(vec1) ] - compute the NORM_2 of the vector using VecNorm
- `const PetscVector operator/(const PetscVector &vec1, const PetscVector &vec2)` [vec3 = vec1/vec2]
