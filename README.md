# petscvector

Simple C++ Petsc wrapper for manipulation with vectors

Created by Lukas Pospisil, USI Lugano, 2016

Inspirated by [MINLIN](https://github.com/bcumming/minlin) 
(Minimal linear algebra library, Timothy Moroney, Ben Cumming)

The library provides basic vector operations for Petsc Vec in MinLin-Matlab-style using overloaded operations.

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
and afterwards, load module again and run sample using
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

