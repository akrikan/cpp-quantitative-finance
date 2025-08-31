#ifndef _SIMPLEMATRIX_CPP
#define _SIMPLEMATRIX_CPP

#include "simplematrix.h"


template <typename Type>
SimpleMatrix<Type> :: SimpleMatrix(){};

template <typename Type>
SimpleMatrix<Type> :: SimpleMatrix(const int& rows, const int& columns ,const Type& val){
    for (int i = 0; i < rows; i++){
        std::vector<Type> col_vec(columns,val);
        mat.push_back(col_vec);
    }
}

// overloaded asigment operator
template <typename Type>
SimpleMatrix<Type> &SimpleMatrix<Type>::operator=(const SimpleMatrix<Type>& _rhs){
    if(this == &_rhs)return *this;
    mat = _rhs.get_mat();
    return *this;
}
//Destructor 

template <typename Type> 
SimpleMatrix<Type>:: ~SimpleMatrix() {}

// Matrix access method , via copying
template <typename Type>
std::vector<std::vector<Type> > SimpleMatrix<Type>::get_mat() const {
    return mat;
}

// Matrix access method, via row and column index
template <typename Type>
Type& SimpleMatrix<Type>::value(const int& row, const int& col) {
    return mat[row][ col ]; 
}

#endif